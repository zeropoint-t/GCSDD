// ------------------------------------------------ bintree.h ---------------------------------------------------------
// Programmer Name:             Tetsuya Hayashi
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               01/29/2019
// Date of Last Modification:   01/29/2019
// -------------------------------------------------------------------------------------------------------------------- 
// BinTree - Header file - this class represents a binary search tree with additional functionalities such as
//           bstreeToArray & arrayToBSTree
// -------------------------------------------------------------------------------------------------------------------- 
// Notes on specifications, special algorithms, and assumptions.
// Uses binary search tree algorithm for insert and other relevant methods.
// --------------------------------------------------------------------------------------------------------------------

//hello world

#ifndef BINTREE_H
#define BINTREE_H

#include <string>
#include <iostream>
#include <fstream>
#include "nodedata.h"
using namespace std;

class BinTree {				// you add class/method comments and assumptions

    friend ostream & operator << (ostream &out, BinTree &bt);

public:
	BinTree();								// constructor
	BinTree(const BinTree &);				// copy constructor
	~BinTree();								// destructor, calls makeEmpty	
	bool isEmpty() const;					// true if tree is empty, otherwise false
	void makeEmpty();						// make the tree empty so isEmpty returns true
	BinTree& operator=(const BinTree &);    // assignment operator overload - deep copy
	bool operator==(const BinTree &) const; // compares data & structure of the trees for equality
	bool operator!=(const BinTree &) const; // compares data & structure of the trees for inequality
	bool insert(NodeData*);                 // inserts a NodeData pointer
	bool retrieve(const NodeData&, NodeData*&) const; // returns true and assigns the found NodeData to the 2nd parameter, else returns false
    int getHeight(const NodeData&) const;   // returns a height from a node through the longest path to the leaf for the NodeData
	void displaySideways() const;			// provided below, displays the tree sideways
    void bstreeToArray(NodeData* []);       // moves all NodeData pointers to the array in order, and leaves BinTree empty
    void arrayToBSTree(NodeData* []);       // moves all NodeData objects to BinTree in balanced structure, and leaves the array empty

private:
	struct Node 
    {
		NodeData* data;						// pointer to data object
		Node* left;							// left subtree pointer
		Node* right;						// right subtree pointer
	};
	Node* root;								// root of the tree

    // utility functions
    void sideways(Node*, int) const;        // provided below, helper for displaySideways()
    void deleteNode(Node*, const bool&);    // provides a way to release memories for the entire tree. The 2nd parameter determines if NodeData should be removed
    void findNodeHelper(Node*, const NodeData&, Node*&) const; // helper method to find a node that contains a specified NodeData
    int getHeightHelper(const Node*) const; // helper method to calculate height
    int getMax(int l, int r) const;         // find maximum of 2 int
    bool insertHelper(Node*&, NodeData*);   // helper method to insert
    NodeData* deepCopy(Node*, Node*&);      // helper method to deep copy
    void inorderHelper(ostream&, Node*) const;// helper method to display node
    bool equalityCheckHelper(Node* lhs, Node* rhs) const;// helper method to check if 2 BinTree are structurally equal
    void bstreeToArrayHelper(Node*, int&, NodeData* []);// helper method to move data from BinTree to array. Post: Leaves the BinTree empty
    void arrayToBSTreeHelper(NodeData* [], int, int);// helper method to move data from array to BinTree. Post: Leaves the array empty
};

#endif