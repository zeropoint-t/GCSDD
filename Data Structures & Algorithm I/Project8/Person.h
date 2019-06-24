#ifndef Person_h
#define Person_h

#include <string>
using namespace std;

//person
class Person{
public:
    Person(string name);
	virtual string getName() const;
	virtual void setName(string name);
private:
	string name;
};

#endif