#include "Data.h"

template<class T>
Data<T>::Data(T d){
    this->data = d;
}

template<class T>
Data<T>::Data(const Data& d){
    this->data = d.getData();
    // cout << "Data object copied at " << this << endl;
}

template<class T>
Data<T>::~Data<T>(){
    // cout << "Data object destroyed at " << this << endl;
}

template<class T>
T Data<T>::getData() const{
    return this->data;
}

template<class T>
bool Data<T>::operator==(const Data<T>& d){
    if(d.data == this->data)
        return true;

    return false;
}

template<class T>
bool Data<T>::operator>(const Data<T>& d){
    if(d.data > this->data)
        return true;

    return false;
}

template<class T>
bool Data<T>::operator<(const Data<T>& d){
    if(d.data < this->data)
        return true;

    return false;
}