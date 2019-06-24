// ------------------------------------------------ file name ------------------------------------------------------- -
// Programmer Name:             Tetsuya Hayashi
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               01/13/2019
// Date of Last Modification:   01/13/2019
// -------------------------------------------------------------------------------------------------------------------- 
// Poly - Header file - this class represents a polynomial function that allows addition,
// subtraction and multiplication of ploynomals
// -------------------------------------------------------------------------------------------------------------------- 
// Notes on specifications, special algorithms, and assumptions.
// 
// --------------------------------------------------------------------------------------------------------------------
#ifndef Poly_h
#define Poly_h

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Poly
{
    //prints a polynomial function: ex. +15x^11 +15x^6 -15
    friend ostream & operator << (ostream &out, const Poly &p);

    //enter one coefficient and an exponent repeatedly, Enters -1 for both when done
    //to set A, Poly A(2,7); so that A is +5x^7 -4x^3 +10x –2
    //the user enters the pairs in any order:5 7 10 1 -4 3 -2 0 -1 -1 on one line or multiple lines.
    //pre condition: exponent must be non-negative or it defaults to 0
    friend istream & operator >> (istream &in, Poly &p) ;
public:
    
    Poly();                     //default constructor
    Poly(int, int = 0); //constructor that takes coefficient & exponent(defaults to 0 if not provided)
    Poly(const Poly&);          //copy constructor
    ~Poly();                    //destructor


    Poly& operator=(const Poly&);   //assign poly object
    Poly operator+(const Poly&);    //add 2 poly objects
    Poly operator-(const Poly&);    //subtract 2  poly objects
    Poly operator*(const Poly&);    //multiply 2 poly objects
    Poly& operator+=(const Poly&);  //add & assign to left operand
    Poly& operator-=(const Poly&);  //subtract & assign to left operand
    Poly& operator*=(const Poly&);  //multiply & assign to left operand
    bool operator==(const Poly&);   //equality, 2 poly objects are identical
    bool operator!=(const Poly&);   //inequality, 2 poly objects are different
    bool operator>>(const string);  //set coefficients with a string. ex (5 7 -4 3 10 1 -2 0) = +5x^7 -4x^3 +10x -2
 
    int getCoeff(int) const;    //returns a coeff of the exp(0 if the coeff is not found)
    void setCoeff(int,int); //sets a coefficient and exponent. mutator
private:
    int* arr;           //pointer to first element of array
    int maxExponent;    //maximum exponent
    void resetArr();        //sets 0 to all elements in arr
};

#endif