// ------------------------------------------------ file name ------------------------------------------------------- -
// Programmer Name:             Tetsuya Hayashi
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               02/14/2019
// Date of Last Modification:   02/14/2019
// -------------------------------------------------------------------------------------------------------------------- 
// GraphL - Header file - GraphL class represents a graph object
// -------------------------------------------------------------------------------------------------------------------- 
// Notes on specifications, special algorithms, and assumptions.
// uses adjacency list
// implements depth first search algorithm in depthFirstSearch.
// --------------------------------------------------------------------------------------------------------------------

#ifndef GRAPHL_H
#define GRAPHL_H

#include <fstream>//for ifstream
#include <sstream>//convert string to int
#include <iomanip>//display data
#include <string>

#include "nodedata.h"

#define MAXNODES 101

using namespace std;

struct EdgeNode;      // forward reference for the compiler
struct GraphNode {    // structs used for simplicity, use classes if desired
    EdgeNode* edgeHead; // head of the list of edges
    NodeData* data;     // data information about each node
    bool visited;                
};

struct EdgeNode {
    int adjGraphNode;  // subscript of the adjacent graph node
    EdgeNode* nextEdge;
};

class GraphL {                            
public:
    GraphL();   //constructor
    ~GraphL();  //destructor

    void buildGraph(ifstream&);     // builds up graph node information and adjacency list of edges between each node reading from a data file.
    void displayGraph() const;      // Displays each node information and edge in the graph
    void depthFirstSearch() const;  // Makes a depth-first search and displays each node in depth-first order

private:
    GraphNode* adjList[MAXNODES];           //adjacency list
    int size;                               //size of matrix
    bool insertEdge(int, int);              //inserts an edge between nodes
    void dfs(int graphNode) const;          //helper method to implement depth first search
    void deleteEdgeNode(EdgeNode*);         //helper method to release memory
    int toInt(const string& str) const;     //helper method to convert string to int
    string toString(int) const;             //helper method to convert int to string
};

#endif