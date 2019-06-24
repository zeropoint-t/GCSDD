#include <iostream>
#include "ExpenseVisitor.h"
#include "ExpenseElement.h"
#include "Bill.h"

using namespace std;

//-----------------------------
// CalcTotalVisitor
//-----------------------------
//Constructor
CalcTotalVisitor::CalcTotalVisitor(){};

//Destructor
CalcTotalVisitor::~CalcTotalVisitor(){};

void CalcTotalVisitor::Visit(ExpenseElement& exp) const{
    int cnt = 0;
    if(exp.getBills().begin() != exp.getBills().end()){
        Bill totalBill((*exp.getBills().begin())->getType());
        for(auto it = exp.getBills().begin(); it != exp.getBills().end(); ++it){
            totalBill += *(*it);
            ++cnt;
        }
        cout << exp.getExpenseType() << " - Total: " << totalBill.getAmount() << " for the last " << cnt << " months" << endl;
    }
}

void CalcTotalVisitor::VisitExpenses(PersonalExpense& p_exp) const{
    auto expenses = p_exp.GetExpenses();
    for(auto it = expenses.begin(); it != expenses.end(); ++it){
         (*it)->Accept(*this);
    }
}

//-----------------------------
// CalcAvgVisitor
//-----------------------------
//Constructor
CalcAvgVisitor::CalcAvgVisitor(){};

//Destructor
CalcAvgVisitor::~CalcAvgVisitor(){};

void CalcAvgVisitor::Visit(ExpenseElement& exp) const{
    int cnt = 0;
    if(exp.getBills().begin() != exp.getBills().end()){
        Bill totalBill((*exp.getBills().begin())->getType());
        for(auto it = exp.getBills().begin(); it != exp.getBills().end(); ++it){
            totalBill += *(*it);
            ++cnt;
        }
        cout << exp.getExpenseType() << " - Monthly Average: " << totalBill.getAmount() / cnt << " for the last " << cnt << " months" << endl;
    }
}

void CalcAvgVisitor::VisitExpenses(PersonalExpense& p_exp) const{
    auto expenses = p_exp.GetExpenses();
    for(auto it = expenses.begin(); it != expenses.end(); ++it){
        (*it)->Accept(*this);
    }
}
