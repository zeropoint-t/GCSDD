
#ifndef DataManagerInterface_h
#define DataManagerInterface_h

#include "DataObserverInterface.h"
#include <list>

using namespace std;

// The Abstract Subject
template<class T>
class DataManagerInterface
{
public:
    virtual void registerObserver(DataObserverInterface<T>* ob) = 0;
    virtual void removeObserver(DataObserverInterface<T>* ob) = 0;
    virtual void notifyObserver() = 0;

    virtual void operator+=(DataObserverInterface<T>* ob) = 0;
    virtual void operator-=(DataObserverInterface<T>* ob) = 0;

    virtual void displayObservers(typename list<DataObserverInterface<T>* >::iterator beg, typename list<DataObserverInterface<T>* >::iterator end){
        for(auto it = beg; it != end; it++){
            (*it)->display();
        }
    }
};

#endif