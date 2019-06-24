#ifndef Data_h
#define Data_h

template<class T>
class Data{
    public:
        Data(T d);
        Data(const Data& d);
        ~Data();
        T getData() const;
        //[2 Points] == overload
        bool operator==(const Data<T>& data);
        bool operator>(const Data<T>& data);
        bool operator<(const Data<T>& data);
    private:
        T data;
};

#include "Data.cpp"

#endif