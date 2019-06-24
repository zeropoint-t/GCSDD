// -------------------------------------Film.h----------------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/03/2019
// -----------------------------------------------------------------------------
// Film - inherits from media, adds additional functionality 
// -----------------------------------------------------------------------------
#ifndef Film_h
#define Film_h

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

#include "Media.h"

using namespace std;

class Film : public Media
{
private:
	char filmType;//genre: Comedy(F), Classic(C), Drama(D), etc
	string director;
	string title;
	int year;

public:
	//constructors & destructor
	Film(char mediaType, char storageType, int numStock, char filmType,
		string director, string title, int year);
	Film();
	~Film() override = default;;

	//getters
	char getFilmType() const;
	string getDirector() const;
	string getTitle() const;
	int getYear() const;

	//setters
	void setFilmType(char nFilmType);
	void setDirector(string nDirector);
	void setTitle(string nTitle);
	void setYear(int nYear);

	// virtual bool operator>(const Media&) const = 0;
	// virtual bool operator==(const Media&) const = 0;
	// virtual bool operator<(const Media&) const = 0;
	bool operator>(const Media&) const override;
	bool operator==(const Media&) const override;
	bool operator<(const Media&) const override;
	void printHeader() const override;
	void print() const override;
};

#endif
