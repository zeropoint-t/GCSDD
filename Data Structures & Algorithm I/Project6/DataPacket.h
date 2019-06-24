
#ifndef DataPacket_h
#define DataPacket_h

#include <ctime>
#include <iostream>
using namespace std;

template<typename T>
struct DataPacket{
    time_t utc_timestamp;
    T open;
    T high;
    T low;
    T close;
    int32_t volume;

    ~DataPacket(){
        // cout << "DataPacket destroyed at " << this << endl;
    }

    void display(){
        char buff[80];
        strftime(buff, 20, "%Y/%m/%d %H:%M:%S", localtime(&this->utc_timestamp));
        cout << "Datetime: " << buff;
        cout << " Close: " << this->close;
        cout << " Volume: " << this->volume << endl;
    }
};

#endif