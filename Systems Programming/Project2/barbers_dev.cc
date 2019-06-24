//
// barbers.cc
//
// Simulate the sleeping barbers problem.
//
// Author: Morris Bernstein
// Copyright 2019, Systems Deployment, LLC.


#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <random>
#include <time.h>
#include <unistd.h>
#include <vector>

using namespace std;

const char* PROG_NAME = "";


class Shop;
class Barber;
class Customer;

enum customer_state { cust_init, waiting, ready_to_sit, receiving_haircut, finished_haircut , paying };
enum barber_state { barber_init, sleeping, receiving_customer, cutting_hair, receiving_payment };

class Lock {
public:
  Lock(pthread_mutex_t* mutex): mutex(mutex) {
    int rc = pthread_mutex_lock(this->mutex);
    if (rc < 0) {
      perror("can't lock mutex");
      exit(EXIT_FAILURE);
    }
  }

  ~Lock() {
    int rc = pthread_mutex_unlock(mutex);
    if (rc < 0) {
      perror("can't unlock mutex");
      exit(EXIT_FAILURE);
    }
  }

private:
  pthread_mutex_t* mutex;
};


class Shop {
public:

  // static bool isShopClosed;

  struct BarberOrWait {
    Barber* barber;		// Barber is available
    bool chair_available;	// If barber isn't available,
				// waiting chair is available.
  };

  // Constructor initializes shop and creates Barber threads (which
  // will immediately start calling next_customer to fill the
  // collection of sleeping barbers).
  Shop(int n_barbers,
       unsigned waiting_chairs,
       int average_service_time_ms,
       int service_time_deviation_ms,
       int average_customer_arrival_ms,
       int duration);

  // Main thread: open the shop and spawn customer threads until
  // closing time.  Report summary statistics for the day.
  void run();

  // Customer thread announces arrival to shop. If the collection of
  // currently sleeping barbers is not empty, remove and return one
  // barber from the collection. If all the barbers are busy and there
  // is an empty chair in the waiting room, add the customer to the
  // waiting queue and return {nullptr, true}.  Otherwise, the
  // customer will leave: return {nullptr, false}.
  BarberOrWait arrives(Customer* customer);

  // Barber thread requests next customer.  If no customers are
  // currently waiting, add the barber to the collection of
  // currently sleeping barbers and return nullptr.
  Customer* next_customer(Barber* barber);

  // Return random service time
  int service_time();

  // Return random customer arrival.
  unsigned int customer_arrival_time();

  //customer thread calls this method to put him/her on the waiting list
  void waitInWaitingRoom(Customer* customer);

  //customer thread calls this method to put him/her on the waiting list
  void barberSleepInSeat(Barber* barber);

  #if defined(VERBOSE)
  void showSleepingBarbers(){
    {
      Lock lock(mutex);
      cout << "-----currently " << barber_queue.size() << " barbers sleeping" << endl;
    }
  }
  #endif
private:
  int n_barbers;
  unsigned waiting_chairs;
  double average_service_time_ms;
  double service_time_deviation_ms;
  double average_customer_arrival_ms;

  vector<pthread_t*> barber_threads;
  pthread_mutex_t* mutex;// Protect shared access.

  struct timespec time_limit;

  int customers_served_immediately = 0;
  int customers_waited = 0;
  int customers_turned_away = 0;
  int customers_total = 0;

  vector<Barber*> barbers;
  static queue<Barber*> barber_queue;//barbers sleeping queue
  static queue<Customer*> customer_queue;//waiting customers
};

// bool Shop::isShopClosed = false;

queue<Barber*> Shop::barber_queue;//barbers sleeping queue
queue<Customer*> Shop::customer_queue;//waiting customers

class Barber {
public:
  Barber(Shop* shop, int id);

  // Barber thread function.
  void run();

  // Shop tells barber it's closing time.
  void closing_time();

  // Customer wakes up barber.
  void awaken(Customer* customer);

  // Customer sits in barber's chair.
  void customer_sits();

  // Customer proffers payment.
  void payment();

  const int id;

  //sleeps while waiting for barber to finish
  void sleep();

  //wakes up
  void wakeup();

private:
  Shop* shop;
  pthread_mutex_t* mutex;
  pthread_cond_t* cond;

  Customer* myCustomer = nullptr;
  bool shopClosed = false;
  barber_state state;
};

class Customer {
  // friend Shop;
public:
  Customer(Shop* shop, int id);
  ~Customer();

  // Customer thread function.
  void run();

  // Barber calls this customer after checking the waiting-room
  // queue (customer should be waitint).
  void next_customer(Barber* barber);

  // Barber finishes haircut (Customer should be receiving haircut).
  void finished();

  // Barber has accepted payment for service.
  void payment_accepted();

  const int id;

  //sleeps while waiting for barber to finish
  void sleep();

  //wakes up
  void wakeup();

private:
  Shop* shop;
  pthread_mutex_t* mutex;
  pthread_cond_t* cond;

  Barber* myBarber = nullptr;
  customer_state state;
};


// Barber thread.  Must be joined by Shop thread to allow shop to
// determine when barbers have left.
void* run_barber(void* arg) {
  Barber* barber = reinterpret_cast<Barber*>(arg);
  barber->run();
  return nullptr;
}


// Customer thread.  Runs in detatched mode so resources are
// automagically cleaned up when customer leaves shop.
void* run_customer(void* arg) {
  Customer* customer = reinterpret_cast<Customer*>(arg);
  customer->run();
  delete customer;
  return nullptr;
}


Shop::Shop(int n_barbers,
	   unsigned int waiting_chairs,
	   int average_service_time_ms,
	   int service_time_deviation_ms,
	   int average_customer_arrival_ms,
	   int duration):
        n_barbers(n_barbers), 
        waiting_chairs(waiting_chairs),
        average_service_time_ms(average_service_time_ms),
        service_time_deviation_ms(service_time_deviation_ms), 
        average_customer_arrival_ms(average_customer_arrival_ms)
{

    int rc = clock_gettime(CLOCK_REALTIME, &time_limit);
    if (rc < 0) {
        perror("reading realtime clock");
        exit(EXIT_FAILURE);
    }
    // Round to nearest second
    time_limit.tv_sec += duration + (time_limit.tv_nsec >= 500000000);
    time_limit.tv_nsec = 0;

    
    //Initialize mutex
    mutex = reinterpret_cast<pthread_mutex_t*>(malloc(sizeof(pthread_mutex_t)));//allocate memory for mutex
    rc = pthread_mutex_init(mutex, NULL);
    if (rc != 0) {
      errno = rc;
      perror("initializing mutex");
      exit(EXIT_FAILURE);
    }

    //barbers are serving customers
}


void Shop::run() {
  cout << "the shop opens" << endl;

  //create n_barbers threads for each barber
  for(int next_barber_id = 0; next_barber_id < n_barbers; next_barber_id++)
  {
    Barber* barber = new Barber(this, next_barber_id);
    barbers.push_back(barber);
    pthread_t* thread = reinterpret_cast<pthread_t*>(calloc(1, sizeof(pthread_t)));
    int rc = pthread_create(thread, nullptr,
                            ::run_barber, reinterpret_cast<void *>(barber));
    if (rc != 0) {//exit if it failed to create a thread
        errno = rc;
        perror("creating pthread");
        exit(EXIT_FAILURE);
    }

    barber_threads.push_back(thread);
  }

  //create customers
  for (int next_customer_id = 0; ; next_customer_id++) 
  {
    struct timespec now;
    int rc = clock_gettime(CLOCK_REALTIME, &now);
    if (now.tv_sec >= time_limit.tv_sec) {
      // Shop closes.
      break;
    }

    #if defined(VERBOSE)
      cout << "-----" << now.tv_sec - time_limit.tv_sec << " seconds remaining until shop closes-----" << endl;
    #endif

    //my code
    Customer* customer = new Customer(this, next_customer_id);
    pthread_t* thread = reinterpret_cast<pthread_t*>(calloc(1, sizeof(pthread_t)));
    rc = pthread_create(thread, nullptr,
                            ::run_customer, reinterpret_cast<void *>(customer));
    if (rc != 0) {
        errno = rc;
        perror("creating pthread");
        exit(EXIT_FAILURE);
    }

    // Wait for random delay, then create new Customer thread.
    // unsigned int sleep_seconds = 1 + (random() % 2);
    usleep(customer_arrival_time());
  }

  cout << "the shop closes" << endl;

  //let all barbers know the shop is closing
  for(auto barber : barbers)
  {
    barber->closing_time();
  }

  #if defined (VERBOSE)
  {
    Lock lock(mutex);
    cout << "(" << customer_queue.size() << ") customers are still waiting" << endl;
  }
  #endif

  for (auto thread: barber_threads) {
    pthread_join(*thread, nullptr);
  }

  cout << endl << endl;
  cout << "customers served immediately: " << customers_served_immediately << endl;
  cout << "customers waited " << customers_waited << endl;
  cout << "total customers served " << (customers_served_immediately + customers_waited) << endl;
  cout << "customers turned away: " << customers_turned_away << endl;
  cout << "total customers: " << customers_total << endl;
}

// Customer thread announces arrival to shop. If the collection of
// currently sleeping barbers is not empty, remove and return one
// barber from the collection. If all the barbers are busy and there
// is an empty chair in the waiting room, add the customer to the
// waiting queue and return {nullptr, true}.  Otherwise, the
// customer will leave: return {nullptr, false}.
Shop::BarberOrWait Shop::arrives(Customer* customer)
{
    customers_total++;

    {
        Lock lock(mutex);
        // Find a sleeping barber.
        if(barber_queue.size() > 0)
        {
            Barber* nextBarber = barber_queue.front();
            barber_queue.pop();

            #if defined(VERBOSE)
            cout << "barber(" << nextBarber->id << ") is available" << endl;
            #endif

            customers_served_immediately++;
            return BarberOrWait{nextBarber, false};
        }else if(customer_queue.size() < waiting_chairs)
        {
            // No barber: check for a waiting-area chair.
            customers_waited++;
            return BarberOrWait{nullptr, true};
        }

        // Otherwise, customer leaves.
        customers_turned_away++;
        return BarberOrWait{nullptr, false};
    }
}

// Barber thread requests next customer.  If no customers are
// currently waiting, add the barber to the collection of
// currently sleeping barbers and return nullptr.
Customer* Shop::next_customer(Barber* barber) {
  {
    Lock lock(mutex);
    if(customer_queue.size() == 0){//no customer on a waiting list
        return nullptr;
    }else{
        #if defined(VERBOSE)
          cout << "barber(" << barber->id << ") peeks in and ("<< customer_queue.size() << ") customers waiting" << endl;
        #endif
        Customer* nextCustomer = customer_queue.front();
        customer_queue.pop();
        return nextCustomer;
    }
  }
    return nullptr;
}

int Shop::service_time(){

    std::random_device rd;
    default_random_engine generator(rd());
    normal_distribution<double> distribution(average_service_time_ms, service_time_deviation_ms);
    double service_time = distribution(generator);
    double ref_time = average_service_time_ms-(service_time_deviation_ms*2) < 0 ? average_service_time_ms : average_service_time_ms-(service_time_deviation_ms*2);
    if(service_time < ref_time)
        service_time = average_service_time_ms - (service_time_deviation_ms*2);

    return (int)service_time * 1000;

}

unsigned int Shop::customer_arrival_time(){
    unsigned int seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine customer_arrival_time (seed);
    poisson_distribution<int> distribution(average_customer_arrival_ms);
    unsigned int arrival_time = distribution(customer_arrival_time);
    return arrival_time * 1000;
}

void Shop::waitInWaitingRoom(Customer* customer){
  cout << "customer[" << customer->id << "] takes a seat in the waiting room" << endl;
  {
    Lock lock(mutex);
    #if defined(VERBOSE)
      cout << "(" << waiting_chairs - customer_queue.size() << ") seats remaining in a waiting room" << endl;
    #endif
    customer_queue.push(customer);
  }
}

void Shop::barberSleepInSeat(Barber* barber){
  {
    Lock lock(mutex);
    cout << "barber(" << barber->id << ") takes a nap" << endl;
    barber_queue.push(barber);
  }
  barber->sleep();
}






Barber::Barber(Shop* shop, int id):id(id),shop(shop),state(barber_init)
{
    mutex = reinterpret_cast<pthread_mutex_t*>(malloc(sizeof(pthread_mutex_t)));//allocate memory for mutex
    int rc = pthread_mutex_init(mutex, NULL);
    if (rc != 0) {
      errno = rc;
      perror("customer initializing mutex");
      exit(EXIT_FAILURE);
    }

    //initialize condition variable
    cond = reinterpret_cast<pthread_cond_t*>(malloc(sizeof(pthread_cond_t)));
    rc = pthread_cond_init(cond, NULL);
    if (rc != 0) {
      errno = rc;
      perror("initializing condition variable");
      exit(EXIT_FAILURE);
    }
}

void Barber::run() 
{
  cout << "barber " << id << " arrives for work" << endl; 
  while(true)
  {
    //if there is no customer in a waiting room, this thread waits until woken up by a customer
    Customer* customer = shop->next_customer(this);
    if(customer == nullptr)//no customer is on a waiting list
    {
      cout << barber goes to sleep ;
      {
        lock;
        while (shop not closed and not my_customer) {
          wait();
        }
        if !customer {
          break;
        }
        customer = my_customer
      }
    } else {
      customer-> next_customer(this);
    }

    // wait for customer to sit
    {
      lock;
      while !customer_sat {
        wait;
      }
    }

    // usleep for duration of haircut

    customer->finished();

    // wait for payement

    customer->payment_accepted();



    bo
  }
}

void Barber::closing_time() 
{
  shopClosed = true;
}

// Customer wakes up barber.
void Barber::awaken(Customer* customer){
  myCustomer = customer;
  cout << "customer[" << myCustomer->id << "] wakes barber(" << id << ")" << endl;
  myCustomer = customer;
  wakeup();
}

void Barber::customer_sits() 
{
  // print statement in Barber::run
  //cout << "customer[" << myCustomer->id << "] sits in barber(" << id << ")'s chair" << endl;
  lock;
  customer_sat = true  or state == CUST_SAT
  signal;
}

void Barber::payment() 
{
  cout << "customer[" << myCustomer->id << "] gets up and proffers payment to barber(" << id << ")" << endl;  
}

//sleeps while waiting for barber to finish
void Barber::sleep(){
  {
    Lock lock(mutex);
    int rc = pthread_cond_wait(cond,mutex);//barber goes to sleep
    if (rc != 0){
      perror("barber wait");
      exit(EXIT_FAILURE);
    }
  }
}

void Barber::wakeup(){
  int rc = pthread_cond_signal(cond);
  if (rc != 0){
    perror("barber wakeup");
    exit(EXIT_FAILURE);
  }
}



Customer::Customer(Shop* shop, int id):id(id),shop(shop),state(cust_init)
{
    mutex = reinterpret_cast<pthread_mutex_t*>(malloc(sizeof(pthread_mutex_t)));//allocate memory for mutex
    int rc = pthread_mutex_init(mutex, NULL);
    if (rc != 0) {
      errno = rc;
      perror("customer initializing mutex");
      exit(EXIT_FAILURE);
    }

    //initialize condition variable
    cond = reinterpret_cast<pthread_cond_t*>(malloc(sizeof(pthread_cond_t)));
    rc = pthread_cond_init(cond, NULL);
    if (rc != 0) {
      errno = rc;
      perror("customer initializing condition variable");
      exit(EXIT_FAILURE);
    }
}

Customer::~Customer() 
{

}

void Customer::run() 
{
  cout << "customer[" << id << "] arrives" << endl;
  Shop::BarberOrWait bw = shop->arrives(this);

  if(bw.barber != nullptr || bw.chair_available)//barber is available
  {
    if(bw.chair_available)
    {
      #if defined(VERBOSE)
      cout << "Customer " << id << " is going to wait since all barbers are busy now " << endl;
      #endif
      state = waiting;
      shop->waitInWaitingRoom(this);
      sleep();
    }else{
      #if defined(VERBOSE)
      cout << "Customer " << id << " is assigned to a barber " << bw.barber->id << endl;
      #endif
      next_customer(bw.barber);
    }

    bool isGettingHaircut = true;
    while(isGettingHaircut)
    {
      switch(state) {
        case cust_init:
          myBarber->awaken(this);//wake up a barber
          state = ready_to_sit;
          break;
        case waiting:
          state = ready_to_sit;
          myBarber->wakeup();
          #if defined(VERBOSE)
          cout << "Customer " << id << " is waking up barber " << myBarber->id << " from waiting state" << endl;
          #endif
          break;
        case ready_to_sit:
          myBarber->customer_sits();
          state = receiving_haircut;
          myBarber->wakeup();
          break; 
        case receiving_haircut:
          myBarber->payment();
          state = finished_haircut;
          myBarber->wakeup();
          break;  
        case finished_haircut:
          state = paying;
          myBarber->wakeup();
          break;
        case paying:
          myBarber->wakeup();
          isGettingHaircut = false;
          break; 
        default:
          cout << "Invalid Selection\n";
          break;
      }

      if(isGettingHaircut)
        sleep();
    }
  }else{
    cout << "customer[" << id << "] leaves without getting haircut" << endl;

    #if defined(VERBOSE)
      cout << "All barbers are BUSY, and NO chair in a waiting room so Customer [" << id << "] is leaving." << endl;
    #endif

    return;
  }
}

// Barber calls this customer after checking the waiting-room
// queue (customer should be waitint).
void Customer::next_customer(Barber* barber){
  myBarber = barber;
}

void Customer::finished() 
{
  cout << "barber(" << myBarber->id << ") finishes customer[" << id << "]'s haircut" << endl;
}

// Barber has accepted payment for service.
void Customer::payment_accepted(){
    cout << "barber(" << myBarber->id << ") accepts payment from customer[" << id << "]" <<  endl;
}


//sleeps while waiting for barber to finish
void Customer::sleep(){
  {
    Lock lock(mutex);
    int rc = pthread_cond_wait(cond,mutex);//barber goes to sleep
    if (rc != 0){
      perror("customer wait");
      exit(EXIT_FAILURE);
    }
  }
}

//wakes up
void Customer::wakeup(){
    int rc = pthread_cond_signal(cond);
    if (rc != 0){
      perror("customer wakeup");
      exit(EXIT_FAILURE);
    }
}

void usage() {
  cerr
    << "usage: "
    << PROG_NAME
    << " <nbarbers>"
    << " <nchairs>"
    << " <avg_service_time>"
    << " <service_time_std_deviation>"
    << " <avg_customer_arrival_time>"
    << " <duration>"
    << endl;
  exit(EXIT_FAILURE);
}


int main(int argc, char* argv[]) {
  PROG_NAME = argv[0];

  if (argc != 7) {
    usage();
  }
  int barbers = atoi(argv[1]);
  if (barbers <= 0) {
    usage();
  }
  int chairs = atoi(argv[2]);
  if (chairs < 0) {
    usage();
  }
  int service_time = atoi(argv[3]);
  if (service_time <= 0) {
    usage();
  }
  int service_deviation = atoi(argv[4]);
  if (service_time <= 0) {
    usage();
  }
  int customer_arrivals = atoi(argv[5]);
  if (customer_arrivals <= 0) {
    usage();
  }
  int duration = atoi(argv[6]);
  if (duration <= 0) {
    usage();
  }

  Shop barber_shop(barbers,
		   chairs,
		   service_time,
		   service_deviation,
		   customer_arrivals,
		   duration);

  barber_shop.run();

  return EXIT_SUCCESS;
}