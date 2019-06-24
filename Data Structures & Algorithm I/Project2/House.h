


#ifndef House_h
#define House_h

#include <iostream>
#include "Score.h"

class House{
    public:
        House();
        House(Score* score, double price, double milesFromMainCity, double squareFootage, double numOfRooms, double numOfBathrooms);
        ~House();

        bool operator>(const House& house);
        bool operator>=(const House& house);

        bool operator<(const House& house);
        bool operator<=(const House& house);

        double getPrice() const;
        double getMilesFromMainCity() const;
        double getSquareFootage() const;
        double getNumOfRooms() const;
        double getNumOfBathrooms() const;

        double getScore() const;

    private:
        Score* score;

        double price = 0;
        double milesFromMainCity = 0;
        double squareFootage = 0;
        double numOfRooms = 0;
        double numOfBathrooms = 0;
};

#endif