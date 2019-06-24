// ----------------------------------------------HashTable.h--------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/03/2019
// -----------------------------------------------------------------------------
// Hash - Header file - Custom Hash Class
// -----------------------------------------------------------------------------
// Notes on specifications, special algorithms, and assumptions.
// -----------------------------------------------------------------------------

#ifndef HashTable_h
#define HashTable_h

#include <iostream>
#include <fstream>
#include "HashNode.h"

using namespace std;

// Hash map class template
template <typename k, typename v>
class HashTable
{
    typedef HashNode<k, v>* hashPtr;

private:
    static size_t maxSize;
    static size_t curSize;
    hashPtr* arr;
    void growTable();

    void add(const k& key, const v& value, hashPtr*& table);

    void addFront(const k& key, const v& value, hashPtr*& table);

    void deleteAll(hashPtr*& table, size_t size);

public:
    HashTable();

    ~HashTable();

    bool get(const k& key, v& value);

    HashNode<k, v>* get(const k& key);

    void add(const k& key, const v& value);

    void addFront(const k& key, const v& value);

    void remove(const k& key);

    unsigned long getHash(const k &key) const;

    void showItems() const
    {
        cout << "------------Show Items-------------" << endl;
        // destroy all nodes one by one
        for (size_t i = 0; i < maxSize; i++) 
        {
            hashPtr cur = arr[i];

            while (cur != nullptr) 
            {
                hashPtr prev = cur;
                cur = cur->getNext();
                cout << "Showing " << *(prev->getValue()) << endl;
            }
        }
    }
};

template <typename k, typename v>
size_t HashTable<k,v>::maxSize = 83;

template <typename k, typename v>
size_t HashTable<k,v>::curSize;

template <typename k, typename v>
HashTable<k,v>::HashTable(){
    arr = new hashPtr[maxSize];
    for(int i = 0; i < maxSize; i++)
    {
        arr[i] = nullptr;
    }
}

template <typename k, typename v>
HashTable<k,v>::~HashTable()
{
    // destroy all nodes one by one
    deleteAll(arr, maxSize);
}

template <typename k, typename v>
void HashTable<k,v>::growTable()
{
    curSize = 0;
    maxSize *= 2;//grow table size by 2
    hashPtr* newTable = new hashPtr[maxSize];
    //set all elements to nullptr
    for(int i = 0; i < maxSize; i++)
    {
        newTable[i] = nullptr;
    }

    //copy over to new array
    for(int i = 0; i < maxSize / 2; i++)
    {
        hashPtr ptr = arr[i];
        while(ptr != nullptr)
        {
            add(ptr->getKey(), ptr->getValue(), newTable);
            ptr = ptr->getNext();
        }
    }

    deleteAll(arr, maxSize / 2);
    
    arr = newTable;
}

template <typename k, typename v>
void HashTable<k,v>::add(const k& key, const v& value, hashPtr*& table)
{
    unsigned long hashValue = getHash(key);
    hashPtr prev = nullptr;
    hashPtr cur = table[hashValue];

    while (cur != nullptr && cur->getKey() != key)
    {
        prev = cur;
        cur = cur->getNext();
    }

    if (cur == nullptr)//insert
    {
        cur = new HashNode<k, v>(key, value);

        if (prev == nullptr)//very first node
        {
            table[hashValue] = cur;//insert
        } else 
        {
            prev->setNext(cur);
        }

        curSize++;//increment current item count
    } else {//found the same key
        cur->setValue(value);//update
    }

    //grow table size
    if(curSize / (double)maxSize > 0.8){
        growTable();
    }
}

template <typename k, typename v>
void HashTable<k,v>::addFront(const k& key, const v& value, hashPtr*& table)
{
    unsigned long hashValue = getHash(key);
    hashPtr prev = nullptr;
    hashPtr cur = table[hashValue];

    while (cur != nullptr && cur->getKey() != key)
    {
        prev = cur;
        cur = cur->getNext();
    }

    if (cur == nullptr)//insert
    {
        cur = new HashNode<k, v>(key, value);

        if (prev == nullptr)//very first node
        {
            table[hashValue] = cur;//insert
        } else 
        {
            prev->setNext(cur);
        }

        curSize++;//increment current item count
    } else {//found the same key so add the new node at the head
        hashPtr newNode = new HashNode<k, v>(key, value);
        newNode->setNext(cur);

        if(prev != nullptr)
            prev->setNext(newNode);
        else
            table[hashValue] = newNode;
    }

    //grow table size
    // if(curSize / (double)maxSize > 0.8){
    //     growTable();
    // }
}

template <typename k, typename v>
void HashTable<k,v>::deleteAll(hashPtr*& table, size_t size)
{
    // destroy all nodes one by one
    for (size_t i = 0; i < size; i++) 
    {
        hashPtr cur = table[i];

        while (cur != nullptr) 
        {
            hashPtr prev = cur;
            cur = cur->getNext();
            delete prev;
        }

        table[i] = nullptr;
    }

    delete[] table;
}

template <typename k, typename v>
bool HashTable<k,v>::get(const k& key, v& value)
{
    unsigned long hashValue = getHash(key);
    hashPtr cur = arr[hashValue];

    while (cur != nullptr) {
        if (cur->getKey() == key) {
            value = cur->getValue();
            return true;
        }

        cur = cur->getNext();
    }

    return false;
}

//returns head hashnode
template <typename k, typename v>
HashNode<k, v>* HashTable<k,v>::get(const k& key)
{
    unsigned long hashValue = getHash(key);
    hashPtr cur = arr[hashValue];

    while (cur != nullptr) {
        if (cur->getKey() == key) {
            return cur;
        }
        cur = cur->getNext();
    }

    return nullptr;
}

template <typename k, typename v>
void HashTable<k,v>::add(const k& key, const v& value)
{
    add(key, value, arr);
}

template <typename k, typename v>
void HashTable<k,v>::addFront(const k& key, const v& value)
{
    addFront(key, value, arr);
}

template <typename k, typename v>
void HashTable<k,v>::remove(const k& key)
{
    unsigned long hashValue = getHash(key);
    hashPtr prev = nullptr;
    hashPtr cur = arr[hashValue];

    while (cur != nullptr && cur->getKey() != key)
    {
        prev = cur;
        cur = cur->getNext();
    }

    if (cur == nullptr)//key not found
    {
        return;
    } else 
    {
        if (prev == nullptr)//remove first bucket of the list
        {
            arr[hashValue] = cur->getNext();
        } else 
        {
            //cur is not nullptr && previous is not nullptr
            //meaning the slot has more than 1 items chained
            prev->setNext(cur->getNext());
        }
        delete cur;
        curSize--;//decrement current item count
    }
}

template <typename k, typename v>
unsigned long HashTable<k,v>::getHash(const k &key) const
{
    unsigned long key_hash = std::hash<k>()(key) % maxSize;;
    return key_hash;
}

#endif
