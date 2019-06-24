#include "EMA.h"

template<class T>
EMA<T>::EMA(const EMA<T>& ema):DataObserverInterface<T>("EMA Observer"){
    setPeriod(ema.period);
}

template<class T>
EMA<T>::EMA(int32_t p):DataObserverInterface<T>("EMA Observer"){
    setPeriod(p);
}

template<class T>
EMA<T>::~EMA(){
    if(packet != nullptr)
        delete packet;
}

template<class T>
int32_t EMA<T>::getPeriod(){
    return this->period;
}

template<class T>
void EMA<T>::setPeriod(int32_t p){
    if(p <= 0 || p > 30){
        throw std::out_of_range("EMA: period out of range exception");
    }

    this->period = p;
}

template<class T>
void EMA<T>::update(DataPacket<T>& dp){
    add(dp);
}

template<class T>
void EMA<T>::add(DataPacket<T>& dp){
    if(packet != nullptr)
        delete packet;

    this->packet = new DataPacket<T>(dp);
    calculate();
}

template<class T>
void EMA<T>::calculate(){
    if(isFirstIter){
        this->prev_value = this->packet->close;
        this->value = this->packet->close;
        isFirstIter = false;
    }else{
        this->value = (this->packet->close - this->prev_value) * (2 / this->period + 1) + this->prev_value;
    }
    cout << "EMA: " << this->value << endl;
}