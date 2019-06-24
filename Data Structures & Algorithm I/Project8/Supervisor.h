
#ifndef Supervisor_h
#define Supervisor_h

#include <string>
#include <stdexcept>
#include <algorithm>

#include "Instructor.h"

using namespace std;

//Supervisor
class Supervisor{
public:
	void addInstructor(string&& instructorName);
    void addStudent(string&& studentName);
    void addStudent(string&& studentName, string&& instructorName);
	void removeInstructor(string&& instructorName);
	void removeStudent(string&& studentName);
    void displayClasses();
private:
	vector<Instructor> instructors;
};

#endif