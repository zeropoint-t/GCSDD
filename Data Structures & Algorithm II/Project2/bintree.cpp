
// ------------------------------------------------ bintree.cpp -------------------------------------------------------
// Programmer Name:             Tetsuya Hayashi
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               01/29/2019
// Date of Last Modification:   01/29/2019
// -------------------------------------------------------------------------------------------------------------------- 
// BinTree - Implementation file - this class represents a binary search tree with additional functionalities such as
//           bstreeToArray & arrayToBSTree
// -------------------------------------------------------------------------------------------------------------------- 
// Notes on specifications, special algorithms, and assumptions.
// Uses binary search tree algorithm for insert and other relevant methods.
// --------------------------------------------------------------------------------------------------------------------

#include "bintree.h"

ostream & operator << (ostream &out, BinTree &bt) 
{
    bt.inorderHelper(out, bt.root);
    return out; 
}

//default constructor
BinTree::BinTree():root(NULL){
}

//copy constructor - deep copy
BinTree::BinTree(const BinTree & bt)
{
    if(bt.root != NULL)
    {
        root = new Node();
        root->data = deepCopy(bt.root, root);
    }else
    {
        //copy empty BinTree
        root = NULL;
    }
}

//destructor
BinTree::~BinTree()
{
    makeEmpty();
}	

//assignment operator overload - deep copy
BinTree& BinTree::operator=(const BinTree & bt)
{
    //same object then return this
    if(this == &bt)
        return *this;

    //release memory before losing a link
    makeEmpty();
    //deep copy if rhs has some values
    if(bt.root != NULL)
    {
        root = new Node();
        root->data = deepCopy(bt.root, root);
    }else
    {
        //copy empty BinTree
        root = NULL;
    }

    return *this;
}

// --BinTree::operator==(const BinTree &) const----------------------------------------------------
// Description: compare equality between lhs & rhs. Compares data & structure of this object
// Pre: NONE
// Post: BinTree remains unchanged.
// ------------------------------------------------------------------------------------------------
bool BinTree::operator==(const BinTree &rhs) const{
    //check nodes equality
    return equalityCheckHelper(root, rhs.root);
}

// --BinTree::operator!=(const BinTree &) const----------------------------------------------------
// Description: compare inequality between lhs & rhs. Compares data & structure of this object
// Pre: NONE
// Post: BinTree remains unchanged.
// ------------------------------------------------------------------------------------------------
bool BinTree::operator!=(const BinTree & rhs) const{
    //check nodes equality
    return !(*this == rhs);
}

// --BinTree::getHeight(const NodeData& nd) const----------------------------------------------------
// Description: returns a height from a node through the longest path to the leaf for the NodeData
// Pre: NONE
// Post: BinTree remains unchanged.
// --------------------------------------------------------------------------------------------------
int BinTree::getHeight(const NodeData& nd) const
{
    Node* n = NULL;
    findNodeHelper(root, nd, n);
    if(n != NULL)
    {
        return getHeightHelper(n);
    }else
    {
        return 0;
    }
}

// --BinTree::insert(NodeData* n)--------------------------------------------------------------------
// Description: Inserts a NodeData and returns true if successful. 
//              Returns false if duplicate is found
// Pre: NONE
// Post: BinTree will contain a new node
// --------------------------------------------------------------------------------------------------
bool BinTree::insert(NodeData* n){
    return insertHelper(root, n);
}

// --BinTree::retrieve(const NodeData& tgtNodeData, NodeData*& foundNodeData)----------------------
// Description: Returns true and foundNodeData points to the NodeData contained in BinTree,
//              or returns false and foundObject remains NULL
// Pre: foundNodeData is NULL
// Post: foundNodeData points to the found object, or NULL if not found
// ------------------------------------------------------------------------------------------------
bool BinTree::retrieve(const NodeData& tgtNodeData, NodeData*& foundNodeData) const{
    Node* foundNode = NULL;
    //find node that contains tgtNodeData first. foundNode should not be null if exists
    findNodeHelper(root, tgtNodeData, foundNode);

    //nodedata was found.
    if(foundNode != NULL){
        foundNodeData = foundNode->data;
        return true;
    }else
    {
        return false;
    }
}

// --BinTree::isEmpty()----------------------------------------------------------------------------
// Description: Checks if BinTree is empty
// Pre: NONE
// Post: BinTree remains unchanged.
// ------------------------------------------------------------------------------------------------
bool BinTree::isEmpty() const
{
    if(root == NULL)
        return true;
    else
        return false;
}

// --BinTree::makeEmpty()-------------------------------------------------------------------------
// Description: releases memories all the nodes occupy and sets the root to null
// Pre: NONE
// Post: BintTree is empty
// ------------------------------------------------------------------------------------------------
void BinTree::makeEmpty(){
    bool deleteData = true;
    deleteNode(root, deleteData);
    root = NULL;
}

//------------------------- displaySideways ---------------------------------
// Displays a binary tree as though you are viewing it from the side;
// hard coded displaying to standard output.
// Pre: NONE
// Post: BinTree remains unchanged.
void BinTree::displaySideways() const {
	sideways(root, 0);
}

// --BinTree::bstreeToArray(NodeData* nd[])--------------------------------------------------------
// Description: Moves all NodeData pointers to the array in order, and leaves BinTree empty
// Pre: NodeData array has sufficient memory for data transfer
// Post: Array is populated in order with NodeData objects and BinTree is empty
// ------------------------------------------------------------------------------------------------
void BinTree::bstreeToArray(NodeData* nd[])
{
    int index = 0;
    bstreeToArrayHelper(root, index, nd);

    bool deleteData = false;//do not delete nodedata for this operation
    deleteNode(root, deleteData);
    root = NULL;
}

// --BinTree::arrayToBSTree(NodeData* nd[])--------------------------------------------------------
// Description: Moves all NodeData objects to BinTree in balanced structure, and leaves the array empty
// Pre: None
// Post: BinTree is populated in balanced structure with NodeData objects and teh array is empty
// ------------------------------------------------------------------------------------------------
void BinTree::arrayToBSTree(NodeData* nd[])
{
    if(nd != NULL){
        int low = 0;
        int high = 0;
        //count upper limit and set it to high
        while(true){
            if(nd[high++] == NULL)
                break;
        }
        if(high == 0)
            return;
        //adjust high to the last index
        high -= 2;
        arrayToBSTreeHelper(nd, low, high);

        //empty array
        for(int i = 0; i <= high; i++){
            nd[i] = NULL;
        }
    }
}



//-------------------------------------------------------------------------------------
// private member methods
//-------------------------------------------------------------------------------------


// --BinTree::equalityCheckHelper(Node* lhs, Node* rhs) const--------------------------------------
// Description: Checks equality in NodeData and structure of BinTree
// Pre: None
// Post: BinTree remains unchanged
// ------------------------------------------------------------------------------------------------
bool BinTree::equalityCheckHelper(Node* lhs, Node* rhs) const{
    //both are NULL so it is the same
    if(lhs == NULL && rhs == NULL)
        return true;

    //one has value while the other is NULL, structure is different so it is false
    if((lhs != NULL && rhs == NULL) || (lhs == NULL && rhs != NULL))
        return false;

    //lhs and rhs has different data
    if(*(lhs->data) != *(rhs->data))
        return false;
    
    bool isEqual = false;
    isEqual = equalityCheckHelper(lhs->left, rhs->left);
    if(isEqual)
        isEqual = equalityCheckHelper(lhs->right, rhs->right);

    return isEqual;
}

// -- BinTree::findNodeHelper(Node* root, const NodeData& tgtNodeData, Node*& foundNode)-------------
// Description: finds & sets a node pointer that contains tgtNodeData to foundNode if exists
// Pre: foundNode is NULL
// Post: foundNode points to a node that contains tgtNodeData 
// --------------------------------------------------------------------------------------------------
void BinTree::findNodeHelper(Node* root, const NodeData& tgtNodeData, Node*& foundNode) const
{
    //didn't find it in this path
    if(root == NULL)
        return;

    //found the node that contains tgtNodeData
    //set the pointer to foundNode and return
    if(*(root->data) == tgtNodeData)
    {
        foundNode = root;
        return;
    }

    //go left
    findNodeHelper(root->left, tgtNodeData, foundNode);

    //go right if not found yet
    if(foundNode == NULL)
    {
        findNodeHelper(root->right, tgtNodeData, foundNode);
    }
}

// -- BinTree::insertHelper(Node*& n, NodeData* nd)--------------------------------------------------
// Description: helper method to insert NodeData into Bintree
// Pre: None
// Post: BinTree will contain a new Node
// --------------------------------------------------------------------------------------------------
bool BinTree::insertHelper(Node*& n, NodeData* nd)
{
    if(n == NULL)
    {
        n = new Node();
        n->data = nd;
        n->left = NULL;
        n->right = NULL;
        return true;
    }
    
    //duplicate
    if(*nd == *(n->data))
    {
        return false;
    }
    //less, go left
    else if(*nd < *(n->data))
    {
        return insertHelper(n->left, nd);
    }
    //more, go right
    else
    {
        return insertHelper(n->right, nd);
    }
}

// -- BinTree::insertHelper(Node*& n, NodeData* nd)--------------------------------------------------
// Description: helper method to do deep copy operation
// Pre: None
// Post: BinTree is populated with new NodeData
// --------------------------------------------------------------------------------------------------
NodeData* BinTree::deepCopy(Node* origNode, Node*& newNode)
{
    //base case - return if original node is null
    if(origNode == NULL)
        return NULL;

    //set data property of node to new NodeData object
    newNode->data = new NodeData(*origNode->data);

    if(origNode->left != NULL)
    {
        newNode->left = new Node();
        newNode->left->left = NULL;
        newNode->left->right = NULL;
        NodeData* leftNodeData = deepCopy(origNode->left, newNode->left);

        if(leftNodeData != NULL){
            newNode->left->data = leftNodeData;
        }
    }

    if(origNode->right != NULL)
    {
        newNode->right = new Node();
        newNode->right->left = NULL;
        newNode->right->right = NULL;
        NodeData* rightNodeData = deepCopy(origNode->right, newNode->right);

        if(rightNodeData != NULL){
            newNode->right->data = rightNodeData;
        }
    }

    return newNode->data;
}

// -- BinTree::getHeightHelper(const Node* n) const--------------------------------------------------
// Description: a helper method to find height of the node
// Pre: None
// Post: BinTree remains unchanged
// --------------------------------------------------------------------------------------------------
int BinTree::getHeightHelper(const Node* n) const{
    if(n == NULL)
        return 0;

    return 1 + getMax(getHeightHelper(n->left), getHeightHelper(n->right));
}

// -- BinTree::getMax(int l, int r) const------------------------------------------------------------
// Description: returns maximum of l or r
// Pre: None
// Post: BinTree remains unchanged
// --------------------------------------------------------------------------------------------------
int BinTree::getMax(int l, int r) const
{
    return l > r ? l : r;
}

// -- BinTree::deleteNode(Node* n)------------------------------------------------------------
// Description: helper method to delete a node in post order
// Pre: None
// Post: BinTree deletes the node
// -------------------------------------------------------------------------------------------
void BinTree::deleteNode(Node* n, const bool& deleteData)
{
    if(n == NULL)
        return;

    deleteNode(n->left, deleteData);
    deleteNode(n->right, deleteData);

    //if deleteData is true, delete data first if not null
    if(deleteData && n->data != NULL)
        delete n->data;
    //delete node
    delete n;
}

// -- BinTree::bstreeToArrayHelper(Node* n, int& index, NodeData* nd[])-----------------------
// Description: helper method for bstreeToArray method
// Pre: None
// Post: Ownership of the NodeData objects shifts to the array & BinTree is empty
// -------------------------------------------------------------------------------------------
void BinTree::bstreeToArrayHelper(Node* n, int& index, NodeData* nd[])
{
    if(n == NULL)
        return;
    
    bstreeToArrayHelper(n->left, index, nd);
    nd[index++] = n->data;
    bstreeToArrayHelper(n->right, index, nd);
}

// -- BinTree::arrayToBSTreeHelper(NodeData* nd[], int low, int high)-------------------------
// Description: helper method for arrayToBSTreeHelper
// Pre: None
// Post: BinTree will contain & owns Nodedata objects in balanced structure
// -------------------------------------------------------------------------------------------
void BinTree::arrayToBSTreeHelper(NodeData* nd[], int low, int high)
{
    if(low > high)
        return;
    int mid = (low + high) / 2;

    insertHelper(root, nd[mid]);
    arrayToBSTreeHelper(nd, low, mid-1);//left side
    arrayToBSTreeHelper(nd, mid+1, high);//left side
}

// --BinTree::inorderDisplayNode(ostream &out, Node* n)----------------------------------------------
// Description: display BinTree in Inorder
// Pre: NONE
// Post: BinTree remains unchanged.
// --------------------------------------------------------------------------------------------------
void BinTree::inorderHelper(ostream &out, Node* n) const
{
    if(n == NULL)
        return;

    inorderHelper(out, n->left);
    out << *(n->data) << " ";
    inorderHelper(out, n->right);
}

//---------------------------- Sideways -------------------------------------
// Helper method for displaySideways
// Pre: NONE
// Post: BinTree remains unchanged.
void BinTree::sideways(Node* current, int level) const {
	if (current != NULL) {
		level++;
		sideways(current->right, level);

		// indent for readability, 4 spaces per depth level 
		for (int i = level; i >= 0; i--) {
			cout << "    ";
		}

		cout << *current->data << endl;        // display information of object
		sideways(current->left, level);
	}
}