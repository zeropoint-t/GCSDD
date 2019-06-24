// ------------------------------------------------ file name ------------------------------------------------------- -
// Programmer Name:             Tetsuya Hayashi
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               02/14/2019
// Date of Last Modification:   02/14/2019
// -------------------------------------------------------------------------------------------------------------------- 
// GraphM - Implementation file - GraphM class represents a graph object and implements Dijkstra's shortest path algorithm
// -------------------------------------------------------------------------------------------------------------------- 
// Notes on specifications, special algorithms, and assumptions.
// uses adjaceny matrix
// implements Dijkstra's shortest path algorithm in findShortestPath which finds the shortest path between 
// every node to every other node in the graph
// --------------------------------------------------------------------------------------------------------------------

#include "graphm.h"
using namespace std;

// -- GraphM::GraphM()-------------------------------------------------------------------------------
// Description: constructor - initialize C & T to default values
// Pre: None
// Post: C & T initialized
// --------------------------------------------------------------------------------------------------
GraphM::GraphM()
{
    //set adjacency matrix to infinity
    for(int r = 0; r < MAXNODES; r++)
    {  
        for(int c = 0; c < MAXNODES; c++)
        {
            C[r][c] = INT_MAX;
            T[r][c].visited = false;
            T[r][c].dist = INT_MAX;
            T[r][c].path = 0;
        }
    }
}

// -- GraphM::~GraphM()------------------------------------------------------------------------------
// Description: destructor
// Pre: None
// Post: None
// --------------------------------------------------------------------------------------------------
GraphM::~GraphM()
{

}

// -- GraphM::buildGraph(ifstream& infile)-----------------------------------------------------------
// Description: build a directed, weighted graph based on ifstream
// Pre: ifstream contains data that conforms to the input format
// Post: graph is built
// --------------------------------------------------------------------------------------------------
void GraphM::buildGraph(ifstream& infile){
    //find number of nodes in a graph and set it to size
    string s;
    getline(infile, s);
    size = this->toInt(s);

    //extract node data
    int nodeCount = size;
    int index = 1;
    while(--nodeCount >= 0){
        getline(infile, s);
        data[index++] = s;
    }

    //extract adjacent data
    index = 1;
    for (;;) {
        string i_str, j_str, dist_str;
        int i, j, dist;

		infile >> i_str;//from
        infile >> j_str;//to
        infile >> dist_str;//distance

        //convert to integers
        i = this->toInt(i_str);
        j = this->toInt(j_str);
        dist = this->toInt(dist_str);

        if(i == 0){
            getline(infile, s);//make sure to go to the next line
            break;
        }

        //add an edge
        this->insertEdge(i, j, dist);
	}
}

// -- bool GraphM::insertEdge(int i, int j, int dist)------------------------------------------------
// Description: inserts a new weighted edge between nodes i & j
// Pre: i & j have to be between 0 and size
// Post: new edge is inserted
// --------------------------------------------------------------------------------------------------
bool GraphM::insertEdge(int i, int j, int dist)
{
    //out of range
    if(i > size || j > size || i < 0 || j < 0)
        return false;

    //negative distance not allowed
    if(dist < 0)
        return false;

    C[i][j] = dist;

    return true;
}

// -- bool GraphM::removeEdge(int i, int j)----------------------------------------------------------
// Description: removes an edge between nodes i & j
// Pre: i & j have to be between 0 and size
// Post: an edge is removed
// --------------------------------------------------------------------------------------------------
bool GraphM::removeEdge(int i, int j) 
{
    //out of range
    if(i > size || j > size || i < 0 || j < 0)
        return false;

    C[i][j] = INT_MAX;

    return true;
}

// -- void GraphM::findShortestPath()----------------------------------------------------------------
// Description: find a shortest path from all nodes to all other nodes
// Pre: none
// Post: T contains shortest path information
// --------------------------------------------------------------------------------------------------
void GraphM::findShortestPath()
{
    for (int source = 1; source <= size; source++) {
        T[source][source].dist = 0;
        T[source][source].visited = true;
        int curVisitedPath = source;
        while(true)
        {
            //1. update distance & path from curVisitedPath to its adjacent nodes
            updateDistAndPath(source, curVisitedPath);

            //2. find next unvisited node to visit and save it to curVisitedPath, shortest distance at this point
            curVisitedPath = findNextPath(source);

            if(curVisitedPath == INT_MAX)//no more nodes to visit
                break;

            //3. mark curVisitedPath as visited
            T[source][curVisitedPath].visited = true;
        }
    }
}

// -- void GraphM::updateDistAndPath(int source, int curVisitedPath)---------------------------------
// Description: update T with the currently shortest dist and path
// Pre: none
// Post: T is updated with shortest distance and previous path
// --------------------------------------------------------------------------------------------------
void GraphM::updateDistAndPath(int source, int fromPath)
{
    //1. iterate through all paths
    for(int toPath = 1; toPath <= size; toPath++){
        //2. find adjacent node from fromPath to toPath
        if(C[fromPath][toPath] != INT_MAX)
        {
            //3. calculate distance from fromPath to this path
            int newDist = 
                T[source][fromPath].dist + //currently shortest dist from source to fromPath
                C[fromPath][toPath];//dist from fromPath to toPath

            //4. if newDist is less than currently shortest distance from source to toPath
            //   update dist and path in T
            if(newDist < T[source][toPath].dist)
            {
                T[source][toPath].dist = newDist;
                T[source][toPath].path = fromPath;
            }
        }
    }
}

// -- int GraphM::findNextPath(int source) ----------------------------------------------------------
// Description: finds the next unvisited path with minimum distance from the source
// Pre: none
// Post: none
// --------------------------------------------------------------------------------------------------
int GraphM::findNextPath(int source) const
{
    int minDist = INT_MAX;
    int nextPath = INT_MAX;
    //1. iterate through all paths
    for(int toPath = 1; toPath <= size; toPath++)
    {
        //2. find unvisited path
        if(!T[source][toPath].visited)
        {
            //3. distance from source to path
            int newDist = T[source][toPath].dist;
            //4. find current minimum distance of all unvisited paths
            if(newDist < minDist)
            {
                minDist = newDist;//keep track of smallest
                nextPath = toPath;
            }
        }
    }

    return nextPath;
}

// -- void GraphM::displayAll() const----------------------------------------------------------------
// Description: display the cost and path from every node to every other node 
// Pre: none
// Post: none
// --------------------------------------------------------------------------------------------------
void GraphM::displayAll() const
{
    if(size == 0)
        return;

    cout << "-----------------------------------------------------------" << endl;
    cout << "----------------------- Display All -----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;


    int descColumnSize = 20;
    int baseColumnSize = 12;

    cout << left <<
        setw(descColumnSize) << "Description" <<
        setw(baseColumnSize) << "From node" <<
        setw(baseColumnSize) << "To node" <<
        setw(baseColumnSize) << "Dijkstra's" <<
        setw(baseColumnSize) << "Path"
    << endl;

    for(int source = 1; source <= size; source++)
    {
        bool showDiscription = true;
        for(int path = 1; path <= size; path++)
        {
            if(path != source)
            {
                NodeData description;
                string fromNode = toString(source);
                string toNode = toString(path);
                string dijkstra = T[source][path].visited ? toString(T[source][path].dist) : "---";
                string pathSequence = "";

                if(showDiscription)
                {
                    description = data[source];
                    cout << description << endl;
                    showDiscription = false;
                }

                if(T[source][path].visited)
                {
                    pathSequence = getPathSequence(source, path);
                }

                cout << left <<
                    setw(descColumnSize) << "" <<
                    setw(baseColumnSize) << fromNode <<
                    setw(baseColumnSize) << toNode <<
                    setw(baseColumnSize) << dijkstra <<
                    setw(baseColumnSize) << pathSequence
                << endl;
            }
        }
    }

    cout << endl << endl;
}    

// -- void GraphM::display(int from, int to) const---------------------------------------------------
// Description: displays one path in detail
// Pre: none
// Post: none
// --------------------------------------------------------------------------------------------------
void GraphM::display(int from, int to) const
{
    //out of range
    if(from < 1 || from > size || to < 1 || to > size)
        return;

    //not visited, just return
    if(!T[from][to].visited)
        return;

    cout << "---------------------------------" << endl;
    cout << "------------ Display ------------" << endl;
    cout << "---------------------------------" << endl;

    int dist = T[from][to].dist;
    string pathSequence = T[from][to].visited ? getPathSequence(from, to) : "";

    cout << left <<
        setw(7) << from <<
        setw(7) << to <<
        setw(10) << dist <<
        pathSequence
    << endl;

    //split path sequence by space, and use each node to print node data
    string s;
    for(string::iterator it = pathSequence.begin(); it != pathSequence.end(); it++)
    {
        if(*it == ' ')//space?
        {
            int path = toInt(s);//convert s to int
            s = "";//empty s
            cout << data[path] << endl;
        }else
        {
            s += *it;//append char
        }
    }
    //print last nodedata
    if(s.length() > 0)
    {
        int path = toInt(s);
        s = "";
        cout << data[path] << endl;
    }
    cout << endl;
}

// -- string GraphM::getPathSequence(int from, int to) const-----------------------------------------
// Description: returns a string representing a sequence of shortest path between 2 nodes
// Pre: none
// Post: none
// --------------------------------------------------------------------------------------------------
string GraphM::getPathSequence(int from, int to) const
{
    if(from < 1 || from > size || to < 1 || to > size)
        return "";

    //appaned destination path
    string pathSequence = toString(to);
    int curPath = T[from][to].path;
    //follow the sequence
    while(curPath != from)
    {
        pathSequence.insert(0, " ");
        pathSequence.insert(0, toString(curPath));             
        curPath = T[from][curPath].path;
    }
    //append source
    pathSequence.insert(0, " ");
    pathSequence.insert(0, toString(from)); 

    return pathSequence;
}

// -- GraphM::toInt(const string& str) const---------------------------------------------------------
// Description: helper method to convert string to int
// Pre: none
// Post: none
// --------------------------------------------------------------------------------------------------
int GraphM::toInt(const string& str) const
{
    int i = 0;
    istringstream ss(str);
    ss >> i;
    return i;
}

// -- string GraphM::toString(int num)---------------------------------------------------------------
// Description: helper method to convert int to string
// Pre: none
// Post: none
// --------------------------------------------------------------------------------------------------
string GraphM::toString(int num) const
{
    ostringstream convert;   // stream used for the conversion
    convert << num;      // insert the textual representation of 'Number' in the characters in the stream
    return convert.str(); // set 'Result' to the contents of the stream
}