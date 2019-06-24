#ifndef ExpenseVisitor_h
#define ExpenseVisitor_h

#include "PersonalExpense.h"

class PersonalExpense;
class ExpenseElement;

struct ExpenseVisitor{
    virtual void Visit(ExpenseElement& exp) const = 0;
    virtual ~ExpenseVisitor(){};
};

struct CalcTotalVisitor: public ExpenseVisitor{
    CalcTotalVisitor();
    ~CalcTotalVisitor();
    void VisitExpenses(PersonalExpense& p_exp) const;
    void Visit(ExpenseElement& exp) const override;
};

struct CalcAvgVisitor: public ExpenseVisitor{
    CalcAvgVisitor();
    ~CalcAvgVisitor();
    void VisitExpenses(PersonalExpense& p_exp) const;
    void Visit(ExpenseElement& exp) const override;
};

#endif
