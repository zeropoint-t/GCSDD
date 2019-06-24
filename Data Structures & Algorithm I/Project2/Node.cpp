

#include "Node.h"

Node::Node():next(nullptr),prev(nullptr){};

Node::Node(const Node& node):next(nullptr),prev(nullptr){
    this->data = node.data;
}

Node::Node(const House& house):next(nullptr),prev(nullptr){
    this->data = house;
}

House& Node::getData(){
    return this->data;
}

void Node::setNext(Node* next){
    this->next = next;
}

Node* Node::getNext(){
    return this->next;
}

void Node::setPrev(Node* prev){
    this->prev = prev;
}

Node* Node::getPrev(){
    return this->prev;
}

void Node::operator=(Node& node){
    this->data = node.data;
    next = nullptr;
    prev = nullptr;
}