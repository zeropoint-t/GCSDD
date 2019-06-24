//
//  Bill.cpp
//  HW1
//
//  Created by Tetsuya Hayashi on 10/7/18.
//  Copyright © 2018 Tetsuya Hayashi. All rights reserved.
//

#include <iostream>
#include "Bill.h"

Bill::Bill(BillType type):type(type),year(0),month(0),amount(0){
}

Bill::Bill(const Bill& bill){
    this->type = bill.getType();
    this->year = bill.getYear();
    this->month = bill.getMonth();
    this->amount = bill.getAmount();
}

Bill::~Bill(){
}

bool Bill::setBill(int year, int month, double amount){
    if(year <= 1900 || year >= 2049 || month <= 0 || month >= 13){
        return false;
    }
    this->year = year;
    this->month = month;
    this->amount = amount;
    return true;
}

Bill Bill::operator+(const Bill& bill) const{
    Bill newBill(bill.getType());
    newBill.setBill(this->year, this->month, this->amount);
    return newBill+=bill;
}

Bill& Bill::operator+=(const Bill& bill){
    //take the greater year & month of 2 as a new billing month date
//    int newBillingYear = 0;
//    int newBillingMonth = 0;
//    if(this->year > bill.getYear()){
//        newBillingYear = this->year;
//        newBillingMonth = this->month;
//    }else if(this->year == bill.getYear()){
//        newBillingYear = this->month > bill.getMonth() ? this->year : bill.getYear();
//        newBillingMonth = this->month > bill.getMonth() ? this->month : bill.getMonth();
//    }else{
//        newBillingYear = bill.getYear();
//        newBillingMonth = bill.getMonth();
//    }
    
//    this->year = newBillingYear;
//    this->month = newBillingMonth;
    this->amount += bill.getAmount();
    
    return *this;
}

Bill Bill::operator-(const Bill& bill) const{
    Bill newBill(bill.getType());
    newBill.setBill(this->year, this->month, this->amount);
    return newBill-=bill;
}

Bill& Bill::operator-=(const Bill& bill){
    this->amount -= bill.getAmount();
    return *this;
}

bool Bill::operator==(const Bill& bill) const{
    if(this->type == bill.getType()
       && this->year == bill.getYear()
       && this->month == bill.getMonth()){
        return true;
    }
    return false;
}

bool Bill::operator!=(const Bill& bill) const{
    if(this->type == bill.getType()
       && this->year == bill.getYear()
       && this->month == bill.getMonth()){
        return false;
    }
    return true;
}


int Bill::getYear() const{
    return this->year;
}

int Bill::getMonth() const {
    return this->month;
}

double Bill::getAmount() const {
    return this->amount;
}

BillType Bill::getType() const{
    return this->type;
}
