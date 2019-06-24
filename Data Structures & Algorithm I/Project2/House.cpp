
#include <iostream>
#include "House.h"
using namespace std;

House::House()
    :score(nullptr),price(0),milesFromMainCity(0), squareFootage(0), numOfRooms(0), numOfBathrooms(0)
{}

House::House(Score* score, double price, double milesFromMainCity, double squareFootage, double numOfRooms, double numOfBathrooms)
    :score(score),price(price),milesFromMainCity(milesFromMainCity), squareFootage(squareFootage), numOfRooms(numOfRooms), numOfBathrooms(numOfBathrooms)
{
    score->setPrice(price);
    score->setMilesFromMainCity(milesFromMainCity);
    score->setSquareFootage(squareFootage);
    score->setNumOfRooms(numOfRooms);
    score->setNumOfBathrooms(numOfBathrooms);
}

House::~House(){
    
}

double House::getScore() const{
    //find total weights by adding them all up
    double total_w = score->getPriceWeight() + score->getMilesFromMainCityWeight() + score->getSquareFootageWeight() + score->getNumOfRoomsWeight() + score->getNumOfBathroomsWeight();

    double price_w = score->getMaxPrice()-score->getMinPrice() == 0 ? 0 : (1 - (((this->price - score->getMinPrice())/(score->getMaxPrice()-score->getMinPrice())))) * (score->getPriceWeight() / total_w);
    double milesFromMainCity_w = score->getMaxMilesFromMainCity()-score->getMinMilesFromMainCity() == 0 ? 0 : (1 - (((this->milesFromMainCity - score->getMinMilesFromMainCity())/(score->getMaxMilesFromMainCity()-score->getMinMilesFromMainCity())))) * (score->getMilesFromMainCityWeight()/total_w);
    double squareFootage_w = score->getMaxSquareFootage()-score->getMinSquareFootage() == 0 ? 0 : ((this->squareFootage - score->getMinSquareFootage())/(score->getMaxSquareFootage()-score->getMinSquareFootage()))*(score->getSquareFootageWeight()/total_w);
    double numOfRooms_w = score->getMaxNumOfRooms()-score->getMinNumOfRooms() == 0 ? 0 :((this->numOfRooms - score->getMinNumOfRooms())/(score->getMaxNumOfRooms()-score->getMinNumOfRooms()))*(score->getNumOfRoomsWeight()/total_w);
    double numOfBathrooms_w = score->getMaxNumOfBathrooms()-score->getMinNumOfBathrooms() == 0 ? 0 : ((this->numOfBathrooms - score->getMinNumOfBathrooms())/(score->getMaxNumOfBathrooms()-score->getMinNumOfBathrooms()))*(score->getNumOfBathroomsWeight()/total_w);
    return price_w + milesFromMainCity_w + squareFootage_w + numOfRooms_w + numOfBathrooms_w;
}

bool House::operator > (const House& house){
    return this->getScore() > house.getScore();
}
bool House::operator >= (const House& house){
    return this->getScore() >= house.getScore();
}
bool House::operator < (const House& house){
    return this->getScore() < house.getScore();
}
bool House::operator <= (const House& house){
    return this->getScore() <= house.getScore();
}

double House::getPrice() const{
    return this->price;
}

double House::getMilesFromMainCity() const{
    return this->milesFromMainCity;
}

double House::getSquareFootage() const{
    return this->squareFootage;
}

double House::getNumOfRooms() const{
    return this->numOfRooms;
}

double House::getNumOfBathrooms() const{
    return this->numOfBathrooms;
}