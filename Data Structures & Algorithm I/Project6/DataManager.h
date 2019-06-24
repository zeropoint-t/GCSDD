#ifndef DataManager_h
#define DataManager_h

#include "DataManagerInterface.h"
#include "DataPacket.h"
#include <list>

using namespace std;

template<class T>
class DataManager: public DataManagerInterface<T>{
public:
    void dataChange(DataPacket<T>& d);

    void registerObserver(DataObserverInterface<T>* ob);
    void removeObserver(DataObserverInterface<T>* ob);

    void operator+=(DataObserverInterface<T>* ob);
    void operator-=(DataObserverInterface<T>* ob);

    void notifyObserver();

    typename list<DataObserverInterface<T>* >::iterator begin();
    typename list<DataObserverInterface<T>* >::iterator end();
private:
    DataPacket<T> data;
    list<DataObserverInterface<T>* > m_obs;
};

#include "DataManager.cpp"

#endif