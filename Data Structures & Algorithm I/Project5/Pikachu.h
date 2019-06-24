
#ifndef Pikachu_h
#define Pikachu_h

#include <iostream>
using namespace std;

class Pikachu{
    friend ostream& operator<<(ostream &outStream, const Pikachu &pk);

    public:
        Pikachu();
        Pikachu(int pw);

        int getRemainingPower() const;
        void setRemainingPower(int pw);

        bool operator==(const Pikachu& pk);
        bool operator>(const Pikachu& pk);
        bool operator<(const Pikachu& pk);

    private:
        int remainingPower = 0;
};

#endif