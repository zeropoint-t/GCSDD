// --------------------------------------InventoryMgr.h-------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/03/2019
// -----------------------------------------------------------------------------
// InventoryMgr - Header file - in charge of inventory related tasks
// -----------------------------------------------------------------------------
// Notes on specifications, special algorithms, and assumptions.
//------------------------------------------------------------------------------

#ifndef InventoryMgr_h
#define InventoryMgr_h

#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>//typeid

#include "Comedy.h"
#include "Drama.h"
#include "Classic.h"
#include "MediaTree.h"
#include "MediaNode.h"
#include "Media.h"

using namespace std;

class InventoryMgr
{
private:
	int stockCount;
	vector <MediaTree*> MediaTreesVec;

	void buildInv(string filePath);
	/*
		buildInv(){
		Invariance - check for false input
		*cycle through infile
		*get genre(film type)
		*check if genre is legitamate
		*parse based on genre -> create key/string
		*check if film in system if YES update stock ELSE
		*call appropriate constructor(FUNNY(FILM INFO) CLASSIC(FILM INFO) DRAMA(FILM INFO, key))
	*/
	MediaTree* findMediaTree(const Media*);

public:
	//constructors & destructor
	InventoryMgr();
	InventoryMgr(string);
	~InventoryMgr();
	bool insert(Media*);
	bool incInv(Media&);//increment stock count
	bool decInv(Media&);//decrement stock count
	int getStock(Media&);//return s
	void printInv();//print inventory
	Media* getMedia(Media&);
};

#endif
