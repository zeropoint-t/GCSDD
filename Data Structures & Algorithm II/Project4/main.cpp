


// #include "DRIVER FOR ASSIGNMENT $.hpp"
// Driver:
// Input files
// *Build inventory
// *Build customer list
// *Transactions

#include <iostream>
#include <fstream>

//hello world

// #include "Media.h"
// #include "Comedy.h"
// #include "Account.h"
// #include "AccountMgr.h"
// #include "TransactionMgr.h"

// #include "Comedy.h"
// #include "Drama.h"
// #include "Film.h"
// #include "InventoryMgr.h"

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
