
#include <iostream>
#include <algorithm>

#include "Instructor.h"

using namespace std;

Instructor::Instructor(string instructorName):Person(instructorName){}

void Instructor::addStudent(string&& studentName){
    if(studentName.size() == 0)
        throw runtime_error("Student name can not be empty");
    if(studentName.size() > 50)
        throw runtime_error("Student name is too long");

    auto it = find_if(students.begin(), students.end(), [&studentName](const Student& obj){return obj.getName() == studentName;});
    if(it != students.end())//students exists
        throw runtime_error("Student already exists");

    Student newStudent(studentName);
    students.push_back(newStudent);
}

Student Instructor::removeStudent(string&& studentName){
    if(studentName.size() == 0)
        throw runtime_error("Student name can not be empty");
    if(studentName.size() > 50)
        throw runtime_error("Student name is too long");

    auto it = find_if(students.begin(), students.end(), [&studentName](const Student& obj){return obj.getName() == studentName;});
    if(it == students.end())//students exists
        throw runtime_error("Student does not exist and cannot be removed");
    
    Student s = *it;
    students.erase(it);
    return s;
}

int Instructor::getClassSize(){
    return students.size();
}

void Instructor::displayClass(){
    cout << "Instructor: " << this->getName() << endl;
    for(auto it = students.begin(); it != students.end(); it++){
        cout << "   Student: " << it->getName() << endl; 
    }
}