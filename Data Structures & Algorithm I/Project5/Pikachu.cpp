

#include "Pikachu.h"

ostream& operator<<(ostream &outStream, const Pikachu &pk)
{
	outStream << "Remaining Power: " << pk.getRemainingPower();
	return outStream;
}

Pikachu::Pikachu():remainingPower(0){};
Pikachu::Pikachu(int pw):remainingPower(pw){};

int Pikachu::getRemainingPower() const{
    return remainingPower;
}
void Pikachu::setRemainingPower(int pw){
    this->remainingPower = pw;
}

bool Pikachu::operator==(const Pikachu& pk){
    return this->remainingPower == pk.getRemainingPower();
}
bool Pikachu::operator>(const Pikachu& pk){
    return this->remainingPower > pk.getRemainingPower();
}
bool Pikachu::operator<(const Pikachu& pk){
    return this->remainingPower < pk.getRemainingPower();
}