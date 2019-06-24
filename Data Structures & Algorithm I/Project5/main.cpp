
#include <iostream>
#include <cstdlib>
#include <string>
#include "Set.h"
#include "Data.h"
#include "Pikachu.h"

using namespace std;

int main(){

    cout << "*********************************************************" << endl;
    cout << "--Instructions on how to execute this program--" << endl;
    cout << "--Compilation Instruction: use the below command to compile this program--" << endl;
    cout << "Change a directory path to HW5 directory and execute the code below--" << endl;
    cout << "g++ -g -std=c++11 ./main.cpp ./Pikachu.cpp -o ./main" << endl;
    cout << "Once the above code compiles the program, execute commend \"./main\" to run this program" << endl;
    cout << "*********************************************************" << endl;

    cout << endl << endl;

    cout << "*********************************************************" << endl;
    cout << "--Demonstration 1: Key increments in a set--" << endl;
    cout << "*********************************************************" << endl;
    //demo set
    Set<Data<string>> demoSet1;
    //first templated object
    Data<string> strData1("Hello World");
    //second templated object with the same value
    Data<string> strData2("I love programming");
    demoSet1.Insert(strData1);
    demoSet1.Insert(strData2);
    cout << "--Show 2 separate objects--" << endl;
    demoSet1.PrintAll();
    cout << endl;

    //Third duplicate object with the same value
    //[1 Point] Adding an item that already exists in the set
    Data<string> strData3("I love programming");
    demoSet1.Insert(strData3);
    cout << "--Show that duplicate objects are incremented--" << endl;
    demoSet1.PrintAll();

    cout << endl << endl;

    cout << "*********************************************************" << endl;
    cout << "--Demonstration 2: Inserting integer objects into a set--" << endl;
    cout << "*********************************************************" << endl;

    //create random integer values and insert into a custom set
    Set<Data<int>> demoSet2;
    for(int i = 0; i < 200; i++){
        int val = rand()%15+1;//i%(rand()%11+1);
        Data<int> intData(val);
        demoSet2.Insert(intData);
    }

    cout << endl;

    //[1 Point] Print out everything that is in the set(key + counter)
    cout << "--Print all items--" << endl;
    demoSet2.PrintAll();

    cout << endl;

    //[2 points] A way to display the top 3 keys in the structure
    cout << "--Print top n items--" << endl;
    demoSet2.PrintTopN(3);

    cout << endl;

    //[1 Point] A way to obtain the counter for a given key
    cout << "--Find data whose value = 2 --" << endl;
    int val = 2;
    Data<int> int_to_find(val);
    int i_counter = demoSet2.GetCounter(int_to_find);
    cout << "Counter for key " << val << " is " << i_counter << endl;


    cout << endl << endl;


    cout << "*********************************************************" << endl;
    cout << "--Demonstration 3: Inserting Pikachu objects into a set--" << endl;
    cout << "*********************************************************" << endl;

    Set<Data<Pikachu>> demoSet3;
    for(int i = 1; i <= 100; i++){
        int val = rand()%15+1;//i%(rand()%11+1)*100;
        Pikachu pk(val);
        Data<Pikachu> pikaData(pk);
        demoSet3.Insert(pikaData);
    }

    cout << endl;

    //[1 Point] Print out everything that is in the set(key + counter)
    cout << "--Print all items--" << endl;
    demoSet3.PrintAll();

    cout << endl;

    //[2 points] A way to display the top 3 keys in the structure
    cout << "--Print top n items--" << endl;
    demoSet3.PrintTopN(3);

    cout << endl;

    //[1 Point] A way to obtain the counter for a given key
    cout << "--Find Picachu whose remaining power is 500 --" << endl;
    int pika_remain_power = 500;
    Data<Pikachu> pikachu_to_find(pika_remain_power);
    int p_counter = demoSet3.GetCounter(pikachu_to_find);
    cout << "Counter for Pikachu whose remaining power is " << val << " is " << p_counter << endl;

    cout << endl << endl;

    cout << "*********************************************************" << endl;
    cout << "--Big O Analysis--" << endl;
    cout << "Please see the actual implementations in Set.cpp file" << endl;
    cout << "*********************************************************" << endl;

    cout << "--Insert--" << endl;
    cout << "find & insert functions of std::map<T,int> operate at O(log n) with red black tree implementation" << endl;
    cout << "Therefore, Set::Insert performs at O(log n) time complexity" << endl;

    cout << "--Top 3 Function--" << endl;
    cout << "The first iteration happens n times over the private member map m." << endl;
    cout << "Inside the linear loop, insert action into std::multimap takes place. Given the implementatin of multimap with red-black tree under the hood, insert operation is O(log n)" << endl;
    cout << "Therefore, the time complexity of Top 3 function is \"n\" times loop and \"log n\" times insertion = O(n log n)" << endl;

    cout << endl << endl;
}