

#include "SearchCriteria.h"

SearchCriteria::SearchCriteria(
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
):max_price(max_price),
        min_price(min_price),

        max_MilesFromMainCity(max_MilesFromMainCity),
        min_MilesFromMainCity(min_MilesFromMainCity),

        max_SquareFootage(max_SquareFootage),
        min_SquareFootage(min_SquareFootage),

        max_NumOfRooms(max_NumOfRooms), 
        min_NumOfRooms(min_NumOfRooms),

        max_numOfBathrooms(max_numOfBathrooms),
        min_numOfBathrooms(min_numOfBathrooms)
{

}

SearchCriteria::~SearchCriteria(){

}