    #include "SkiSchool.h"

    SkiSchool::SkiSchool(){}
    SkiSchool::~SkiSchool(){}

	void SkiSchool::registerInstructor(string instructorName){
		try{
			this->supervisor.addInstructor(move(instructorName));
		}catch(std::runtime_error err){
			cout << err.what() << endl;
		}
	}
	void SkiSchool::registerStudent(string studentName){
		try{
			this->supervisor.addStudent(move(studentName));
		}
		catch(std::runtime_error err){
			cout << err.what() << endl;
		}
	}
    void SkiSchool::registerStudent(string studentName, string instructorName){
		try{
			this->supervisor.addStudent(move(studentName),move(instructorName));
		}
		catch(std::runtime_error err){
			cout << err.what() << endl;
		}
	}
	void SkiSchool::quitInstructor(string instructorName){
		try{
			this->supervisor.removeInstructor(move(instructorName));
		}
		catch(std::runtime_error err){
			cout << err.what() << endl;
		}
	}
	void SkiSchool::quitStudent(string studentName){
		try{
			this->supervisor.removeStudent(move(studentName));
		}
		catch(std::runtime_error err){
			cout << err.what() << endl;
		}
	}
	void SkiSchool::displayClasses() noexcept{
		this->supervisor.displayClasses();
	}