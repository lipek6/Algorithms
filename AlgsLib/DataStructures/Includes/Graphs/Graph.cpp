// #pragma once
#include <iostream>
// #include "AL.h"
#include "AL_Generational.h"
#include "../Hash.h"

// Multigraph is not supported yet. This is the wrapper function. This is friend with the Graph_AL class

/*
    We need a hasher to map the abstract class that the user wants to store to the
    Adjacent List that works only with size_t.
    
    If I am not using a key value division in th Graph, and the Type T has a lot of data besides what is used
    to identify it, we will have a lot of wasted space beacause this data will be both on th Vector<T> and on
    the HasTable_Closed<T, size_t>. Might refactor that. Hmmmm. 


    Lazy deletion for idToNode vector, otherwise (if using unorderedRemove)we will need to rearrange the topology to match 
    the new indexes, which will cost O(V + E). To reuse the space of these deleted nodes, we will use another vector as a 
    free list to reuse.

    Need to deal with the HashTable/vector memory problem when dealing with huge objects. Try to avoid the use of double the size.
    But this will probably be complicated without asking the user to define a key. Hmmmmm.

    Will deal with directed graphs later.
*/



template <typename T = int, typename W = NoWeight>
class Graph
{
private:
    AL_Generational<W> graph;  
    bool isDirected;

    HashTable_Closed<T, size_t> nodeToId;        // Need to add a custom hashing function later to satisfy the algorithms teacher
    Vector<T> idToNode;
    
public:
    Graph(bool isDirected = false) : isDirected(isDirected) {}

    void addNode(const T& newNode = T())
    {
        size_t newId = graph.addNode();
        nodeToId.insert(newNode, newId);

        if(newId < idToNode.size())
            idToNode[newId] = newNode;
        else
            idToNode.pushBack(newNode);
    }

    void addEdge(const T& sourceNode, const T& destinyNode, const W weight = W())
    {
        size_t* sourceId  = nodeToId.find(sourceNode);
        size_t* destinyId = nodeToId.find(destinyNode);

        if(sourceId != nullptr && destinyId != nullptr)
        {
            graph.addEdge(*sourceId, *destinyId, weight);
            
            if(!isDirected)
                graph.addEdge(*destinyId, *sourceId, weight);
        }
    }

    void removeNode(const T& node)
    {
        size_t* idPtr = nodeToId.find(node);
        if(idPtr != nullptr)
        {            
            graph.removeNode(*idPtr);
            nodeToId.remove(node);
        }
    }

    void removeEdge(const T& sourceNode, const T& destinyNode)
    {
        size_t* sourceId  = nodeToId.find(sourceNode);
        size_t* destinyId = nodeToId.find(destinyNode);

        if(sourceId != nullptr && destinyId != nullptr)
        {
            graph.removeEdge(*sourceId, *destinyId);

            if(!isDirected)
                graph.removeEdge(*destinyId, *sourceId);
        }           
    }
};


int main()
{
    Graph<std::string, int> G(false);
    
    std::string Recife = "Recife";
    std::string Olinda = "Olinda";
    std::string Paulista = "Paulista";
    std::string Camaragibe = "Camaragibe";
    std::string Petrolina = "Petrolina";

    G.addNode(Recife);
    G.addNode(Olinda);
    G.addNode(Paulista);
    G.addNode(Camaragibe);
    G.addNode(Petrolina);
    G.addEdge(Recife, Olinda, 2);
    G.addEdge(Olinda, Paulista, 2);
    G.addEdge(Paulista, Camaragibe, 2);
    G.addEdge(Camaragibe, Petrolina, 5);
    G.addEdge(Recife, Petrolina, 10);
    G.removeNode(Petrolina);
    G.removeEdge(Recife, Olinda);
}