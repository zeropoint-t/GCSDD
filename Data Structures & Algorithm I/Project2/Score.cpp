#include "Score.h"

Score::Score(int price_w, int milesFromMainCity_w, int squareFootage_w, int numOfRooms_w, int numOfBathrooms_w)
:price_w(price_w), milesFromMainCity_w(milesFromMainCity_w), squareFootage_w(squareFootage_w), numOfRooms_w(numOfRooms_w), numOfBathrooms_w(numOfBathrooms_w)
{
}

void Score::setPrice(double price){
    //find max value
    if(this->max_price < price){
        this->max_price = price;
    }

    //find min value
    if(this->min_price == 0 || this->min_price > price){
        this->min_price = price;
    }
}
void Score::setMilesFromMainCity(int milesFromMainCity){
    //find max value
    if(this->max_MilesFromMainCity < milesFromMainCity){
        this->max_MilesFromMainCity = milesFromMainCity;
    }

    //find min value
    if(this->min_MilesFromMainCity == 0 || this->min_MilesFromMainCity > milesFromMainCity){
        this->min_MilesFromMainCity = milesFromMainCity;
    }
}
void Score::setSquareFootage(int quareFootage){
    //find max value
    if(this->max_SquareFootage < quareFootage){
        this->max_SquareFootage = quareFootage;
    }

    //find min value
    if(this->min_SquareFootage == 0 || this->min_SquareFootage > quareFootage){
        this->min_SquareFootage = quareFootage;
    }
}
void Score::setNumOfRooms(int numOfRooms){
    //find max value
    if(this->max_NumOfRooms < numOfRooms){
        this->max_NumOfRooms = numOfRooms;
    }

    //find min value
    if(this->min_NumOfRooms == 0 || this->min_NumOfRooms > numOfRooms){
        this->min_NumOfRooms = numOfRooms;
    }
}
void Score::setNumOfBathrooms(int numOfBathrooms){
    //find max value
    if(this->max_NumOfBathrooms < numOfBathrooms){
        this->max_NumOfBathrooms = numOfBathrooms;
    }

    //find min value
    if(this->min_NumOfBathrooms == 0 || this->min_NumOfBathrooms > numOfBathrooms){
        this->min_NumOfBathrooms = numOfBathrooms;
    }
}

int Score::getPriceWeight() const{
    return this->price_w;
}
int Score::getMilesFromMainCityWeight() const{
    return this->milesFromMainCity_w;
}
int Score::getSquareFootageWeight() const{
    return this->squareFootage_w;
}
int Score::getNumOfRoomsWeight() const{
    return this->numOfRooms_w;
}
int Score::getNumOfBathroomsWeight() const{
    return this->numOfBathrooms_w;
}

double Score::getMaxPrice() const{
    return this->max_price;
}
int Score::getMaxMilesFromMainCity() const{
    return this->max_MilesFromMainCity;
}
int Score::getMaxSquareFootage() const{
    return this->max_SquareFootage;
}
int Score::getMaxNumOfRooms() const{
    return this->max_NumOfRooms;
}
int Score::getMaxNumOfBathrooms() const{
    return this->max_NumOfBathrooms;
}

double Score::getMinPrice() const{
    return this->min_price;
}
int Score::getMinMilesFromMainCity() const{
    return this->min_MilesFromMainCity;
}
int Score::getMinSquareFootage() const{
    return this->min_SquareFootage;
}
int Score::getMinNumOfRooms() const{
    return this->min_NumOfRooms;
}
int Score::getMinNumOfBathrooms() const{
    return this->min_NumOfBathrooms;
}