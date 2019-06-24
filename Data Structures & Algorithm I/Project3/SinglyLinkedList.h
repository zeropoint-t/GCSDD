
#ifndef SinglyLinkedList_h
#define SinglyLinkedList_h

#include "Node.h"
#include <string>

class SinglyLinkedList{
    public:

        SinglyLinkedList();
        SinglyLinkedList(const SinglyLinkedList& list);
        ~SinglyLinkedList();

        bool insert(string d, int index);
        void insertFirst(string d);
        void insertLast(string d);
        
        bool remove(int index);

        string find(string str, Node* n = nullptr) const;

        void print() const;
        void printReverseIterative() const;
        void printReverseRecursive(Node* n = nullptr) const;
        void clear();

        void sort_desc();
        void sort_asc();

        void sort_rec();

    private:
        Node* head;
        int size;
        Node* getNodeAt(int index);
        void swap(Node* &prev, Node* &cur, Node* &next);
        void sort_rec(int& recCnt, Node* prev, Node* cur);
};

#endif