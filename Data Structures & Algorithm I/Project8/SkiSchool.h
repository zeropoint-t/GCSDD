#ifndef SkiSchool_h
#define SkiSchool_h

#include <string>
#include <iostream>
#include "Supervisor.h"

using namespace std;

class SkiSchool {
public:
    SkiSchool();
    ~SkiSchool();
	//[2 Points] Insert instructor
	void registerInstructor(string instructorName);

	//[2 Points] Insert a student without specifying instructor
	void registerStudent(string studentName);

	//[3 Points] Insert a student with specifying instructor
    void registerStudent(string studentName, string instructorName);

	//[2 Points] Remove Instructor
	void quitInstructor(string instructorName);

	//[2 Points] Remove student
	void quitStudent(string studentName);

	//[3 Points] “Display” function – “prints” out all teachers with all students under each teacher
	void displayClasses() noexcept;
private:
	Supervisor supervisor;
	int numberOfInstructors; //supervisor.instructor_list.size()
	int numberOfStudents;
};

#endif