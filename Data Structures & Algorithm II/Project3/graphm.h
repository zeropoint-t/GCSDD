// ------------------------------------------------ file name ------------------------------------------------------- -
// Programmer Name:             Tetsuya Hayashi
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               02/14/2019
// Date of Last Modification:   02/14/2019
// -------------------------------------------------------------------------------------------------------------------- 
// GraphM - Header file - GraphM class represents a graph object and implements Dijkstra's shortest path algorithm
// -------------------------------------------------------------------------------------------------------------------- 
// Notes on specifications, special algorithms, and assumptions.
// uses adjaceny matrix
// implements Dijkstra's shortest path algorithm in findShortestPath which finds the shortest path between 
// every node to every other node in the graph
// --------------------------------------------------------------------------------------------------------------------

#ifndef GRAPHM_H
#define GRAPHM_H

#include <fstream>//for ifstream
#include <climits>//INT_MIN
#include <iomanip>//display data
#include <sstream>//convert string to int
#include <string>

#include "nodedata.h"

#define MAXNODES 101

using namespace std;

struct TableType {
    bool visited;          // whether node has been visited
    int dist;              // shortest distance from source known so far
    int path;              // previous node in path of min dist
};

class GraphM {                            
public:
    GraphM();
    ~GraphM();

    void buildGraph(ifstream&);         // builds up graph node information and adjacency matrix of edges between each node reading from a data file 
    bool insertEdge(int, int, int);     // insert an edge into graph between two given nodes
    bool removeEdge(int, int);          // remove an edge between two given nodes
    void findShortestPath();            // find the shortest path between every node to every other node in the graph
    void displayAll() const;            // demonstrate that the algorithm works properly
    void display(int, int) const;       // display the shortest distance with path info between the fromNode to toNode
private:
    NodeData data[MAXNODES];            // data for graph nodes 
    int C[MAXNODES][MAXNODES];          // Cost array, the adjacency matrix
    int size;                           // number of nodes in the graph
    TableType T[MAXNODES][MAXNODES];    // stores visited, distance, path
    int findNextPath(int) const;        // find the next path to visit, shortest distance at this point
    void updateDistAndPath(int, int);   // update distance & path for adjacent paths from currently visited node
    string getPathSequence(int, int) const;   // returns sequence of path from source to destination
    int toInt(const string&) const;     // helper function to convert string to int
    string toString(int) const;         //helper function to convert int to string
};


#endif