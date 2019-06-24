
#ifndef MACD_h
#define MACD_h

#include "DataObserverInterface.h"
#include "SMA.h"
#include "EMA.h"
#include "DataPacket.h"
#include <list>
#include <iostream>

using namespace std;

//simplified version of Moving Average Convergence Divergence
template<class T>
class MACD: public virtual DataObserverInterface<T>, public virtual SMA<T>, public virtual EMA<T>
{
public:
    MACD(const MACD<T>& macd);
    MACD(int32_t p1, int32_t p2);
    ~MACD();
    void update(DataPacket<T>& dp) override;
    void setPeriod(int32_t p1, int32_t p2);
    
private:
    void add(DataPacket<T>& dp);

    int32_t period1 = 0;
    int32_t period2 = 0;

    T sma_value;
    T ema_value;
    T hist_value;
    void calculate();
};

#include "MACD.cpp"

#endif