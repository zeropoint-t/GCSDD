#ifndef set_h
#define set_h

#include <map>
using namespace std;

template<class T>
bool operator<(const T &d1, const T &d2);

template<class T>
class Set
{
public:
    Set();
    void Insert(T& d);
    //[1 Point] A way to obtain the counter for a given key
    int GetCounter(T& d);
    void PrintAll() const;
    void PrintTopN(int top) const;
private:
    //T is key, and value is used as a counter which keeps track of 
    //the number of times the same key was attempted to be added
    map<T,int> m;
};

#include "Set.cpp"

#endif