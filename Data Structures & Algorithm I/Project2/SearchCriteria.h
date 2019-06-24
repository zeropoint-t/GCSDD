
#ifndef SearchCriteria_h
#define SearchCriteria_h

#include <stdio.h>

class SearchCriteria{
    public:
        SearchCriteria(
            double max_price,
            double min_price,

            double max_MilesFromMainCity,
            double min_MilesFromMainCity,

            double max_SquareFootage,
            double min_SquareFootage,

            double max_NumOfRooms, 
            double min_NumOfRooms,

            double max_numOfBathrooms,
            double min_numOfBathrooms
        );
        ~SearchCriteria();

        double max_price;
        double min_price;

        double max_MilesFromMainCity;
        double min_MilesFromMainCity;

        double max_SquareFootage;
        double min_SquareFootage;

        double max_NumOfRooms; 
        double min_NumOfRooms;

        double max_numOfBathrooms;
        double min_numOfBathrooms;
};

#endif