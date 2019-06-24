// ------------------------------------------------ file name ------------------------------------------------------- -
// Programmer Name:             Tetsuya Hayashi
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               02/14/2019
// Date of Last Modification:   02/14/2019
// -------------------------------------------------------------------------------------------------------------------- 
// GraphL - Implementation file - GraphL class represents a graph object
// -------------------------------------------------------------------------------------------------------------------- 
// Notes on specifications, special algorithms, and assumptions.
// uses adjacency list
// implements depth first search algorithm in depthFirstSearch.
// --------------------------------------------------------------------------------------------------------------------

#include "graphl.h"
using namespace std;

// -- GraphL::GraphL()------------------------------------------------------------------------------
// Description: constructor
// Pre: non
// Post: adjList is initialized
// --------------------------------------------------------------------------------------------------
GraphL::GraphL()
{
    for(int i = 0; i < MAXNODES; i++)
    {
        adjList[i] = new GraphNode();
        adjList[i]->edgeHead = NULL;
        adjList[i]->data = NULL;
        adjList[i]->visited = false;
    }
}

// -- GraphL::~GraphL()------------------------------------------------------------------------------
// Description: destructor
// Pre: non
// Post: all memories are released
// --------------------------------------------------------------------------------------------------
GraphL::~GraphL()
{
    for(int i = 0; i < MAXNODES; i++)
    {
        GraphNode* gn = adjList[i];
        if(gn != NULL)
        {
            deleteEdgeNode(gn->edgeHead);
            delete gn->data;
            delete gn;
        }
    }
}

// -- void GraphL::deleteEdgeNode(EdgeNode* en)------------------------------------------------------
// Description: helper method to release all memories occpied by EdgeNode list
// Pre: non
// Post: a list of edgenodes is released
// --------------------------------------------------------------------------------------------------
void GraphL::deleteEdgeNode(EdgeNode* en)
{
    if(en == NULL)
        return;

    deleteEdgeNode(en->nextEdge);

    delete en;
}

// -- void GraphL::buildGraph(ifstream& infile)------------------------------------------------------
// Description: build a directed graph based on ifstream
// Pre: ifstream contains data that conforms to the input format
// Post: graph is built
// --------------------------------------------------------------------------------------------------
void GraphL::buildGraph(ifstream& infile)
{
    //find number of nodes in a graph and set it to size
    string s;
    getline(infile, s);
    size = this->toInt(s);

    //extract node data
    int nodeCount = size;
    int index = 1;
    while(--nodeCount >= 0){
        getline(infile, s);
        adjList[index++]->data = new NodeData(s);
    }

    //extract adjacent data
    index = 1;
    for (;;) {
        string i_str, j_str;
        int i, j;

		infile >> i_str;//from
        infile >> j_str;//to

        //convert to integers
        i = this->toInt(i_str);
        j = this->toInt(j_str);

        if(i == 0){
            getline(infile, s);//make sure to go to the next line
            break;
        }

        //add an edge
        this->insertEdge(i, j);
	}
}

// -- bool GraphL::insertEdge(int i, int j)----------------------------------------------------------
// Description: inserts a new edge between nodes i & j
// Pre: i & j have to be between 0 and size
// Post: new edge is inserted
// --------------------------------------------------------------------------------------------------
bool GraphL::insertEdge(int i, int j)
{
    if(i < 1 || i > size || j < 1 || j > size)
        return false;

    GraphNode* gn = adjList[i];//get GraphNode from adjacency list
    EdgeNode* en = new EdgeNode();//new edgenode
    en->adjGraphNode = j;//set adj to j
    en->nextEdge = gn->edgeHead;//insert new edgenode at the head
    gn->edgeHead = en;

    return true;
}

// -- void GraphL::displayGraph() const--------------------------------------------------------------
// Description: Displays each node information and edge in the graph
// Pre: none
// Post: none
// --------------------------------------------------------------------------------------------------
void GraphL::displayGraph() const
{
    cout << left << setw(20) << "Graph:" << endl;

    for(int i = 1; i <= size; i++)
    {
        string nodeNum = "Node " + toString(i);
        cout << setw(20) << nodeNum;
        cout << setw(40) << *(adjList[i]->data) << endl;

        EdgeNode* en = adjList[i]->edgeHead;
        while(en != NULL)
        {
            string edge = "  edge  " + toString(i) + "  " + toString(en->adjGraphNode);
            cout << setw(20) << edge << endl;
            en = en->nextEdge;
        }
    }

    cout << endl;
}

// -- void GraphL::depthFirstSearch() const----------------------------------------------------------
// Description: Makes a depth-first search and displays each node in depth-first order
// Pre: none
// Post: none
// --------------------------------------------------------------------------------------------------
void GraphL::depthFirstSearch() const
{
    cout << "Depth-first ordering: ";
    dfs(1);
    cout << endl << endl;
}

// -- void GraphL::dfs(int graphNode) const----------------------------------------------------------
// Description: helper method to implement depth first search
// Pre: none
// Post: none
// --------------------------------------------------------------------------------------------------
void GraphL::dfs(int graphNode) const
{
    cout << graphNode << " ";

    GraphNode* gn = adjList[graphNode];
    gn->visited = true;

    EdgeNode* nextEN = gn->edgeHead;
    while(true)
    {
        if(nextEN == NULL)//base case
            return;

        GraphNode* nextGN = adjList[nextEN->adjGraphNode];
        if(nextGN->visited == false)
        {
            dfs(nextEN->adjGraphNode);
        }

        nextEN = nextEN->nextEdge;
    }
}

// -- GraphL::toInt(const string& str) const---------------------------------------------------------
// Description: helper method to convert string to int
// Pre: none
// Post: none
// --------------------------------------------------------------------------------------------------
int GraphL::toInt(const string& str) const
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
string GraphL::toString(int num) const{
    ostringstream convert;   // stream used for the conversion
    convert << num;      // insert the textual representation of 'Number' in the characters in the stream
    return convert.str(); // set 'Result' to the contents of the stream
}