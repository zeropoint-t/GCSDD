// ------------------------------------------Store.h----------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/03/2019
// -----------------------------------------------------------------------------
// Store - represents a media rental store - public interface
// -----------------------------------------------------------------------------
// Notes on specifications, special algorithms, and assumptions.
// -----------------------------------------------------------------------------

#ifndef Store_h
#define Store_h

#include<iostream>
#include<fstream>
#include "InventoryMgr.h"
#include "AccountMgr.h"
#include "TransactionMgr.h"
using namespace std;

class Store 
{
private:
	InventoryMgr* invMgr;
	AccountMgr* acctMgr;
	TransactionMgr* transMgr;
	vector<Store> storesVect;

public:
	//constructors & destructor
	Store();
	~Store();

	void buildInventory(string InventoryInputFile);//build
	void buildCustomerList(string AccountsInputFile);
	void processTransactions(string TransactionsInputFile);
};

#endif
