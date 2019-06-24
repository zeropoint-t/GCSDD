//
//  main.cpp
//  HW1
//
//  Created by Tetsuya Hayashi on 10/6/18.
//  Copyright © 2018 Tetsuya Hayashi. All rights reserved.
//

#include <iostream>
#include "ExpenseElement.h"
#include "PersonalExpense.h"
#include "ExpenseVisitor.h"

int main(int argc, const char * argv[]) {
    
    //create electricity bills on free store
    Bill* electBill_Aug18 = new Bill(Electicity);
    Bill* electBill_Sep18 = new Bill(Electicity);
    Bill* electBill_Oct18 = new Bill(Electicity);
    
    //wrong month to test class invariant(month = 13)
    if(!electBill_Aug18->setBill(2018,13,100)){
        cout << "Oops, wrong date was entered!!" << endl;
    }
    //wrong year to test class invariant(year = 1800)
    if(!electBill_Aug18->setBill(1800,8,100)){
        cout << "Oops, wrong date was entered!!" << endl;
    }
    
    cout << endl;
    
    //set valid records for Aug, Sep and Oct
    if(!electBill_Aug18->setBill(2018,8,100)){
        cout << "Oops, wrong date was entered!!" << endl;
        return 1;
    }
    if(!electBill_Sep18->setBill(2018,9,150)){
        cout << "Oops, wrong date was entered!!" << endl;
        return 1;
    }
    if(!electBill_Oct18->setBill(2018,10,130)){
        cout << "Oops, wrong date was entered!!" << endl;
        return 1;
    }
    
    //-----------------------------------
    //A customer didn't pay August bill!!
    //-----------------------------------
    //Combine Aug & Sep bills into a new bill and use the left operand month as a new billing month
    Bill newSepBill = (*electBill_Sep18) + (*electBill_Aug18);
    cout << "New Sep 18 electricity bill amount: " << newSepBill.getAmount() << endl;
    //Lets adjust the amount again using -= & -
    newSepBill = newSepBill - (*electBill_Aug18);
    cout << "Back to original Sep 18 electricity bill amount: " << newSepBill.getAmount() << endl;
    
    cout << endl;
    
    //Here modify Sep bill to add August bill
    (*electBill_Sep18) += (*electBill_Aug18);
    cout << "New Sep 18 electricity bill amount: " << electBill_Sep18->getAmount() << endl;
    //Here modify Sep bill to remove August bill
    (*electBill_Sep18) -= (*electBill_Aug18);
    cout << "Back to original Sep 18 electricity bill amount: " << electBill_Sep18->getAmount() << endl;
    
    cout << endl;
    
    //comparing the same bill, should return true(1);
    bool isSameBill = (*electBill_Aug18) == (*electBill_Aug18);
    cout << "The same bills? " << isSameBill << endl;
    
    //comparing different bills, should return false(0);
    isSameBill = (*electBill_Aug18) == (*electBill_Sep18);
    cout << "The same bills? " << isSameBill << endl;
    
    
    //comparing the same bill, should return false(0)
    bool isDiffBill = (*electBill_Aug18) != (*electBill_Aug18);
    cout << "Diff bills? " << isDiffBill << endl;
    
    //comparing different bills, should return true(1)
    isDiffBill = (*electBill_Aug18) != (*electBill_Sep18);
    cout << "Diff bills? " << isDiffBill << endl;
    
    cout << endl << endl;
    
    //-------------------------------------------------------------------------
    //Additional demonstrations of operator overloading using visitor pattern
    //-------------------------------------------------------------------------
    //create city bills on free store
    Bill* cityBill_Aug18 = new Bill(City);
    Bill* cityBill_Sep18 = new Bill(City);
    Bill* cityBill_Oct18 = new Bill(City);
    
    //wrong month to test class invariant(month = 13)
    if(!cityBill_Aug18->setBill(2018,8,90)){
        cout << "Oops, wrong date was entered!!" << endl;
        return 1;
    }
    //wrong year to test class invariant(year = 1800)
    if(!cityBill_Sep18->setBill(2018,9,120)){
        cout << "Oops, wrong date was entered!!" << endl;
        return 1;
    }
    //set valid records for Aug, Sep and Oct
    if(!cityBill_Oct18->setBill(2018,10,110)){
        cout << "Oops, wrong date was entered!!" << endl;
        return 1;
    }
    
    ElectricityExpense* elecBills = new ElectricityExpense("Electricity Expense");
//    cout << "Electricity Expense is created on a free store at " << elecBills << endl;
    elecBills->AddBill(electBill_Aug18);
    elecBills->AddBill(electBill_Sep18);
    elecBills->AddBill(electBill_Oct18);
    
    CityExpense* cityBills = new CityExpense("City Expense");
//    cout << "City Expense is created on a free store at " << cityBills << endl;
    cityBills->AddBill(cityBill_Aug18);
    cityBills->AddBill(cityBill_Sep18);
    cityBills->AddBill(cityBill_Oct18);
    
    PersonalExpense p_exp;
    p_exp.AddExpense(*elecBills);
    p_exp.AddExpense(*cityBills);
    
    CalcTotalVisitor calcTotalVisitor;
    calcTotalVisitor.VisitExpenses(p_exp);
    
    cout << endl;
    
    CalcAvgVisitor calcAvgVisitor;
    calcAvgVisitor.VisitExpenses(p_exp);
    
    cout << endl;
    
    return 0;
}
