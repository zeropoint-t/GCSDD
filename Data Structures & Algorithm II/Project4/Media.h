// -------------------------------------Media.h---------------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/03/2019
// -----------------------------------------------------------------------------
// Media - basic inventory item that all other films and genres inherit form
// -----------------------------------------------------------------------------

#ifndef Media_h
#define Media_h

#include <string>
#include <vector>

using namespace std;

class Media
{
private:
	char mediaType;//movie, music, etc
	char storageType;//DVD, CD, etc
	int numStock=0;//stock count
	int maxStock=0;//tracks max amount ever stored

public:
	//constructors & destructor
	Media(char mediaType, char storageType, int numStock);
	Media();
	virtual ~Media();

	//getters/setters
	char getMediaType() const;
	char getStorageType() const;
	int getNumStock() const;
	int getMaxStock() const;

	void setMaxStock(const int);//updates max amount of media
	void setMediaType(char nStorageType);
	void setStorageType(char nStorageType);
	void setNumStock(int nNumStock);

	// virtual string getKey() const = 0;//unique key

	//comparision overloads
	virtual bool operator>(const Media&) const = 0;
	virtual bool operator==(const Media&) const = 0;
	virtual bool operator<(const Media&) const = 0;
	virtual void printHeader() const = 0;
	virtual void print() const = 0;
	void borrowMedia();//increase stock by 1
	void returnMedia();//increase stock by 1
};

#endif
