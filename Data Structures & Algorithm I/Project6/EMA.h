
#ifndef EMA_h
#define EMA_h

#include "DataObserverInterface.h"
#include "DataPacket.h"
#include "Error.h"
#include "SMA.h"
#include <iostream>

using namespace std;

// The Abstract Observer
template<class T>
class EMA: public virtual DataObserverInterface<T>
{
public:
    EMA(const EMA<T>& ema);
    EMA(int32_t p);
    ~EMA();
    void update(DataPacket<T>& dp) override;
    int32_t getPeriod();
    void setPeriod(int32_t p);
    template<typename U> friend class SMA;
protected:
    T value;

private:
    T prev_value;
    DataPacket<T>* packet = nullptr;
    bool isFirstIter = true;
    void calculate();
    void add(DataPacket<T>& dp);
    int32_t period = 0;
};

#include "EMA.cpp"

#endif