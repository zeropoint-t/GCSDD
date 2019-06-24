
#include "Person.h"

Person::Person(string name):name(name){}

void Person::setName(string name){
    this->name = name;
}

string Person::getName() const{
    return this->name;
}