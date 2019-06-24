
#include "MACD.h"

template<class T>
MACD<T>::MACD(const MACD<T>& macd)
try :period1(macd.period1),period2(macd.period2),
EMA<T>(macd.period1),SMA<T>(macd.period2),DataObserverInterface<T>("MACD Observer")
{
}catch(const std::out_of_range& oor) {
    throw std::out_of_range("MACD: period out of range exception");
}

template<class T>
MACD<T>::MACD(int32_t p1, int32_t p2)
try :period1(p1),period2(p2),EMA<T>(p1),SMA<T>(p2),DataObserverInterface<T>("MACD Observer")
{
}catch(const std::out_of_range& oor) {
    throw std::out_of_range("MACD: period out of range exception");
}

template<class T>
MACD<T>::~MACD(){
}

template<class T>
void MACD<T>::update(DataPacket<T>& dp){

    SMA<T>::update(dp);
    EMA<T>::update(dp);

    this->sma_value = SMA<T>::value;
    this->ema_value = EMA<T>::value;
    this->hist_value = this->ema_value - this->sma_value;

    cout << "Hist Value: " << this->hist_value << endl;
}

template<class T>
void MACD<T>::setPeriod(int32_t p1, int32_t p2){
    if(p1 <= 0 || p1 > 300 || p2 <= 0 || p2 > 300)
        throw std::out_of_range("MACD: period out of range exception");

    period1 = p1;
    period2 = p2;
}