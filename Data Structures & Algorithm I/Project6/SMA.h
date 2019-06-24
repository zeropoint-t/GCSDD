
#ifndef SMA_h
#define SMA_h

#include "DataObserverInterface.h"
#include "DataPacket.h"
#include "Error.h"
#include "EMA.h"
#include <iostream>

using namespace std;

// The Abstract Observer
template<class T>
class SMA: public virtual DataObserverInterface<T>
{
public:
    SMA(const SMA<T>& sma);
    SMA(int32_t p);
    ~SMA();
    void update(DataPacket<T>& dp) override;
    int32_t getPeriod();
    void setPeriod(int32_t p);
    void ShowEMAValue(EMA<T>& ema) const;

protected:
    T value;

private:
    DataPacket<T>** values = nullptr;
    void calculate();
    void add(DataPacket<T>& dp);
    int32_t period = 0;
    int curIndex = 0;
};

#include "SMA.cpp"

#endif