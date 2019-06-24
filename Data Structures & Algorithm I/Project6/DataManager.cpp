#include "DataManager.h"

template<class T>
void DataManager<T>::dataChange(DataPacket<T>& d){
    this->data = d;
    notifyObserver();
}

template<class T>
void DataManager<T>::registerObserver(DataObserverInterface<T>* ob){
    this->m_obs.push_back(ob);
}

template<class T>
void DataManager<T>::removeObserver(DataObserverInterface<T>* ob){
    this->m_obs.remove(ob);
}

template<class T>
void DataManager<T>::operator+=(DataObserverInterface<T>* ob){
    this->m_obs.push_back(ob);
}

template<class T>
void DataManager<T>::operator-=(DataObserverInterface<T>* ob){
    this->m_obs.remove(ob);
}

template<class T>
typename list<DataObserverInterface<T>* >::iterator DataManager<T>::begin(){
    return this->m_obs.begin();
}

template<class T>
typename list<DataObserverInterface<T>* >::iterator DataManager<T>::end(){
    return this->m_obs.end();
}

template<class T>
void DataManager<T>::notifyObserver(){
    auto pos = this->m_obs.begin();
    while(pos != this->m_obs.end()){
        ((DataObserverInterface<T>*)(*pos))->update(this->data);
        ++pos;
    }
}