
#include <iostream>
#include "Node.h"
using namespace std;

Node::Node():data(""),next(nullptr){
    // cout << "Node is created at " << this << endl;
}

Node::Node(string d):data(d),next(nullptr){
    // cout << "Node is created with " << data << " at " << this << endl;
}

Node::~Node(){
    // cout << "Node is destroyed at " << this << endl;
}

Node* Node::getNext() const{
    return this->next;
}

void Node::setNext(Node* n){
    this->next = n;
}

string Node::getData() const{
    return this->data;
}

void Node::setData(string d){
    this->data = d;
}