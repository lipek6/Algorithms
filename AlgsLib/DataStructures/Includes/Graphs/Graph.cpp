// #pragma once
#include <iostream>
#include "AL.h"
#include "../Hash.h"

// Multigraph is not supported yet. This is the wrapper function. This is friend with the Graph_AL class

/*
    We need a hasher to map the abstract class that the user wants to store to the
    Adjacent List that works only with size_t.
    
    If I am not using a key value division in th Graph, and the Type T has a lot of data besides what is used
    to identify it, we will have a lot of wasted space beacause this data will be both on th Vector<T> and on
    the HasTable_Closed<T, size_t>. Might refactor that. Hmmmm. 
*/



template <typename T = int, typename W = NoWeight>
class Graph
{
private:
    AL<W> graph;  
    bool is_directed;

    HashTable_Closed<T, size_t> nodeToId;        // Need to add a custom hashing function later to satisfy the algorithms teacher
    Vector<T> IdToNode;

public:
    Graph(bool is_directed = false) : is_directed(is_directed) {}
 

    void addNode(const T& newNode = T(), const W = W())
    {
        graphAL.addNode();
        nodeToId.insert(newNode, graph.topology.size())

    }



};


int main()
{
    Graph a;
}