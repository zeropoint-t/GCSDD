// -------------------------------------Classic.h-------------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/03/2019
// -----------------------------------------------------------------------------
// adds main actor, and month 
// -----------------------------------------------------------------------------

#ifndef Classic_h
#define Classic_h

#include "Film.h"
#include <string>
using namespace std;

class Classic : public Film
{
private:
	string mainActorFirst;
	string mainActorLast;
	int month;

public:
	//constructors & destructor
	Classic(char nMediaType, char nStorageType, int nNumStock,
		char nFilmType, string nDirector, string nTitle, int nYear,
		string nMainActorFirst, string nMainActorLast, int nMonth);

	Classic();

	~Classic() override = default;

	//getters
	string getMainActorFirst() const;
	string getMainActorLast() const;
	int getMonth() const;

	//setters
	void setMainActorFirst(string nMainActorFirst);
	void setMainActorLast(string nMainActorLast);
	void setMonth(int nMonth);

	bool operator>(const Media&) const override;
	bool operator==(const Media&) const override;
	bool operator<(const Media&) const override;
	void printHeader() const override;
	void print() const override;
};

#endif
