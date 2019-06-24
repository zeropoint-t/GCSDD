
#include <iostream>
#include "DoublyLinkedList.h"
#include "SearchCriteria.h"
#include "Node.h"

using namespace std;

DoublyLinkedList::DoublyLinkedList():size(0),head(nullptr),tail(nullptr){
}
DoublyLinkedList::DoublyLinkedList(const DoublyLinkedList& list):size(0),head(nullptr),tail(nullptr){
    //implement deep copy
    if(list.head == nullptr){
        this->head = nullptr;
        this->tail = nullptr;
    }else{
        Node* origChain = list.head;
        Node* newChain = new Node(*origChain);
        // cout << newChain << " vs " << origChain << endl;
        this->head = newChain;
        this->tail = newChain;

        origChain = origChain->getNext();
        while(origChain != nullptr){
            Node* newNode = new Node(*origChain);
            newChain->setNext(newNode);
            newNode->setPrev(newChain);
            tail = newNode;

            //advance 1 further
            newChain = newChain->getNext();
            origChain = origChain->getNext();

            // cout << newChain << " vs " << origChain << endl;
        }
    }
}
DoublyLinkedList::~DoublyLinkedList(){
    clear();
}
bool DoublyLinkedList::isEmpty() const{
    return this->size == 0;
}
int DoublyLinkedList::getLength() const{
    return this->size;
}
bool DoublyLinkedList::insert(int newIndex, House& newHouse){
    bool canInsert = (newIndex >= 0 && newIndex <= size);
    if(canInsert){
        Node* newNode = new Node(newHouse);
        if(newIndex == 0){
            //insert at the beginning
            if(head == nullptr){
                head = newNode;
                tail = newNode;
            }else{
                newNode->setNext(head);
                head->setPrev(newNode);
                head = newNode;
            }
        }
        else {
            Node* current = this->getNodeAt(newIndex);
            if(current == nullptr){
                //insert at the end
                tail->setNext(newNode);
                newNode->setPrev(tail);
                tail = newNode;
            }else{
                Node* prev = current->getPrev();
                newNode->setNext(current);
                current->setPrev(newNode);
                prev->setNext(newNode);
                newNode->setPrev(prev);
            }
        }
        ++size;
    }
    return canInsert;
}
bool DoublyLinkedList::remove(int index){
    bool canRemove = (index >= 0 && index < size);
    if(canRemove){
        if(size == 1){
            //only node is removed
            Node* nodeToRemove = head;
            delete nodeToRemove;
            head = nullptr;
            tail = nullptr;
        }else{
            //find node to remove
            int c = 0;
            Node* nodeToRemove = this->getNodeAt(index);

            //get previous node
            Node* prev = nodeToRemove->getPrev();
            Node* next = nodeToRemove->getNext();

            //first node to be removed
            if(prev == nullptr){
                head = next;
            }

            //last node to be removed
            if(next == nullptr){
                tail = prev;
            }

            //hook up prevous to next
            if(prev != nullptr){
                prev->setNext(next);
            }

            //hook up next to previous
            if(next != nullptr){
                next->setPrev(prev);
            }

            nodeToRemove->setNext(nullptr);
            nodeToRemove->setPrev(nullptr);
            delete nodeToRemove;
        }
        --size;
    }

    return canRemove;
}
void DoublyLinkedList::clear(){
    while(!this->isEmpty()){
        this->remove(0);
    }
}
Node* DoublyLinkedList::getNodeAt(int index) const{
    if(index >= 0 && index < this->size){
        int cnt = 0;
        Node* curNode = head;
        while(cnt++ < index){
            curNode = curNode->getNext();
        }
        return curNode;
    }else{
        return nullptr;
    }
}
void DoublyLinkedList::swapNodes(Node* &n1, Node* &n2){
    if(n1 != nullptr && n2 != nullptr){
        Node* next = n2->getNext();
        Node* prev = n1->getPrev();

        //1. rewire next
        if(next != nullptr){
            next->setPrev(n1);
        }else{
            //if next is null, n1 will be the last node in the list so tail needs to point to it
            tail = n1;
        }

        //2. rewire n2
        n2->setNext(n1);
        n2->setPrev(prev);

        //3. rewire n1
        n1->setNext(next);
        n1->setPrev(n2);

        //4. rewire prev
        if(prev != nullptr){
            prev->setNext(n2);
        }else{
            //if prev is nullptr, n2 will be the first node in the list so head needs to point to it
            head = n2;
        }

        // swap n1 & n2;
        Node* temp_n1 = n1;
        n1 = n2;
        n2 = temp_n1;
    }
}
void DoublyLinkedList::sort(SortType sortType) {
    if(size >= 2){
        for(int indexTo = this->size-1; indexTo > 0; indexTo--){
            int curIndex = 0;
            Node* n1 = this->head;
            Node* n2 = n1->getNext();
            while(++curIndex <= indexTo){
                if((sortType == asc && n1->getData() > n2->getData()) ||
                    (sortType == desc && n1->getData() < n2->getData()))
                {
                    this->swapNodes(n1,n2);
                }
                //move to next pair
                n1 = n1->getNext();
                n2 = n1->getNext();
            }
        }
    }
}
DoublyLinkedList DoublyLinkedList::serch(const SearchCriteria& filter) const{
    DoublyLinkedList list;
    Node* curNode = head;
    while(curNode!=nullptr){
        if(
            (filter.max_price == 0 || curNode->getData().getPrice() <= filter.max_price)
            && (filter.min_price == 0 || curNode->getData().getPrice() >= filter.min_price)
            && (filter.max_MilesFromMainCity == 0 || curNode->getData().getMilesFromMainCity() <= filter.max_MilesFromMainCity)
            && (filter.min_MilesFromMainCity == 0 || curNode->getData().getMilesFromMainCity() >= filter.min_MilesFromMainCity)
            && (filter.max_SquareFootage == 0 || curNode->getData().getSquareFootage() <= filter.max_SquareFootage)
            && (filter.min_SquareFootage == 0 || curNode->getData().getSquareFootage() >= filter.min_SquareFootage)
            && (filter.max_NumOfRooms == 0 || curNode->getData().getNumOfRooms() <= filter.max_NumOfRooms)
            && (filter.min_NumOfRooms == 0 || curNode->getData().getNumOfRooms() >= filter.min_NumOfRooms)
            && (filter.max_numOfBathrooms == 0 || curNode->getData().getNumOfBathrooms() <= filter.max_numOfBathrooms)
            && (filter.min_numOfBathrooms == 0 || curNode->getData().getNumOfBathrooms() >= filter.min_numOfBathrooms)
        ){
            list.insert(0, curNode->getData());
        }
        curNode = curNode->getNext();
    }
    return list;
}
void DoublyLinkedList::displayForward(){
    Node* n = this->head;
    while(n != nullptr){
        cout << "Score: " << printf("%.3f", n->getData().getScore())
        << "  Price: " << n->getData().getPrice() << "\t"
        << "  SqrFtg: " << n->getData().getSquareFootage()
        << "  MlsFromCty: " << n->getData().getMilesFromMainCity()
        << "  NumRms: " << n->getData().getNumOfRooms()
        << "  NmBthrms: " << n->getData().getNumOfBathrooms()
        << endl;
        n = n->getNext();
    }
}
void DoublyLinkedList::displayBackward(){
    Node* n = this->tail;
    while(n != nullptr){
        cout << "Score: " << printf("%.3f", n->getData().getScore())
        << "  Price: " << n->getData().getPrice() << "\t"
        << "  SqrFtg: " << n->getData().getSquareFootage()
        << "  MlsFromCty: " << n->getData().getMilesFromMainCity()
        << "  NumRms: " << n->getData().getNumOfRooms()
        << "  NmBthrms: " << n->getData().getNumOfBathrooms()
        << endl;
        n = n->getPrev();
    }
}