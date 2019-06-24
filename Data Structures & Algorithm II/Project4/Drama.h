// -------------------------------------Drama.h---------------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/03/2019
// -----------------------------------------------------------------------------
// Drama and Comedy share same methods and members
// -----------------------------------------------------------------------------

#ifndef Drama_h
#define Drama_h

#include "Film.h"
#include <string>

using namespace std;

class Drama : public Film
{
public:
	//constructors & destructor
	Drama(char nMediaType, char nStorageType, int nNumStock, char nFilmType,
		string nDirector, string nTitle, int nYear);
		
	Drama();

	~Drama() override = default;;

	bool operator>(const Media&) const override;
	bool operator==(const Media&) const override;
	bool operator<(const Media&) const override;
	//void print() const override;
};

#endif
