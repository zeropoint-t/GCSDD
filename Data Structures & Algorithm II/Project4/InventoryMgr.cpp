// --------------------------------------InventoryMgr.cpp-------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/03/2019
// -----------------------------------------------------------------------------
// InventoryMgr - Header file - in charge of inventory related tasks
// -----------------------------------------------------------------------------
// Notes on specifications, special algorithms, and assumptions.
//------------------------------------------------------------------------------
#include "InventoryMgr.h"

//constructors/destructor
InventoryMgr::InventoryMgr() {
	/*for (vector<MediaTree*>::iterator it = MediaTreesVec.begin(); it != MediaTreesVec.end(); it++) {
		*it = new MediaTree();
	}*/
}

InventoryMgr::InventoryMgr(string infile) {
	this->buildInv(infile);
}

InventoryMgr::~InventoryMgr() {
	for(auto it = MediaTreesVec.begin(); it != MediaTreesVec.end(); it++)
	{
		delete *it;
	}
}

MediaTree* InventoryMgr::findMediaTree(const Media* med)
{
	MediaTree* temp = nullptr; 
	MediaTree* current = nullptr;
	bool classfound = false;
	for (vector<MediaTree*>::iterator it = MediaTreesVec.begin(); it != MediaTreesVec.end(); it++) {

		current = *it;
		Media* m = current->getRoot()->getData();
		const char * type = typeid(*m).name();

		if (typeid(*m).name() == typeid(*med).name()) { // no trees 
			return *it;
		}
	}

	return nullptr;
}

bool InventoryMgr::insert(Media * med) {	

	Film * film = dynamic_cast<Film*>(med);

	if(film == nullptr)
		return false;

	MediaTree* temp = nullptr;

	temp = findMediaTree(med);

	if(temp == nullptr){

		if(film->getFilmType() == 'F')
		{
			MediaTree * comedyTree = new MediaTree();

			comedyTree->insert(med);

			MediaTreesVec.push_back(comedyTree);

			return true;
		} else if (film->getFilmType() == 'C') 
		{
			MediaTree * classicTree = new MediaTree();

			classicTree->insert(med);

			MediaTreesVec.push_back(classicTree);

			return true;
		} else if (film->getFilmType() == 'D') 
		{
			MediaTree * dramaTree = new MediaTree();

			dramaTree->insert(med);

			MediaTreesVec.push_back(dramaTree);

			return true;
		}
	}else{

		if (film->getFilmType() == 'F') {			
			if (temp->find(temp->getRoot(), med)) {
				med->print();
				cout << endl;
				cout << "The film already exists in the DB" << endl;
				temp->insert(med);
			}
			else {
				temp->insert(med);
			}
			return true;
		}

		if (film->getFilmType() == 'C') {
			if (temp->find(temp->getRoot(), med)) {
				med->print();
				cout << endl;
				cout << "The film already exists in the DB" << endl;
				temp->insert(med);
			}
			else {
				temp->insert(med);
			}
			return true;
		}

		if (film->getFilmType() == 'D') {
			if (temp->find(temp->getRoot(), med)) {
				med->print();
				cout << endl;
				cout << "The film already exists in the DB" << endl;
				temp->insert(med);
			}
			else {
				temp->insert(med);
			}
			return true;
		}
	}

	return false;
}

void InventoryMgr::printInv() {
	for (vector<MediaTree*>::iterator it = MediaTreesVec.begin(); it != MediaTreesVec.end(); it++) {
		
		MediaTree * temp = *it;
		Media* m = temp->getRoot()->getData();
		const char * type = typeid(*m).name();
		string typestr(type);
		typestr.erase(remove_if(typestr.begin(), typestr.end(), [](char c) { return !isalpha(c); } ), typestr.end());//remove numbers from class name 
		
		cout << endl;
		cout << typestr << endl;
		temp->getRoot()->getData()->printHeader();
		temp->inOrderTraversal(temp->getRoot());
	}
}

void InventoryMgr::buildInv(string filePath) {
	ifstream file(filePath);
	if (!file)
	{
		cout << "File could not be opened." << endl;
		return;
	}

	while (!file.eof()) {
		int stock=0;
		string director;
		string title;
		int year;
		int month;
		string mainActorFirst;
		string mainActorLast;
		string s;
		getline(file, s);

		istringstream iss(s);

		if (!s.empty()) {
			char filmType = s.at(0);
			if (filmType == 'C' || filmType == 'F' || filmType == 'D') {
				if (filmType == 'F' || filmType == 'D') {
					string delimiter = ", ";
					size_t pos = 0;
					string token;
					int i = 1;
					while ((pos = s.find(delimiter)) != std::string::npos) {
						token = s.substr(0, pos);
						if (i == 2) {
							stringstream stockss(token);
							stockss >> stock;
						}
						if (i == 3) {
							director = token;
						}
						if (i == 4) {
							title = token;
						}

						s.erase(0, pos + delimiter.length());
						i++;
					}
					stringstream yearss(s);
					yearss >> year;
					if (filmType == 'F') {
						Media * med = new Comedy('F', 'D', stock, 'F', director, title, year);
						this->insert(med);
					}
					else {
						Media * med = new Drama('F', 'D', stock, 'D', director, title, year);
						this->insert(med);
					}
				}
				else {
					string delimiter = ", ";
					size_t pos = 0;
					string token;
					int i = 1;
					while ((pos = s.find(delimiter)) != std::string::npos) {
						token = s.substr(0, pos);
						if (i == 2) {
							stringstream stockss(token);
							stockss >> stock;
						}
						if (i == 3) {
							director = token;
						}
						if (i == 4) {
							title = token;
						}
						s.erase(0, pos + delimiter.length());
						i++;
					}

					string delimiter2 = " ";
					size_t pos2 = 0;
					string token2;
					while ((pos2 = s.find(delimiter2)) != std::string::npos) {
						token2 = s.substr(0, pos2);
						//cout << i << endl;
						if (i == 5) {
							mainActorFirst = token2;
							//cout << "Main Actor first is " << mainActorFirst << endl;
						}
						if (i == 6) {
							mainActorLast = token2;
							//cout << "Main Actor Last is " << mainActorLast << endl;
						}
						if (i == 7) {
							stringstream monthss(token2);
							monthss >> month;
							//cout << "month is " << month << endl;
						}
						//cout << token2 << std::endl;
						s.erase(0, pos2 + delimiter2.length());

						i++;
					}
					stringstream yearss(s);
					yearss >> year;
					//cout << "Year is " << year << endl;

					Media * med = new Classic('F', 'D', stock, 'C', director, title, year, mainActorFirst, mainActorLast, month);
					this->insert(med);
				}
			}

			else {
				cout << "ERROR: Genre: " << s.at(0) << " not found" << endl;
			}
		}
	}
}

//increment stock count
bool InventoryMgr::incInv(Media& med){
	med.setNumStock(med.getNumStock()+1);
	return true;
};

//decrement stock count
bool InventoryMgr::decInv(Media& med){
	Film& f = dynamic_cast<Film&>(med);
	int newStock = med.getNumStock()-1;
	med.setNumStock(newStock);
	return true;
};

//return stock on hand
int InventoryMgr::getStock(Media& med)
{
	MediaTree* temp = nullptr;
	temp = findMediaTree(&med);
	int stock = temp->retrieveStock(temp->getRoot(), &med);
	return stock;	
};

//return Media pointer from inventory
Media* InventoryMgr::getMedia(Media& med)
{
	MediaTree* temp = nullptr;
	temp = findMediaTree(&med);
	Media* pMed = temp->retrieveMedia(temp->getRoot(), &med);
	return pMed;
}
