
#include "Supervisor.h"

void Supervisor::addInstructor(string&& instructorName){
    if(instructorName.size() == 0)
        throw runtime_error("Instructor name can not be empty");
    if(instructorName.size() > 50)
        throw runtime_error("Instructor name is too long");

    auto it = find_if(instructors.begin(), instructors.end(), [&instructorName](const Instructor& obj){return obj.getName() == instructorName;});
    if(it != instructors.end())//students exists
        throw runtime_error("Instructor already exists");

    Instructor newInstructor(instructorName);
    instructors.push_back(newInstructor);
}

void Supervisor::addStudent(string&& studentName){
    int min = this->instructors.begin()->getClassSize();
	int max = min;
	for(auto it = instructors.begin() + 1; it != instructors.end(); it++){
		if(it->getClassSize() > max) max = it->getClassSize();
		if(it->getClassSize() < min) min = it->getClassSize(); 
	}

    if(min >= 5 && min < 8){
        //add to instructor with max students
        for(auto inst = instructors.begin(); inst != instructors.end(); inst++){
            if(inst->getClassSize() >= 8)
                continue;

            //add student to the first instructor who has less than 8 students
            if(inst->getClassSize() >= 5 && inst->getClassSize() < 8){
                inst->addStudent(move(studentName));
                break;
            }
        }
    }else{
        //add to instructor with min students
        for(auto inst = instructors.begin(); inst != instructors.end(); inst++){
            //add student to the first instructor who has minimum number of students
            if(inst->getClassSize() == min){
                inst->addStudent(move(studentName));
                break;
            }
        }
    }
}
void Supervisor::removeInstructor(string&& instructorName){
    if(instructorName.size() == 0)
        throw runtime_error("Instructor name can not be empty");
    if(instructorName.size() > 50)
        throw runtime_error("Instructor name is too long");

    auto it = find_if(instructors.begin(), instructors.end(), [&instructorName](const Instructor& obj){return obj.getName() == instructorName;});
    if(it == instructors.end())//students exists
        throw runtime_error("Instructor does not exists");

    Instructor instructorToRemove = *it;
    instructors.erase(it);

    for(auto student = instructorToRemove.begin(); student != instructorToRemove.end(); student++){
        this->addStudent(student->getName());
    }    
}

void Supervisor::addStudent(string&& studentName, string&& instructorName){
    if(studentName.size() == 0)
        throw runtime_error("Student name can not be empty");
    if(studentName.size() > 50)
        throw runtime_error("Student name is too long");

    if(instructorName.size() == 0)
        throw runtime_error("Instructor name can not be empty");
    if(instructorName.size() > 50)
        throw runtime_error("Instructor name is too long");

    //“Find” a teacher
    //[2 Points] Given the teacher’s name
    auto inst = find_if(instructors.begin(), instructors.end(), [&instructorName](const Instructor& obj){return obj.getName() == instructorName;});
    if(inst == instructors.end())
        throw runtime_error("Instructor does not exists");

    inst->addStudent(move(studentName));
}

void Supervisor::removeStudent(string&& studentName){
    if(studentName.size() == 0)
        throw runtime_error("Student name can not be empty");
    if(studentName.size() > 50)
        throw runtime_error("Student name is too long");

    //“Find” a teacher
    //[2 Points] Given the student’s name
    for(auto inst = instructors.begin(); inst != instructors.end(); inst++){
        auto student = find_if(inst->begin(), inst->end(), [&studentName](const Student& obj){return obj.getName() == studentName;});
        if(student != inst->end()){
            inst->removeStudent(student->getName());
            break;
        }
    }
}

void Supervisor::displayClasses(){
    for(auto inst = instructors.begin(); inst != instructors.end(); inst++){
        inst->displayClass();
    }
}