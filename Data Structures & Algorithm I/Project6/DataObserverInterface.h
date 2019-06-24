#ifndef DataObserverInterface_h
#define DataObserverInterface_h

#include "DataPacket.h"

// The Abstract Observer
template<class T>
class DataObserverInterface
{
public:
    //constructors
    DataObserverInterface(string msg){
        setMessage(msg);
    }
    DataObserverInterface(){}

    virtual void update(DataPacket<T>& dp) = 0;

    virtual void display(){
        cout << msg << endl;
    }

    void setMessage(string msg){
        this->msg = msg;
    }

    string getMessage(string msg){
        if(msg == "")
            throw runtime_error("Observer can't have empty message");

        return this->msg;
    }
private:
    string msg = "This is a Data Observer Parent Class";
};

#endif