// ----------------------------------Transaction.cpp----------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/07/2019
// -----------------------------------------------------------------------------
// Transaction implementation transaction item stored in vector in TransactionMgr
// contains info on which account, type of media, and type of transaction occured
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
#include "Transaction.h"
#include "Account.h"
#include "Media.h"

// constructor - passes parameters to media parent
Transaction::Transaction(Account* account, Media* media, char actionType)
    :account(account), media(media), actionType(actionType){}

Transaction::Transaction(){}

Transaction::~Transaction(){}

//getters
Account* Transaction::getAccount() const
{
    return account;
}
Media* Transaction::getMedia() const
{
    return media;
}

char Transaction::getActionType() const
{
    return actionType;
}

//setters
void Transaction::setAccount(Account* nAccount)
{
    account = nAccount;
}
void Transaction::setMedia(Media* nMedia)
{
    media = nMedia;
}
void Transaction::setActionType(char nActionType)
{
    actionType = nActionType;
}

