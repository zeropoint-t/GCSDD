// -- poly.cpp ------------------------------------------------------------------------------------
// Programmer Name:             Tetsuya Hayashi
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               01/13/2019
// Date of Last Modification:   01/13/2019
// ---------------------------------------------------------------------------------------------- 
// Poly - implementation file - this class represents implementations of polynomial functions 
// that allows addition, subtraction and multiplication
// ---------------------------------------------------------------------------------------------- 
// Notes on specifications, special algorithms, and assumptions.
// 
// ----------------------------------------------------------------------------------------------
#include "poly.h"

// --ostream & operator <<-----------------------------------------------------------------------
// Description: overload cout that prints a polynomial function: ex. +15x^11 +15x^6 -15
// ----------------------------------------------------------------------------------------------
ostream & operator << (ostream &out, const Poly &p) 
{
    if(p.maxExponent <= 0)
    {//only 1 term with 0 exponent
        if(p.arr[0] < 0)
        {
            out << " -" << p.arr[0];
        }else if(p.arr[0] > 0)
        {
            out << " +" << p.arr[0];
        }else//must be 0
        {
            out << p.arr[0];
        }
    }else
    {
        for(int i = p.maxExponent; i >= 0; i--)
        {
            if(p.arr[i] != 0)
            {
                if(i == 1)//when exponent is 1
                {
                    if(p.arr[i] < 0)
                    {
                        out << " " << p.arr[i] << "x";//negative number already has "-" sign
                    }else
                    {
                        out << " +" << p.arr[i] << "x";
                    }
                }else if(i == 0)//when exponent is 0
                {
                    if(p.arr[i] < 0)
                    {
                        out << " " << p.arr[i];//negative number already has "-" sign
                    }else
                    {
                        out << " +" << p.arr[i];
                    }
                }else//exponent is > 1 and < maxExponent
                {
                    if(p.arr[i] < 0)
                    {
                        out << " " << p.arr[i] << "x" << "^" << i;//negative number already has "-" sign
                    }else
                    {
                        out << " +" << p.arr[i] << "x" << "^" << i;
                    }
                }
            }// end of if(p.arr[i] != 0)
        }//end of for loop
    }//end of if(p.maxExponent <= 0)
    return out; 
}

// --istream & operator >>-----------------------------------------------------------------------
// Description: overload cin which takes coefficient and exponent pairs from a user and 
//              set coefficient for the exponent
// ----------------------------------------------------------------------------------------------
istream & operator >> (istream &in, Poly &p) 
{
    //reset p
    p.resetArr();

    //Loop until break is called. 
    while(true)
    {
        //take user inputs
        string line;

        //take input from a user
        getline(cin, line);

        //assign coeff and exp pairs
        //if -1 -1 is in line, terminate will return true so break the loop
        bool terminate = p >> line;

        if(terminate)
            break;
    }//end while
    return in; 
}

// --default constructor-----------------------------------------------------------------------
// Description: default constructor
// --------------------------------------------------------------------------------------------
Poly::Poly():arr(NULL), maxExponent(-1)
{
    // cout << "Default Constructor at " << this << endl;
}

// --Poly::Poly(int coeff, int exp)------------------------------------------------------------
// Description: a constructor that takes initial coefficient and exponent
// Precondition: exponent must be non negative
// --------------------------------------------------------------------------------------------
Poly::Poly(int coeff, int exp):
    maxExponent(exp < 0 ? 0 : exp),
    arr(new int[(exp < 0 ? 0 : exp) + 1])//initialize at least 1 element
{
    //set all elements to 0
    this->resetArr();
    //set coeff to exponent
    this->arr[this->maxExponent] = coeff;
}

// --Poly::Poly(const Poly& p)-----------------------------------------------------------------
// Description: a copy constructor
// --------------------------------------------------------------------------------------------
Poly::Poly(const Poly& p): 
    maxExponent(p.maxExponent), 
    arr(new int[p.maxExponent + 1])
{
    //set all elements to 0
    this->resetArr();

    if(this != &p)
    {
        for(int i = 0; i <= this->maxExponent; i++)
        {
            arr[i] = p.arr[i];
        }
    }
}

// --Poly::~Poly()-----------------------------------------------------------------------------
// Description: destructor - calls clear function that releases dynamic array
// --------------------------------------------------------------------------------------------
Poly::~Poly()
{
    // cout << "destructor called at " << this << endl;
    // this->clear();
    delete[] this->arr;
    this->maxExponent = -1;
}

// --Poly& Poly::operator=(const Poly& p)------------------------------------------------------
// Description: operator overload of = which performs a deep copy of the operand
// --------------------------------------------------------------------------------------------
Poly& Poly::operator=(const Poly& p)
{
    if(this != &p)
    {
        if(maxExponent != -1)
        {
            delete[] this->arr;
        }

        this->maxExponent = p.maxExponent;
        this->arr = new int[this->maxExponent + 1];
        for(int i = 0; i <= this->maxExponent; i++)
        {
            this->arr[i] = p.arr[i];
        }
    }
    return *this;
}

// --Poly& Poly::operator+(const Poly& p)------------------------------------------------------
// Description: operator overload of "+" which performs addition of this & operand
//              and returns a copy of new Poly object
// --------------------------------------------------------------------------------------------
Poly Poly::operator+(const Poly& p)
{
    //find max exponent between the 2 poly objects
    int maxExponent = this->maxExponent > p.maxExponent ? this->maxExponent : p.maxExponent;
    if(maxExponent >= 0)
    {
        //create new Poly object to return
        Poly newPoly(0,maxExponent);
        //add like terms
        //visit each exponent subscript from 0 to max exponent
        for(int exp = 0; exp <= maxExponent; exp++)
        {
            //use 0 if exp exceeds maxExponent
            int coeff1 = this->maxExponent >= exp ? this->arr[exp] : 0;
            int coeff2 = p.maxExponent >= exp ? p.arr[exp] : 0;

            //add coefficient
            int new_coeff = coeff1 + coeff2;

            if(new_coeff != 0)
            {
                //set coefficient
                newPoly.setCoeff(new_coeff,exp);
            }
        }
        return newPoly;
    }else
    {
        return Poly();
    }
}

// --Poly& Poly::operator-(const Poly& p)------------------------------------------------------
// Description: operator overload of "-" which performs subtraction of this & operand
//              and returns a copy of new Poly object
// --------------------------------------------------------------------------------------------
Poly Poly::operator-(const Poly& p)
{
    //find max exponent between the 2 poly objects
    int maxExponent = this->maxExponent > p.maxExponent ? this->maxExponent : p.maxExponent;
    if(maxExponent >= 0)
    {
        //create new Poly object to return
        Poly newPoly(0,maxExponent);
        //subtract like terms
        //visit each exponent subscript from 0 to max exponent
        for(int exp = 0; exp <= maxExponent; exp++)
        {
            //use 0 if exp exceeds maxExponent
            int coeff1 = this->maxExponent >= exp ? this->arr[exp] : 0;
            int coeff2 = p.maxExponent >= exp ? p.arr[exp] : 0;

            //subtract coefficient
            int new_coeff = coeff1 - coeff2;

            //set coefficient
            newPoly.setCoeff(new_coeff,exp);
        }
        return newPoly;
    }else
    {
        return Poly();
    }
}

// --Poly& Poly::operator*(const Poly& p)------------------------------------------------------
// Description: operator overload of "*" which performs multiplication of this & operand
//              and returns a copy of new Poly object
// --------------------------------------------------------------------------------------------
Poly Poly::operator*(const Poly& p)
{
    Poly newPoly;
    for(int i = 0; i <= this->maxExponent; i++)
    {
        if(this->arr[i] == 0)//skip if coefficient = 0
            continue;
        for(int j = 0; j <= p.maxExponent; j++)
        {
            if(p.arr[j] == 0)//skip if coefficient = 0
                continue;
            int coeff1 = this->arr[i];
            int coeff2 = p.arr[j];
            int coeff = coeff1 * coeff2;
            int exp = i + j;
            Poly temp(coeff, exp);
            newPoly += temp;
        }//end of inner for loop
    }//end of outer for loop

    return newPoly;
}

// --Poly& Poly::operator+=(const Poly& p)-----------------------------------------------------
// Description: operator overload of "+=" which performs addition of this & operand
//              and mutates this and returns a reference of this
// --------------------------------------------------------------------------------------------
Poly& Poly::operator+=(const Poly& p)
{
    *this = *this + p;
    return *this;
}

// --Poly& Poly::operator-=(const Poly& p)-----------------------------------------------------
// Description: operator overload of "-=" which performs subtraction of this & operand
//              and mutates this and returns a reference of this
// --------------------------------------------------------------------------------------------
Poly& Poly::operator-=(const Poly& p)
{
    *this = *this - p;
    return *this;
}

// --Poly& Poly::operator*=(const Poly& p)-----------------------------------------------------
// Description: operator overload of "*=" which performs multiplication of this & operand
//              and mutates this and returns a reference of this
// --------------------------------------------------------------------------------------------
Poly& Poly::operator*=(const Poly& p)
{
    *this = *this * p;
    return *this;
}

// --Poly& Poly::operator==(const Poly& p)-----------------------------------------------------
// Description: operator overload of "==" which compares equality of this & operand
//              true if both are initialized with default constructor or both have the same
//              number of maxExponent and all coefficients are the same
// --------------------------------------------------------------------------------------------
bool Poly::operator==(const Poly& p)
{
    //flags true or false
    bool isSame = true;

    //these 2 variables are necessary if 2 poly objects have different lengths which requires additional check
    int maxExp = this->maxExponent > p.maxExponent ? this->maxExponent : p.maxExponent;
    int minExp = this->maxExponent < p.maxExponent ? this->maxExponent : p.maxExponent;

    //both are initialized with default constructor
    //so they are the same
    if(this->maxExponent == -1 && p.maxExponent == -1)
        return true;
    
    //if any coefficients are different from one another
    //these 2 poly objects contain 2 different polynomial representations
    for(int i = 0; i <= minExp; i++)
    {
        if(this->arr[i] != p.arr[i])
        {
            isSame = false;
            break;
        }
    }

    //2 polys appear to be the same up to minExp
    //find out if contain non-zero values exist in remaining elements in larger poly
    //which indicates inequality
    if(isSame && maxExp != minExp)
    {
        const Poly& largerPoly = this->maxExponent > p.maxExponent ? *this : p;
        for(int i = minExp + 1; i <= maxExp; i++)
        {
            cout << largerPoly.arr[i] << endl;
            if(largerPoly.arr[i] != 0)
                isSame = false;
        }
    }

    //if it reaches here, these represent the same polynomials
    return isSame;
}

// --Poly& Poly::operator!=(const Poly& p)-----------------------------------------------------
// Description: operator overload of "!=" which compares inequality of this & operand
//              true if they have different number of maxExponent or
//              some coefficients are different
// --------------------------------------------------------------------------------------------
bool Poly::operator!=(const Poly& p)
{
    //if maxExponent is different, these 2 poly objects contain 
    //different polynomial representations
    if(this->maxExponent != p.maxExponent)
        return true;

    //both are initialized with default constructor
    //so they are the same
    if(this->maxExponent == -1)
        return false;
    
    //if any coefficients are different from one another
    //these 2 poly objects contain 2 different polynomial representations
    for(int i = 0; i <= this->maxExponent; i++)
    {
        if(this->arr[i] != p.arr[i])
            return true;
    }

    //if it reaches here, these represent the same polynomials
    return false;
}

// --void Poly::operator>>(const string line)--------------------------------------------------
// Description: operator overload of ">>" which takes a string as a parameter
//              The input parameter is expected to be intergers separated by a blank space
//              and it should represent a sequence of pair(s) of coefficient and exponent pair
//              ex. "5 7 2 3 6 0"sets this poly object to represent "5x^7 +2x^3 + 6"
// --------------------------------------------------------------------------------------------
bool Poly::operator>>(const string line)
{
    //measure the size of inputs
    int size = 0;
    string val;
    stringstream ssize(line);
    while (ssize.good())
    {
        ssize >> val;
        ++size;
    }
    ssize.clear();

    //no element or only 1 elemet is present
    if(size <= 1)
    {
        // cout << "no element or only 1 elemet is present" << endl;
        return false;
    }

    //fill in inputs array
    int inputs[size];
    for (int i = 0; i < size; i++) 
    {
        inputs[i] = 0;// Initialize all elements to zero.
    }

    // cout << "inputs address: " << inputs << endl;
    stringstream ssin(line);
    int i = 0;
    while (ssin.good())
    {
        ssin >> inputs[i++];
    }
    ssin.clear();

    //set coefficient and exponent
    int coeff = 0;
    int exp = 0;
    for(i = 0; i < size; i++)
    {
        coeff = inputs[i++];
        if(i < size)
            exp = inputs[i];

        //terminate flag is true if coeff and exp are both -1
        if(coeff == -1 && exp == -1)
        {
            return true;
        }
        this->setCoeff(coeff, exp);
    }

    return false;//terminate flag is false without coeff & exp are both -1
}

// --int Poly::getCoeff(int exp)---------------------------------------------------------------
// Description: returns a coefficient of an exponent. If exp is out of range, return 0
// --------------------------------------------------------------------------------------------
int Poly::getCoeff(int exp) const
{
    if(exp >= 0 && exp <= this->maxExponent)
    {
        return this->arr[exp];
    }else
    {
        return 0;
    }
}

// --Poly::setCoeff(int coeff, int exp)--------------------------------------------------------
// Description: This is a mutator function that sets coefficient & exponent.
//              If exp is larger than current maxExponent, it resizes the internal array
// Precondition: exponent must be non-negative
// --------------------------------------------------------------------------------------------
void Poly::setCoeff(int coeff, int exp)
{
    //ensure that exponent is non-negative
    if(exp < 0) exp = 0;

    //resize arr if new exp is bigger than currnet exp
    if(exp > this->maxExponent)
    {
        //create a new array with the size of the new exponent
        int* newArr = new int[exp + 1];
        for(int i = 0; i <= exp; i++)
        {
            newArr[i] = 0;
        }
        //copy over elements
        for(int i = 0; i <= this->maxExponent; i++)
        {
            newArr[i] = this->arr[i];
        }
        //free up the space for the internal array
        if(this->maxExponent != -1)
        {
            delete[] this->arr;
            this->arr = NULL;
        }
        
        //assign new array to internal array
        this->arr = newArr;
        //set maxExponent to the new exp
        this->maxExponent = exp;
    }

    //if exp is in a valid range, assign new coefficient
    if(exp >= 0 && exp <= this->maxExponent)
    {
        this->arr[exp] = coeff;
    }
}

// --Poly::resetArr()--------------------------------------------------------
// Description: Sets all elements in arr to 0
// --------------------------------------------------------------------------------------------
void Poly::resetArr()
{
    for(int i = 0; i <= this->maxExponent; i++)
    {
        this->arr[i] = 0;
    }
}


