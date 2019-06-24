#ifndef BinaryNode_h
#define BinaryNode_h

#include <memory>

using namespace std;

template<class K, class V>
class BinaryNode{
    private:
        K key;
        V value;
        shared_ptr<BinaryNode<K,V>> leftChildPtr;
        shared_ptr<BinaryNode<K,V>> rightChildPtr;

    public:
        BinaryNode();
        BinaryNode(const K& k, const V& v);
        BinaryNode(const K& k, const V& v, shared_ptr<BinaryNode<K,V>> leftPtr, shared_ptr<BinaryNode<K,V>> rightPtr);
        ~BinaryNode();

        void setKey(const K& k);
        void setValue(const V& v);
        K getKey() const;
        V getValue() const;

        shared_ptr<BinaryNode<K,V>> getLeftChildPtr() const;
        shared_ptr<BinaryNode<K,V>> getRightChildPtr() const;

        void setLeftChildPtr(shared_ptr<BinaryNode<K,V>> leftPtr);
        void setRightChildPtr(shared_ptr<BinaryNode<K,V>> rightPtr);

        bool operator<(const BinaryNode<K,V>& bn) const;
};

#include "BinaryNode.cpp"

#endif