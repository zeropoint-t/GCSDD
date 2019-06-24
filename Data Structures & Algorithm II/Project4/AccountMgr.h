// ----------------------------------------AccountMgr.h-------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/07/2019
// -----------------------------------------------------------------------------
// AccountMgr - header file - represent an account
// -----------------------------------------------------------------------------
// Class stores information about customer loaded from data4commands.txt
// -----------------------------------------------------------------------------
#ifndef AccuntMgr_h
#define AccuntMgr_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

#include "Account.h"
#include "HashTable.h"

using namespace std;

class AccountMgr {
private:
	vector<int>* accountList;
    HashTable<int, Account*>* accounts; // Account and int customerID
	void buildAccounts(string);

public:
	//constructors & destructor
	AccountMgr(string);
	AccountMgr();
	~AccountMgr();

	// Account& loadAccount(int AcctId, string LastName, string FirstName);//load an account
	Account* getAccount(int accountId);//returns an account object for the accountid
};

#endif
