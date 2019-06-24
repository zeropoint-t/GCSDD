#ifndef Instructor_h
#define Instructor_h

#include <string>
#include <vector>

#include "Person.h"
#include "Student.h"

using namespace std;

//Instructor
class Instructor : public Person{
public:
    Instructor(string instructorName);
	void addStudent(string&& studentName);
	Student removeStudent(string&& studentName);
	int getClassSize();
    void displayClass();
    vector<Student>::iterator begin(){
        return students.begin();
    }
    vector<Student>::iterator end(){
        return students.end();
    }
private:
	vector<Student> students;
};

#endif