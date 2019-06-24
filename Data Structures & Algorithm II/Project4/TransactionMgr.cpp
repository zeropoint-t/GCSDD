// ----------------------------------------TransactionMgr.cpp-------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/03/2019
// -----------------------------------------------------------------------------
// TransactionMgr -Implementation -in charge of processing commands/transactions
//                  (borrow, return, show history, show inventory)
// -----------------------------------------------------------------------------
// Notes on specifications, special algorithms, and assumptions.
// -----------------------------------------------------------------------------

#include "TransactionMgr.h"

//constructor
TransactionMgr::TransactionMgr(InventoryMgr* inv, AccountMgr* aMgr)
:invMgr(inv), acctMgr(aMgr)
{
    accountSet = new set<int>();
    transactions = new HashTable<int, Transaction*>();
}

//constructor
TransactionMgr::TransactionMgr()
{

}

//destructor
TransactionMgr::~TransactionMgr()
{
    	//delete all accounts
	for(auto it = accountSet->begin(); it != accountSet->end(); it++)
	{
        HashNode<int, Transaction*>* cur = transactions->get(*it);//find head node
        HashNode<int, Transaction*>* prev = nullptr;
        //print each transaction from the newest to the oldest
        while(cur != nullptr && cur->getKey() == *it)
        {
            prev = cur;
            cur = cur->getNext();
            Transaction* pTrans = prev->getValue();
            if(pTrans != nullptr)
                delete pTrans;
        }
	}
	delete accountSet;
    delete transactions;
}

// --------------------void ::buildTransactions(const string infile)-------------
// Description: reads transaction commands from a file and processes them
// ------------------------------------------------------------------------------
void TransactionMgr::buildTransactions(const string infile)
{
	ifstream file(infile);
	if (!file) 
	{
		cout << "File could not be opened." << endl;
		return;
	}

    while(!file.eof())
    {
        string s;
        getline(file, s);

        istringstream iss(s);
        char command;

        //read word by word
        bool firstIter = true;
        while(iss >> command) {
            if(firstIter)
            {
                firstIter = false;
                if(command == 'I'){//show inventory
                    invMgr->printInv();
                }else if(command == 'H'){//shwo transaction history
                    string acctId;
                    iss >> acctId;
                    int accountId = stoi(acctId);
                    printAccountHistory(accountId);
                }else if(command == 'B'){//borrow
                    string acctId;
                    iss >> acctId;
                    int accountId = stoi(acctId);

                    //check account is valid
                    if(acctMgr != nullptr)
                    {
                        Account* pAcct = acctMgr->getAccount(accountId);
                        if(pAcct == nullptr)//check if this account exists
                        {
                            cout << "ERROR: AccountId " << accountId << " not found" << endl;
                            break;
                        }

                        char storageType;
                        iss >> storageType;

                        //check storage type
                        if(storageType == 'D'){//DVD
                            char filmType;
                            iss >> filmType;
                            if(filmType == 'F')//funny
                            {
                                //find a title
                                string title;
                                getline(iss, title, ',');//read until delimiter
                                title = trimLeadingAndTrailingSpaces(title);

                                //find a release year
                                string year;
                                iss >> year;
                                
                                //create a stub Comedy object
                                Comedy c('M', storageType, 0, filmType, "", title, stoi(year));
                                Media* pMed = invMgr->getMedia(c);
                                if(pMed != nullptr)
                                {
                                    if(pMed->getNumStock() > 0)
                                    {
                                        borrowMedia(pMed, pAcct, command);
                                    }else{
                                        cout << "ERROR: Comedy Movie: " 
                                        << title 
                                        << " in " 
                                        << year
                                        << " out of stock" << endl;
                                    }
                                }else
                                {
                                    cout << "ERROR: Comedy Movie: " 
                                    << title 
                                    << " in " 
                                    << year
                                    << " not found" << endl;
                                }                               
                            }else if(filmType == 'C')//classic
                            {
                                string releaseMonth;
                                iss >> releaseMonth;

                                string releaseYear;
                                iss >> releaseYear;

                                string majorActorFirstName;
                                iss >> majorActorFirstName;

                                string majorActorLastName;
                                iss >> majorActorLastName;  

                                //create a stub object
                                Classic c('M', storageType, 0, filmType, "", ""
                                , stoi(releaseYear), majorActorFirstName, majorActorLastName, stoi(releaseMonth));
                                Media* pMed = invMgr->getMedia(c);
                                if(pMed != nullptr)
                                {
                                    if(pMed->getNumStock() > 0)
                                    {
                                        borrowMedia(pMed, pAcct, command);
                                    }else{
                                        cout << "ERROR: Classic Movie: " 
                                        << majorActorFirstName 
                                        << " " 
                                        << majorActorLastName 
                                        << " " 
                                        << releaseYear 
                                        << " " 
                                        << releaseMonth 
                                        << " out of stock" << endl;
                                    }
                                }else
                                {
                                    cout << "ERROR: Classic Movie: " 
                                    << majorActorFirstName 
                                    << " " 
                                    << majorActorLastName 
                                    << " " 
                                    << releaseYear 
                                    << " " 
                                    << releaseMonth 
                                    << " not found" << endl;
                                }  
                            }else if(filmType == 'D')//drama
                            {
                                string director;
                                getline(iss, director, ',');//read until delimiter
                                director = trimLeadingAndTrailingSpaces(director);

                                string title;
                                getline(iss, title, ',');//read until delimiter
                                title = trimLeadingAndTrailingSpaces(title);

                                //create a stub object
                                Drama d('M', storageType, 0, filmType, director, title, 0);
                                Media* pMed = invMgr->getMedia(d);
                                if(pMed != nullptr)
                                {
                                    if(pMed->getNumStock() > 0)
                                    {
                                        borrowMedia(pMed, pAcct, command);
                                    }else{
                                        cout << "ERROR: Drama Movie: " 
                                        << title
                                        << " by " 
                                        << director
                                        << " out of stock" << endl;
                                    }
                                }else
                                {
                                    cout << "ERROR: Drama Movie: " 
                                    << title
                                    << " by " 
                                    << director
                                    << " not found" << endl;
                                }  
                            }else
                            {
                                cout << "ERROR: Genre " << filmType << " not found" << endl;
                            }
                        }
                    }
                }else if(command == 'R'){
                    string acctId;
                    iss >> acctId;
                    int accountId = stoi(acctId);

                    //check account is valid
                    if(acctMgr != nullptr)
                    {
                        Account* pAcct = acctMgr->getAccount(accountId);
                        if(pAcct == nullptr)//check if this account exists
                        {
                            cout << "ERROR: AccountId " << accountId << " not found" << endl;
                            break;
                        }

                        char storageType;
                        iss >> storageType;

                        //check storage type
                        if(storageType == 'D'){//DVD
                            char filmType;
                            iss >> filmType;
                            if(filmType == 'F')//funny
                            {
                                //find a title
                                string title;
                                getline(iss, title, ',');//read until delimiter
                                title = trimLeadingAndTrailingSpaces(title);

                                //find a release year
                                string year;
                                iss >> year;
                                
                                //create a stub Comedy object
                                Comedy c('M', storageType, 0, filmType, "", title, stoi(year));
                                Media* pMed = invMgr->getMedia(c);
                                if(pMed != nullptr)
                                {
                                    returnMedia(pMed,pAcct,command);
                                }       
                            }else if(filmType == 'C')//classic
                            {
                                string releaseMonth;
                                iss >> releaseMonth;

                                string releaseYear;
                                iss >> releaseYear;

                                string majorActorFirstName;
                                iss >> majorActorFirstName;

                                string majorActorLastName;
                                iss >> majorActorLastName;  

                                //create a stub object
                                Classic c('M', storageType, 0, filmType, "", ""
                                , stoi(releaseYear), majorActorFirstName, majorActorLastName, stoi(releaseMonth));
                                Media* pMed = invMgr->getMedia(c);
                                if(pMed != nullptr)
                                {
                                    returnMedia(pMed,pAcct,command);
                                } 
                            }else if(filmType == 'D')//drama
                            {
                                string director;
                                getline(iss, director, ',');//read until delimiter
                                director = trimLeadingAndTrailingSpaces(director);

                                string title;
                                getline(iss, title, ',');//read until delimiter
                                title = trimLeadingAndTrailingSpaces(title);

                                //create a stub object
                                Drama d('M', storageType, 0, filmType, director, title, 0);
                                Media* pMed = invMgr->getMedia(d);
                                if(pMed != nullptr)
                                {
                                    returnMedia(pMed,pAcct,command);
                                } 
                            }else
                            {
                                cout << "ERROR: Genre " << filmType << " not found" << endl;
                            }
                        }
                    }
                }else{
                    cout << "ERROR: Command " << command << " is not valid" << endl;
                }
                break;
            }
        }
    }
}

// --------------------bool borrowMedia(Media* med, Account* acct, const char actionType)-------------
// Description: create a transaction object for borrow 
// ---------------------------------------------------------------------------------------------------
bool TransactionMgr::borrowMedia(Media* med, Account* acct, const char actionType)
{
    if(med->getNumStock() > 0)
    {
        if(invMgr->decInv(*med))
        {
            Transaction* trans = new Transaction(acct, med, actionType);
            accountSet->insert(acct->getAccountId());
            transactions->addFront(acct->getAccountId(), trans);
        }
    }else
    {
        return false;//no stock
    }
    return true;
}

// --------------------bool returnMedia(Media* med, Account* acct, const char actionType)-------------
// Description: create a transaction object for Return 
// ---------------------------------------------------------------------------------------------------
bool TransactionMgr::returnMedia(Media* med, Account* acct, const char actionType)
{
    if(med->getNumStock() < med->getMaxStock())
    {
        Transaction* trans = new Transaction(acct, med, actionType);
        accountSet->insert(acct->getAccountId());
        transactions->addFront(acct->getAccountId(), trans);
        invMgr->incInv(*med);
    }else{
        cout << "Stock will be higher than max stock" << endl;
        return false;
    }

    return true;
}

// -----------void TransactionMgr::printAccountHistory(const int acctId)--------
// Description: Print transaction history for an account
// -----------------------------------------------------------------------------
void TransactionMgr::printAccountHistory(const int acctId)
{
    Account* pAcct = acctMgr->getAccount(acctId);
    if(pAcct != nullptr)
    {
        cout << left << endl;

        //print customer header
        cout << "------------------------AccountId: " << pAcct->getAccountId() << " ";
        cout << pAcct->getFirstName() << " ";
        cout << pAcct->getLastName() << "------------------------" << endl;

        HashNode<int, Transaction*>* node = transactions->get(acctId);//find head node
        //print each transaction from the newest to the oldest
        while(node != nullptr && node->getKey() == acctId)
        {
            if(node->getValue()->getMedia()->getMediaType() == 'F')//film
            {
                Film* film = dynamic_cast<Film*>(node->getValue()->getMedia());
                if(film->getFilmType() == 'F')//funny
                {
                    Comedy* pComedy = dynamic_cast<Comedy*>(film);
                    if(pComedy != nullptr)
                    {
                        if(pComedy->getStorageType() == 'D')//DVD
                            cout << left << setw(4) << "DVD" ;
                        
                        string action = "";
                        if(node->getValue()->getActionType() == 'B')//borrow
                        {
                            action = "Borrow";
                        }
                        else if(node->getValue()->getActionType() == 'R')//return
                        {
                            action = "Return";
                        }

                        cout << setw(7) << action;
                        string title = film->getTitle();
                        if(title.length() > 23) title.resize(23);
                        cout << setw(25) << title ;
                        cout << setw(18) << pComedy->getDirector();
                        cout << setw(6) << film->getYear();
                    }
                }else if(film->getFilmType() == 'C')
                {
                    Classic* pClassic = dynamic_cast<Classic*>(film);
                    if(pClassic != nullptr)
                    {
                        if(pClassic->getStorageType() == 'D')//DVD
                            cout << left << setw(4) << "DVD" ;
                        
                        string action = "";
                        if(node->getValue()->getActionType() == 'B')//borrow
                        {
                            action = "Borrow";
                        }
                        else if(node->getValue()->getActionType() == 'R')//return
                        {
                            action = "Return";
                        }

                        cout << setw(7) << action;
                        string title = film->getTitle();
                        if(title.length() > 23) title.resize(23);
                        cout << setw(25) << title;
                        cout << setw(18) << pClassic->getDirector();
                        cout << setw(6) << film->getYear();
                        cout << setw(3) << pClassic->getMonth();
                        cout << setw(1) << pClassic->getMainActorFirst() << " " << pClassic->getMainActorLast();
                    }
                }else if(film->getFilmType() == 'D')
                {
                    Drama* pDrama = dynamic_cast<Drama*>(film);
                    if(pDrama != nullptr)
                    {
                        if(pDrama->getStorageType() == 'D')//DVD
                            cout << left << setw(4) << "DVD" ;
                        
                        string action = "";
                        if(node->getValue()->getActionType() == 'B')//borrow
                        {
                            action = "Borrow";
                        }
                        else if(node->getValue()->getActionType() == 'R')//return
                        {
                            action = "Return";
                        }

                        cout << setw(7) << action;
                        string title = film->getTitle();
                        if(title.length() > 23) title.resize(23);
                        cout << setw(25) << title;
                        cout << setw(18) << pDrama->getDirector();
                        cout << setw(6) << film->getYear();
                    }
                }
                cout << endl;
            }
            node = node->getNext();
        }
    }
}

// -----------string trimLeadingAndTrailingSpaces(string str)--------
// Description: helper method to trim leading and trailing spaces
// ------------------------------------------------------------------
string TransactionMgr::trimLeadingAndTrailingSpaces(string str)
{
    //remove leading spaces
    string newString1;
    string::iterator it;
    bool charStarted = false;
    for(it = str.begin(); it != str.end(); it++) 
    {
        if(*it != ' ')
            charStarted = true;

        if(charStarted)
            newString1.insert(newString1.end(), *it);
    }

    //remove trailing spaces
    string newString2;
    string::reverse_iterator rit;
    charStarted = false;
    for(rit = newString1.rbegin(); rit != newString1.rend(); rit++) 
    {
        if(*rit != ' ')
            charStarted = true;

        if(charStarted)
            newString2.insert(newString2.begin(), *rit);
    }

    return newString2;
}
