#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "DataManager.h"
#include "DataPacket.h"
#include "MACD.h"

using namespace std;

int main(){

        int i;
        for(int i = 0; i < 10; i++){}
        int x = i;
        cout << x << endl;

        cout <<"----------------------------------"<< endl << endl;

        //**************************************************************************************************
        //[1 Point] Functional Positive Testing: Create an instance of each Child and Grandchild objects 
        //given valid values (passes invariant check)
        //**************************************************************************************************
        cout <<"--Functional Positive Testing--"<< endl;
        //Child class 1 - Simple Moving Average
        try{
            SMA<double> sma(12);
            cout << "SMA passed" << endl;
        }
        catch (const std::out_of_range& oor) {
            std::cerr << oor.what() << '\n';
        }
        //Child class 2 - Exponential Moving Average
        try{
            EMA<double> ema(6);
            cout << "EMA passed" << endl;
        }
        catch (const std::out_of_range& oor) {
            std::cerr << oor.what() << '\n';
        }
        //Grand Child class - Moving Average Convergence Divergence
        try{        
            MACD<double> macd(6,12);
            cout << "MACD passed" << endl;
        }
        catch (const std::out_of_range& oor) {
            std::cerr << oor.what() << '\n';
        }

        cout << endl;

        //**************************************************************************************************
        //[1 Point] Functional Negative Testing: Attempt to create an instance of each Child and 
        //Grandchild objects given invalid values (does not pass invariant check – what should it do?)
        //**************************************************************************************************
        cout <<"--Functional Negative Testing--"<< endl;
        //Child class 1 - Simple Moving Average
        try{
            SMA<double> sma(-12);
        }
        catch (const std::out_of_range& oor) {
            std::cerr << oor.what() << '\n';
        }
        //Child class 2 - Exponential Moving Average
        try{
            EMA<double> ema(-6);
        }
        catch (const std::out_of_range& oor) {
            std::cerr << oor.what() << '\n';
        }
        //Grand Child class - Moving Average Convergence Divergence
        try{       
            MACD<double> macd(-6,-12);
        }
        catch (const std::out_of_range& oor) {
            std::cerr << oor.what() << '\n';
        }

        cout << endl;

        //[6 Points] Driver should call each public method (including any public
        //getter/setter)
        cout << "--public function tests--" << endl;
        DataPacket<double> packet;
        packet.utc_timestamp = time(0);
        packet.open += rand()%3;
        packet.high += rand()%3;
        packet.low += rand()%3;
        packet.close += rand()%3;
        packet.volume += rand()%3;

        //functionl test for SMA
        SMA<double> sma1(6);
        SMA<double> sma2 = sma1;
        sma2.update(packet);
        int period = sma2.getPeriod();
        sma2.setPeriod(20);

        //functionl test for EMA
        EMA<double> ema1(6);
        EMA<double> ema2 = ema1;
        ema2.update(packet);
        period = ema2.getPeriod();
        ema2.setPeriod(20);

        //functionl test for MACD
        MACD<double> macd1(6,12);
        MACD<double> macd2 = macd1;
        macd2.update(packet);
        macd2.setPeriod(12,24);

        cout << endl;
        cout << "--friend class demostration SMA is a friend of EMA--" << endl;
        sma1.ShowEMAValue(ema2);

        cout << endl;
        //*******************************************
        //Program Demonstration
        //*******************************************
        DataManager<double> dm;
        MACD<double> macd(6,12);
        dm += &macd;

        cout <<"--Show observers info--"<< endl;
        dm.displayObservers(dm.begin(),dm.end());

        cout << endl;
        cout << "--program demostration--" << endl;
        std::chrono::milliseconds timespan(1000); // or whatever

        DataPacket<double> p;
        p.utc_timestamp = time(0);
        p.open = rand()%35+15;
        p.high = rand()%35+25;
        p.low = rand()%15+1;
        p.close = rand()%50+30;
        p.volume = rand()%35+15;

        //simulate up trend
        for(int i = 0; i < 30; i++){
            DataPacket<double> packet(p);
            packet.utc_timestamp = time(0);
            packet.open += rand()%3;
            packet.high += rand()%3;
            packet.low += rand()%3;
            packet.close += rand()%3;
            packet.volume += rand()%3;

            p = packet;

            packet.display();
            dm.dataChange(packet);
            std::this_thread::sleep_for(timespan);

            cout << endl;
        }

        //simulate down trend
        for(int i = 0; i < 30; i++){
            DataPacket<double> packet(p);
            packet.utc_timestamp = time(0);
            packet.open -= rand()%3;
            packet.high -= rand()%3;
            packet.low -= rand()%3;
            packet.close -= rand()%3;
            packet.volume -= rand()%3;

            p = packet;
            
            dm.dataChange(packet);
            std::this_thread::sleep_for(timespan);

            cout << endl;
        }

    return 0;
}