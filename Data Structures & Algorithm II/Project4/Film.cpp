// -------------------------------------Film.cpp----------------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/03/2019
// -----------------------------------------------------------------------------
// Film - inherits from media, adds additional functionality
// -----------------------------------------------------------------------------
#include "Film.h"

Film::Film(char nMediaType, char nStorageType, int nNumStock, char nFilmType, string nDirector, string nTitle, int nYear) : Media(nMediaType, nStorageType, nNumStock)
{
	setFilmType(nFilmType);
	setDirector(nDirector);
	setTitle(nTitle);
	setYear(nYear);
}

Film::Film() {}

// Film::~Film()
// {

// }

//getters
char Film::getFilmType() const
{
	return filmType;
}
string Film::getDirector() const
{
	return director;
}
string Film::getTitle() const
{
	return title;
}
int Film::getYear() const
{
	return year;
}

//setters
void Film::setFilmType(char nFilmType)
{
	filmType = nFilmType;
}
void Film::setDirector(string nDirector)
{
	director = nDirector;
}
void Film::setTitle(string nTitle)
{
	title = nTitle;
}
void Film::setYear(int nYear)
{
	year = nYear;
}



// -----------operator==(const Comedy& rhs)--------------------------------------
// check if lhs comedy is identical to rhs comedy by title and year
// -----------------------------------------------------------------------------
bool Film::operator==(const Media& rhs) const {
	return true;
}

// -----------operator<(const Comedy& rhs)--------------------------------------
// check if lhs comedy comes before rhs comedy by title, then by year
// -----------------------------------------------------------------------------
bool Film::operator<(const Media& rhs) const
{
	return false;
}
// -----------operator>(const Comedy& rhs)--------------------------------------
// check if lhs comedy comes before rhs comedy by title, then by year
// -----------------------------------------------------------------------------
bool Film::operator>(const Media& rhs) const
{
	return false;
}

void Film::printHeader() const
{
	cout << left << setw(4) << "<IN>" 
	<< setw(4) << "<OUT>" 
	<< setw(30) << "<DVD - TITLE>" 
	<< setw(20) << "<DIRECTOR>" 
	<< setw(10) << "<YEAR>" 
	<< endl;
	cout <<
		"------------------------------------------------------------------"
		<< endl;
}

void Film::print() const
{
	string title = getTitle() ;
	if(title.length() > 28) title.resize(28);
	cout << " " 
	<< setw(4) << getNumStock() 
	<< setw(4) << getMaxStock() - getNumStock()
	<< setw(30) << title 
	<< setw(20) << getDirector() 
	<< setw(10) << getYear();
}
