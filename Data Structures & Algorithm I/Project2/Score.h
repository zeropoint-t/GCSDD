
#ifndef Score_h
#define Score_h

class Score {

    public:
        Score(int price_w, int milesFromMainCity_w, int squareFootage_w, int numOfRooms_w, int numOfBathrooms_w);

        void setPrice(double price);
        void setMilesFromMainCity(int milesFromMainCity);
        void setSquareFootage(int quareFootage);
        void setNumOfRooms(int numOfRooms);
        void setNumOfBathrooms(int numOfBathrooms);

        int getPriceWeight() const;
        int getMilesFromMainCityWeight() const;
        int getSquareFootageWeight() const;
        int getNumOfRoomsWeight() const;
        int getNumOfBathroomsWeight() const;

        double getMaxPrice() const;
        int getMaxMilesFromMainCity() const;
        int getMaxSquareFootage() const;
        int getMaxNumOfRooms() const;
        int getMaxNumOfBathrooms() const;

        double getMinPrice() const;
        int getMinMilesFromMainCity() const;
        int getMinSquareFootage() const;
        int getMinNumOfRooms() const;
        int getMinNumOfBathrooms() const;

    private:
        int price_w = 20;
        int milesFromMainCity_w = 20;
        int squareFootage_w = 20;
        int numOfRooms_w = 20;
        int numOfBathrooms_w = 20;

        double max_price = 0;
        int max_MilesFromMainCity = 0;
        int max_SquareFootage = 0;
        int max_NumOfRooms = 0;
        int max_NumOfBathrooms = 0;

        double min_price = 0;
        int min_MilesFromMainCity = 0;
        int min_SquareFootage = 0;
        int min_NumOfRooms = 0;
        int min_NumOfBathrooms = 0;
};

#endif