#include "PersonalExpense.h"
#include <iostream>
#include <typeinfo>

//Constructor
PersonalExpense::PersonalExpense(){}

//Destructor
PersonalExpense::~PersonalExpense(){
    for(auto it = expenses.begin(); it != expenses.end(); it++){
        string type = typeid(it).name();
        std::cout << type << " is being destroyed at " << *it << std::endl;
        delete *it;
    }
}

void PersonalExpense::AddExpense(ExpenseElement& expense){
    expenses.push_back(&expense);
}

vector<ExpenseElement*>& PersonalExpense::GetExpenses(){
    return expenses;
}
