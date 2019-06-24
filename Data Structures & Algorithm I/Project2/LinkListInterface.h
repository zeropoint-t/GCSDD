#ifndef ListInterface_h
#define ListInterface_h

#include <stdio.h>
#include "House.h"

enum SortType { desc, asc };

class HouseListInterface
{
public:

    virtual bool isEmpty() const = 0;

    virtual int getLength() const = 0;
    
    virtual bool insert(int newIndex, House& newHouse) = 0;
    
    virtual bool remove(int index) = 0;
    
    virtual void clear() = 0;
       
    virtual void sort(SortType sortType) = 0;

    virtual ~HouseListInterface() { }

    virtual void displayForward() = 0;

    virtual void displayBackward() = 0;
};
#endif
