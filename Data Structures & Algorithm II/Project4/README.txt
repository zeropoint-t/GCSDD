Please use a command "g++ -std=c++11 *.cpp -o main" to compile and run the program with "./main"

The main.cpp should contain the below codes to execute the program successfully.

#include "Store.h"

using namespace std;

int main() {

    Store* store = new Store();
    store->buildCustomerList("data4customers.txt");
    store->buildInventory("data4movies.txt");
    store->processTransactions("data4commands.txt");

    cout << endl;
    cout << "GREAT JOB. PROJECT DONE!!!" << endl;

    delete store;
}
