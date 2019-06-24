

#include <iostream>
#include "LinkListInterface.h"
#include "DoublyLinkedList.h"
#include "SearchCriteria.h"
using namespace std;

/*
    This program allows you to enter property information such as price, square footage, 
    number of rooms of houses into a doubly linked list
    And it calculates ranking scores by taking weights of each criteria in a "Score" object.
    You can call sort function of the list to sort the list in asc/desc order by the score values.
    The list also allows you to search for all houses that meet search criteria specified in SearchCriteria object.
    The list sorts items using bubble sort and time complexity of bubble sort is O(n2)
*/
int main(int argc, const char * argv[]){

    //create a list
    DoublyLinkedList list;

    //specify weight for each property criteria
    //My priority is miles from main city since I like to be close to my work so assign 40%
    //Price and a size of the house is also important so assign 20% to each
    int price_weight = 20;
    int milesFromMainCity_weight = 40;
    int squareFootage_weight = 20;
    int numOfRooms_weight = 10;
    int numOfBathrooms_weight = 10;

    //Enter weight for the following metrics in interger that add up to 100
    //Score object maintains max/min of each criteria for score calculations
    Score score(price_weight, milesFromMainCity_weight, squareFootage_weight, numOfRooms_weight, numOfBathrooms_weight);

    //create 10 house objects
    House h1(&score,240000,8,1500,2,1);
    House h2(&score,300000,5,1200,3,2);
    House h3(&score,250000,4,1400,3,2);
    House h4(&score,320000,9,1750,3,2);
    House h5(&score,400000,7,2100,5,3);
    House h6(&score,350000,6,2200,4,2);
    House h7(&score,210000,4,1470,3,2);
    House h8(&score,325000,5,1550,4,2);
    House h9(&score,230000,12,2100,3,2);
    House h10(&score,220000,15,2050,3,1);

    //enter each house into a list
    list.insert(0,h1);
    list.insert(1,h2);
    list.insert(2,h3);
    list.insert(3,h4);
    list.insert(4,h5);
    list.insert(5,h6);
    list.insert(6,h7);
    list.insert(7,h8);
    list.insert(8,h9);
    list.insert(9,h10);

    cout << endl;

    //show current list contents from head to tail
    cout << "-----Show items as they were entered-----" << endl;
    list.displayForward();

    //sort the list in descending order by scores
    list.sort(desc);

    cout << endl;

    //show current list contents from head to tail
    cout << "-----Descending by Score-----" << endl;
    list.displayForward();

    cout << endl;

    //sort the list in ascending order by scores
    list.sort(asc);

    cout << endl;

    //show current list contents from head to tail
    cout << "-----Ascending by Score-----" << endl;
    list.displayForward();


    //let's search houses by some criteria
    //I want a house with a price between 320k & 380k and size is at least 1700
    SearchCriteria filter(380000,320000,0,0,0,1700,0,0,0,0);
    auto selections = list.serch(filter);

    cout << endl;
    //show search results
    cout << "-----Search returned these houses-----" << endl;
    selections.displayForward();

}