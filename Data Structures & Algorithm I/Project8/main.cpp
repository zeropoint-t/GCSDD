
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Person.h"
#include "Student.h"
#include "Instructor.h"
#include "Supervisor.h"
#include "SkiSchool.h"

using namespace std;

int main(){

    SkiSchool school;
  
    school.registerInstructor("Instructor A");
    school.registerInstructor("Instructor B");
    school.registerInstructor("Instructor C");

    //fill all slots up to 5 students per instructor
    for(int i = 1; i <= 19; i++){
        string student = "Student " + to_string(i);
        school.registerStudent(student);
    }

    cout << "-----19 students are added-----" << endl;
    school.displayClasses();
    cout << endl;

    //remove a instructor here
    school.quitInstructor("Instructor C");

    cout << "-----Instructor C was removed-----" << endl;
    school.displayClasses();
    cout << endl;

    cout << "-----Try to add a student to Instructor C who already quit the school-----" << endl;
    school.registerStudent("Student 20 - Instructor C","Instructor D");


    // //remove 5 students
    // for(int i = 1; i <= 5; i++){
    //     string student = "Student " + to_string(i);
    //     school.quitStudent(student);
    // }

    // cout << "-----Top 5 students are removed-----" << endl;
    // school.displayClasses();
    // cout << endl;


    // school.registerStudent("Student 21 - Instructor A","Instructor A");
    // school.registerStudent("Student 22 - Instructor A","Instructor A");
    // school.registerStudent("Student 23 - Instructor A","Instructor A");
    // cout << "-----3 new students requested for Instructor A-----" << endl;
    // school.displayClasses();
    // cout << endl;

    // //add 10 more students
    // for(int i = 24; i <= 33; i++){
    //     string student = "Student " + to_string(i);
    //     school.registerStudent(student);
    // }
    // cout << "-----10 new students are added from 24 through 33-----" << endl;
    // school.displayClasses();
    // cout << endl;

    // cout << "-----Try to add a student to Instructor D who already quit the school-----" << endl;
    // school.registerStudent("Student 34 - Instructor D","Instructor D");
}