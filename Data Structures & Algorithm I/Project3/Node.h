
#ifndef Node_h
#define Node_h

#include <string>
using namespace std;

class Node{
    public:
        Node();
        Node(string d);
        ~Node();

        Node* getNext() const;
        void setNext(Node* n);

        string getData() const;
        void setData(string d);
    private:
        Node* next;
        string data;
};

#endif