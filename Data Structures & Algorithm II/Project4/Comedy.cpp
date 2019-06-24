// -------------------------------------Comedy.cpp------------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/03/2019
// -----------------------------------------------------------------------------
// Drama and Comedy share same methods and members
// -----------------------------------------------------------------------------

#include "Comedy.h"

// bool operator>(Media &m1, Media &m2)  // <--- why friend?
// {
//     const Comedy& c1 = static_cast<const Comedy&>(m1);
//     const Comedy& c2 = static_cast<const Comedy&>(m2);

//     if (c1.getTitle() < c2.getTitle())
//         return true;
//     else if (c1.getTitle() == c2.getTitle()){
//         return (c1.getYear() < c2.getYear());
//     }
//     else
//         return false;
// }

Comedy::Comedy(char nMediaType, char nStorageType, int nNumStock, char nFilmType,
	string nDirector, string nTitle, int nYear) : Film(nMediaType, nStorageType, nNumStock, nFilmType, nDirector, nTitle, nYear)
{

}

Comedy::Comedy(string nTitle, int nYear)
{
	setTitle(nTitle);
	setYear(nYear);
}

Comedy::Comedy()
{

}

// Comedy::~Comedy()
// {

// }

// -----------operator==(const Comedy& rhs)--------------------------------------
// check if lhs comedy is identical to rhs comedy by title and year
// -----------------------------------------------------------------------------
bool Comedy::operator==(const Media& rhs) const {
	const Comedy& c = static_cast<const Comedy&>(rhs);
	// cout << "lhs " << getTitle() << " " << getYear() << endl;
	// cout << "rhs " << c.getTitle() << " " << c.getYear() << endl;
	return (getTitle() == c.getTitle() && getYear() == c.getYear());
}


// -----------operator<(const Comedy& rhs)--------------------------------------
// check if lhs comedy comes before rhs comedy by title, then by year
// -----------------------------------------------------------------------------
bool Comedy::operator<(const Media& rhs) const
{
	const Comedy& c = static_cast<const Comedy&>(rhs);

	if (getTitle() < c.getTitle())
		return true;
	else if (getTitle() == c.getTitle()) {
		return (getYear() < c.getYear());
	}
	// else

	return false;
}
// -----------operator>(const Comedy& rhs)--------------------------------------
// check if lhs comedy comes before rhs comedy by title, then by year
// -----------------------------------------------------------------------------
bool Comedy::operator>(const Media& rhs) const
{
	const Comedy& c = static_cast<const Comedy&>(rhs);
	if (getTitle() > c.getTitle())
		return true;
	else if (getTitle() == c.getTitle()) {
		return (getYear() > c.getYear());
	}

	return false;
}
