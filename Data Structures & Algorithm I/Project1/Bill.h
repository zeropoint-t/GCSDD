//
//  Bill.hpp
//  HW1
//
//  Created by Tetsuya Hayashi on 10/7/18.
//  Copyright © 2018 Tetsuya Hayashi. All rights reserved.
//

#ifndef Bill_hpp
#define Bill_hpp

#include <stdio.h>

enum BillType { Electicity, City };

class Bill{
public:
    //Constructors - Destructor
    Bill(BillType type);
    Bill(const Bill& bill);//copy constructor
    ~Bill();
    
    bool setBill(int year, int month, double amount);
    
    Bill operator+(const Bill& bill) const;
    Bill& operator+=(const Bill& bill);
  
    Bill operator-(const Bill& bill) const;
    Bill& operator-=(const Bill& bill);
    
    bool operator==(const Bill& bill) const;
    bool operator!=(const Bill& bill) const;
    
    BillType getType() const;
    int getYear() const;
    int getMonth() const;
    double getAmount() const;
private:
    BillType type;
    int year;
    int month;
    double amount;
};

#endif /* Bill_hpp */
