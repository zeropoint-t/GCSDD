#include <iostream>
#include <typeinfo>
#include "ExpenseElement.h"

//--------------------------------
// ExpenseElement
//--------------------------------
//Constructor - base class
ExpenseElement::ExpenseElement(string expenseType):expenseType(expenseType){
}

//Destructor -- base class
ExpenseElement::~ExpenseElement(){
    
}

string ExpenseElement::getExpenseType(){
    return this->expenseType;
}

vector<Bill*>& ExpenseElement::getBills(){
    return this->bills;
}

//--------------------------------
// ElectricityExpense
//--------------------------------
//Costructor--ElectricityExpense
ElectricityExpense::ElectricityExpense(string expenseType):ExpenseElement(expenseType){
}

//Destructor--ElectricityExpense
ElectricityExpense::~ElectricityExpense(){
    for(auto it = ExpenseElement::getBills().begin(); it != ExpenseElement::getBills().end(); it++){
        string type = typeid(it).name();
        std::cout << type << " is being destroyed at " << *it << std::endl;
        delete *it;
    }
}

void ElectricityExpense::Accept(const ExpenseVisitor& visitor){
    visitor.Visit(*this);
}

void ElectricityExpense::AddBill(Bill* bill) {
    ExpenseElement::getBills().push_back(bill);
}

//--------------------------------
// CityExpense
//--------------------------------
//Costructor
CityExpense::CityExpense(string expenseType):ExpenseElement(expenseType){
}

//Destructor
CityExpense::~CityExpense(){
    for(auto it = ExpenseElement::getBills().begin(); it != ExpenseElement::getBills().end(); it++){
        string type = typeid(it).name();
        std::cout << type << " is being destroyed at " << *it << std::endl;
        delete *it;
    }
}

void CityExpense::Accept(const ExpenseVisitor& visitor){
    visitor.Visit(*this);
}

void CityExpense::AddBill(Bill* bill) {
    ExpenseElement::getBills().push_back(bill);
}
