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

enum customer_state {waiting, ready_to_sit, getting_haircut, paying, leaving_satisfied};
enum barber_state {sleeping, customer_sitting, cutting_hair, getting_payment};

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


    #if defined (VERBOSE)
    {
      Lock lock(mutex);
      cout << "(" << customer_queue.size() << ") customers are still waiting" << endl;
    }
    #endif

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
    cout << endl;
    cout << "customer[" << customer->id << "] arrives" << endl; 
    customers_total++;

    {
        Lock lock(mutex);
        // Find a sleeping barber.
        if(barber_queue.size() > 0)
        {
            Barber* nextBarber = barber_queue.front();
            barber_queue.pop();
            customers_served_immediately++;
            return BarberOrWait{nextBarber, false};
        }else if(customer_queue.size() < waiting_chairs)
        {
            // No barber: check for a waiting-area chair.
            customer_queue.push(customer);
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
        barber_queue.push(barber);
        return nullptr;
    }else{
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

// void Shop::waitInWaitingRoom(Customer* customer){
//   cout << "customer[" << customer->id << "] takes a seat in the waiting room" << endl;
//   {
//     Lock lock(mutex);
//     customer_queue.push(customer);
//   }
// }

// void Shop::barberSleepInSeat(Barber* barber){
//   {
//     Lock lock(mutex);
//     cout << "barber(" << barber->id << ") takes a nap" << endl;
//     barber_queue.push(barber);
//   }

//   barber->sleep();
// }






Barber::Barber(Shop* shop, int id):id(id),shop(shop)
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
    cout << "barber(" << id << ") arrives for work" << endl; 
    while(true)
    {
        //if there is no customer in a waiting room, this thread waits until woken up by a customer
        myCustomer = shop->next_customer(this);
        if(myCustomer == nullptr)
        {
            if(shopClosed)
              break;

            cout << "barber(" << id << ") takes a nap" << endl;
            //wait until a customer wakes him up or shop closes
            {
                Lock lock(mutex);
                while(!shopClosed && myCustomer == nullptr)
                {
                    state = sleeping;
                    int rc = pthread_cond_wait(cond, mutex);
                    if (rc != 0){
                        perror("barber awaken");
                        exit(EXIT_FAILURE);
                    }
                }
            }

            //woken up and still customer is null which means shop is closed
            if(myCustomer == nullptr)
              break;

            cout << "barber(" << id << ") wakes up" << endl;    
        }else{
            cout << "barber(" << id << ") calls customer[" << myCustomer->id << "]" << endl;    
        }
      
        //either a customer woke me up or customer was waiting in a waiting room
        state = customer_sitting;
        myCustomer->next_customer(this);

        // wait for customer to sit
        {
            Lock lock(mutex);
            while(state == customer_sitting) {
                int rc = pthread_cond_wait(cond, mutex);
                if (rc != 0){
                    perror("barber customer_sitting");
                    exit(EXIT_FAILURE);
                }
            }
        }
        
        cout << "barber(" << id << ") gives customer[" << myCustomer->id << "] a haircut" << endl;

        usleep(shop->service_time());

        cout << "barber(" << id << ") finishes customer[" << myCustomer->id << "]'s haircut" << endl;
        
        myCustomer->finished();

        // wait for customer to pay
        {
            Lock lock(mutex);
            while(state == cutting_hair) {
                int rc = pthread_cond_wait(cond, mutex);
                if (rc != 0){
                    perror("barber cutting_hair");
                    exit(EXIT_FAILURE);
                }
            }
        }

        cout << "barber(" << id << ") accepts payment from customer[" << myCustomer->id << "]" <<  endl;
        
        myCustomer->payment_accepted();
    }

    cout << "barber(" << id << ") leaves for home" << endl;
}

void Barber::closing_time() 
{
    {
        Lock lock(mutex);
        shopClosed = true;
        pthread_cond_signal(cond);
    }
}

// Customer wakes up barber.
void Barber::awaken(Customer* customer){
    {
        Lock lock(mutex);
        myCustomer = customer;
        state = customer_sitting;
        pthread_cond_signal(cond);
    }
}

void Barber::customer_sits() 
{
    {
        Lock lock(mutex);
        state = cutting_hair;
        pthread_cond_signal(cond);
    }
}

void Barber::payment() 
{
    {
        Lock lock(mutex);
        state = getting_payment;
        pthread_cond_signal(cond);
    }
}

Customer::Customer(Shop* shop, int id):id(id), shop(shop)
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
    Shop::BarberOrWait bw = shop->arrives(this);
    if(bw.barber != nullptr)//barber is available & sleeping
    {
        myBarber = bw.barber;
        state = ready_to_sit;

        cout << "customer[" << id << "] wakes barber(" << myBarber->id << ")" << endl;
        
        myBarber->awaken(this);//wake up a barber

        //wait until a barber lets me sit in the chair
        {
            Lock lock(mutex);
            while(state == ready_to_sit){//wait while getting a haircut
                int rc = pthread_cond_wait(cond, mutex);
                if (rc != 0){
                    perror("customer getting_haircut");
                    exit(EXIT_FAILURE);
                }
            }
        }

        cout << "customer[" << id << "] sits in barber[" << myBarber->id << "]'s chair" << endl;  

        myBarber->customer_sits();

        //wait until hair cut is done
        {
            Lock lock(mutex);
            while(state == getting_haircut){//wait while getting a haircut
                int rc = pthread_cond_wait(cond, mutex);
                if (rc != 0){
                    perror("customer getting_haircut");
                    exit(EXIT_FAILURE);
                }
            }
        }

        cout << "customer[" << id << "] gets up and proffers payment to barber(" << myBarber->id << ")" << endl;  
        
        myBarber->payment();

        //wait until payment is done
        {
            Lock lock(mutex);
            while(state == paying){//wait while getting a haircut
                int rc = pthread_cond_wait(cond, mutex);
                if (rc != 0){
                    perror("customer getting_haircut");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }else if(bw.chair_available)//barber is not available but there is a chair
    {
        cout << "customer[" << id << "] takes a seat in the waiting room" << endl;
        
        //set state to waiting and wait
        state = waiting;

        {
          Lock lock(mutex);
          while(state == waiting){
              int rc = pthread_cond_wait(cond, mutex);
              if (rc != 0){
                  perror("customer waiting");
                  exit(EXIT_FAILURE);
              }
          }
        }

        cout << "customer[" << id << "] sits in barber(" << myBarber->id << ")'s chair" << endl;
        
        myBarber->customer_sits();

        //wait until hair cut is done
        {
            Lock lock(mutex);
            while(state == getting_haircut){//wait while getting a haircut
                int rc = pthread_cond_wait(cond, mutex);
                if (rc != 0){
                    perror("customer getting_haircut");
                    exit(EXIT_FAILURE);
                }
            }
        }

        cout << "customer[" << id << "] gets up and proffers payment to barber(" << myBarber->id << ")" << endl;  
        
        myBarber->payment();

        //wait until payment is done
        {
            Lock lock(mutex);
            while(state == paying){//wait while getting a haircut
                int rc = pthread_cond_wait(cond, mutex);
                if (rc != 0){
                    perror("customer getting_haircut");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    else{
        cout << "customer[" << id << "] leaves without getting haircut" << endl;
        return;
    }

    cout << "customer[" << id << "] leaves satisfied" << endl;
}

// Barber calls this customer after checking the waiting-room
// queue (customer should be waitint).
void Customer::next_customer(Barber* barber){
    {   
        Lock lock(mutex);
        myBarber = barber;
        state = getting_haircut;
        pthread_cond_signal(cond);
    }
}

// Barber finishes haircut (Customer should be receiving haircut).
void Customer::finished() 
{    
    {
        Lock lock(mutex);
        state = paying;
        pthread_cond_signal(cond);
    }
}

// Barber has accepted payment for service.
void Customer::payment_accepted(){
    {
        Lock lock(mutex);
        state = leaving_satisfied;
        pthread_cond_signal(cond);
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


int main(int argc, char* argv[]){//int argc, char* argv[]) {
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