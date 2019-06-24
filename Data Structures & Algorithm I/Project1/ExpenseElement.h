#ifndef ExpenseElement_h
#define ExpenseElement_h

#include <vector>
#include <string>
#include "ExpenseVisitor.h"
#include "Bill.h"

using namespace std;

class ExpenseVisitor;

class ExpenseElement{
    public:
        ExpenseElement(string expenseType);
        virtual ~ExpenseElement() = 0;
        virtual void Accept(const ExpenseVisitor& visitor) = 0;
        virtual void AddBill(Bill* bill) = 0;
        string getExpenseType();
        vector<Bill*>& getBills();
    private:
        string expenseType;
        vector<Bill*> bills;
};

class ElectricityExpense : public ExpenseElement{
    public:
        ElectricityExpense(string expenseType);
        ~ElectricityExpense() override;
        void Accept(const ExpenseVisitor& visitor) override;
        void AddBill(Bill* bill) override;
};

class CityExpense : public ExpenseElement{
    public:
        CityExpense(string expenseType);
        ~CityExpense() override;
        void Accept(const ExpenseVisitor& visitor) override;
        void AddBill(Bill* bill) override;
};

#endif
