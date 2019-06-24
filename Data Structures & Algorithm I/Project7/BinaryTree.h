#ifndef BinaryTree_h
#define BinaryTree_h

#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include "BinaryNode.h"

using namespace std;

template<class K, class V>
class BinaryTree{
    private:
        void add(shared_ptr<BinaryNode<K,V>> ptr, const K& k, const V& v);
        void clear(shared_ptr<BinaryNode<K,V>> ptr);
        void preorderTraversal(shared_ptr<BinaryNode<K,V>> ptr, void visit(K&,V&)) const;
        void inorderTraversal(shared_ptr<BinaryNode<K,V>> ptr, void visit(K&,V&)) const;
        void postorderTraversal(shared_ptr<BinaryNode<K,V>> ptr, void visit(K&,V&)) const;
        string getHoffmanCodeRec(K& k, shared_ptr<BinaryNode<K,V>> n, string& hoffmanCode, bool& found) const;

    protected:
        shared_ptr<BinaryNode<K,V>> root;

    public:
        BinaryTree();
        BinaryTree(shared_ptr<BinaryNode<K,V>> rt);
        BinaryTree(const K& k, const V& v);
        BinaryTree(const K& k, const V& v, shared_ptr<BinaryNode<K,V>> leftPrt, shared_ptr<BinaryNode<K,V>> rightPtr);
        ~BinaryTree();

        string getHoffmanCode(K& k) const;
        void add(const K& k, const V& v);
        void clear();
        void preorderTraversal(void visit(K&,V&)) const;
        void inorderTraversal(void visit(K&,V&)) const;
        void postorderTraversal(void visit(K&,V&)) const;
        void breadthFirstSearch(void visit(K&,V&)) const;
        void depthFirstSearch(void visit(K&,V&)) const;
};

#include "BinaryTree.cpp"

#endif