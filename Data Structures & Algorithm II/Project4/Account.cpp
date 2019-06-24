// ----------------------------------------Account.cpp--------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/07/2019
// -----------------------------------------------------------------------------
// Account - implementation file - represent an account
// -----------------------------------------------------------------------------
// Class stores information about customer loaded from data4commands.txt
// -----------------------------------------------------------------------------

//demo for dan hopefully this works mopre changes
#include "Account.h"

ostream& operator<<(ostream &outStream, const Account &acct)
{
	outStream << acct.getFirstName() << " " << acct.getLastName() << endl;
	return outStream;
}

Account::Account(int nAccountId, string nFirstName, string nLastName)
{
    setAccountId(nAccountId);
    setFirstName(nFirstName);
    setLastName(nLastName);
}

Account::Account(){}

Account::~Account()
{
    // cout << "Destroy Account: " << getFirstName() << " " << getLastName() << endl;
}

//getters
int Account::getAccountId() const
{
    return accountId;
}
string Account::getFirstName() const
{
    return firstName;
}
string Account::getLastName() const
{
    return lastName;
}

//setters
void Account::setFirstName(string nFirstName)
{
    firstName = nFirstName;
}
void Account::setLastName(string nLastName)
{
    lastName = nLastName;
}
void Account::setAccountId(int nAccountId)
{
    accountId = nAccountId;
}
