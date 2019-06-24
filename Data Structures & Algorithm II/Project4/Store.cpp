// ------------------------------------------Store.cpp----------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/03/2019
// -----------------------------------------------------------------------------
// Store - represents a media rental store
// -----------------------------------------------------------------------------
// Notes on specifications, special algorithms, and assumptions.
// -----------------------------------------------------------------------------

#include "Store.h"

//constructors & destructor
Store::Store():invMgr(nullptr), acctMgr(nullptr), transMgr(nullptr)
{
}

Store::~Store()
{
    delete transMgr;
    delete acctMgr;
    delete invMgr;
}

// -----------void :buildInventory(string InventoryInputFile)-------------------------
// Description: interface method for building inventory from a file and delegates the task to InventoryManager
// -----------------------------------------------------------------------------------
void Store::buildInventory(string InventoryInputFile)
{
    invMgr = new InventoryMgr(InventoryInputFile);
}

// -----------void Store::buildCustomerList(string AccountsInputFile)-----------------
// Description: interface method for building customers/accounts and delegates the task to AccountManager
// -----------------------------------------------------------------------------------
void Store::buildCustomerList(string AccountsInputFile)
{
    acctMgr = new AccountMgr(AccountsInputFile);
}

// -----------void Store::processTransactions(string TransactionsInputFile)-----------
// Description: interface method for processing transactions and delegates the task to TransactionManager
// -----------------------------------------------------------------------------------
void Store::processTransactions(string TransactionsInputFile)
{
    transMgr = new TransactionMgr(invMgr, acctMgr);
    transMgr->buildTransactions(TransactionsInputFile);
}