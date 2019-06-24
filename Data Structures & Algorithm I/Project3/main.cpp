

#include <iostream>
#include "SinglyLinkedList.h"

using namespace std;

void print(SinglyLinkedList list){
    list.print();
}

int main(int argc, const char* argv[]){

    //create a singly linked list object
    SinglyLinkedList list;

    //insert 10 items in an order
    for(int i = 0; i < 15; i++){
        string item = "Item " + std::to_string(i);
        list.insertLast(item);
    }

    cout << endl;

    cout << "--Print items in an initial order--" << endl;
    list.print();

    cout << endl;

    //Deep copy demonstration
    cout << "--Deep copy demonstration in a copy constructor--" << endl;
    SinglyLinkedList newList = list;

    //print items in the copied list
    cout << "--print items in the copied list--" << endl;
    newList.print();

    cout << endl;

    //Print items in a reverse order using iterative technique
    cout << "--Print items in a reverse order using iterative technique--" << endl;
    // Big-O analysis:
    // Each original node is traversed through creating a new temp object on a heap
    // and reversing the pointer direction at O(n) complexity
    // Each reverse node is printed at O(n).
    // At the end, it has roughly O(2n), dropping 2 makes the final time complexity O(n)
    newList.printReverseIterative();

    cout << endl;

    //Print items in a reverse order using recursive technique
    cout << "--Print items in a reverse order using recursive technique--" << endl;
    // Big O analysis:
    // Each node is called recursively until the node.next is null before each data is printed
    // Therefore, time complexity of this function is O(n);
    newList.printReverseRecursive();

    cout << endl;

    cout << "--Show that the original order is maintained--" << endl;
    newList.print();


    string foundValue = newList.find("Item 9");
    cout << foundValue << endl;

    newList.sort_rec();
    cout << endl;
    newList.print();

    // newList.sort_asc();
    // cout << endl;
    // newList.print();

    // newList.sort_desc();
    // cout << endl;
    // newList.print();

    // newList.sort_desc();
    // cout << endl;
    // newList.print();


}