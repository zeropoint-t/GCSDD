


#ifndef DoublyLinkedList_h
#define DoublyLinkedList_h

#include <iostream>
#include "House.h"
#include "Node.h"
#include "LinkListInterface.h"
#include "SearchCriteria.h"

class DoublyLinkedList: public HouseListInterface{
    public:
        DoublyLinkedList();

        DoublyLinkedList(const DoublyLinkedList& list);

        ~DoublyLinkedList();

        bool isEmpty() const override;

        int getLength() const override;
        
        bool insert(int newIndex, House& newHouse) override;
        
        bool remove(int index) override;
        
        void clear() override;
        
        void sort(SortType sortType) override;

        DoublyLinkedList serch(const SearchCriteria& filter) const;

        void displayForward() override;

        void displayBackward() override;

    private:
        int32_t size;
        Node* head;
        Node* tail;
        Node* getNodeAt(int index) const;
        void swapNodes(Node* &n1, Node* &n2);
};

#endif