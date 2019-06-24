

#ifndef Node_h
#define Node_h

#include <iostream>
#include "House.h"

class Node{
    public:
        Node();
        Node(const Node& node);
        Node(const House& house);
        
        House& getData();

        void setNext(Node* next);
        Node* getNext();

        void setPrev(Node* prev);
        Node* getPrev();

        void operator=(Node& node);

    private:
        House data;
        Node* next;
        Node* prev;
};

#endif