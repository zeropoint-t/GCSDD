#ifndef PersonalExpense_h
#define PersonalExpense_h

#include <vector>
#include "ExpenseElement.h"

using namespace std;

struct ExpenseElement;

class PersonalExpense{
    public:
        PersonalExpense();
        ~PersonalExpense();
        void AddExpense(ExpenseElement& expense);
        vector<ExpenseElement*>& GetExpenses();
    private:
        vector<ExpenseElement*> expenses;
};

#endif
