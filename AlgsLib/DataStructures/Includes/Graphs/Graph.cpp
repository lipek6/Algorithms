// #pragma once
#include <iostream>
#include "GraphsCommons.h"
#include "AL.h"
#include "AM.h"
#include "AL_Generational.h"
#include "../Hash.h"



// Multigraph is not supported yet. This is the wrapper function.

/*
    CQRS (Command Query Responsibility Segregation):
    Esse princípio diz que métodos devem ser "Commands" (mudam o estado do sistema, como `addNode`, `runBFS`)
    OU "Queries" (retornam dados sem mudar nada, como `getCapacity`, `isDirected`). O ideal é que queries sejam
    marcadas com `const` no final da declaração (`size_t getCapacity() const`). Tente revisar a sua interface pública do `Graph` pensando nisso!

    - Start using the Named Return Value Optimization (NRVO) compiler optimization in my functions,
    primarly on th BFS ones, so that I stop storing the type T vector with the path and the one with the TraversedPath.
    These two are gonna be constructed directly inside the functions and be returned efficiently because of the NRVO optimization.
    This way, we will store only the size_t arrays that are encoding the paths and use a dirty bit/memoization/caching to check if
    we need to recalculate the path, if that is not the case, we will simply translate the size_t vector into a type T vector and return it
    with the powers of the NRVO.  
*/

template <typename T = int, typename W = graph_commons::NoWeight>
class Graph
{
private:
    // ==========================================
    // INTERNAL ENGINE & CORE MAPPINGS
    // ==========================================
    AL<W> _graph;  
    
    HashTable_Closed<T, size_t> _nodeToId;             
    Vector<T> _idToNode;
    Vector<size_t> _inDegrees;

    // ==========================================
    // STATE & CACHE MANAGEMENT (DIRTY FLAGS)
    // ==========================================
    enum class BOOL_STATES { TRUE, FALSE, DEPRECATED };

    bool _isSetToDirected;    
    BOOL_STATES _isConnected; 
    BOOL_STATES _isDirected;    
    BOOL_STATES _hasCycle;    

    inline void invalidateCache()
    {
        _isConnected = BOOL_STATES::DEPRECATED;
        _isDirected  = BOOL_STATES::DEPRECATED;
        _hasCycle    = BOOL_STATES::DEPRECATED;
    }

    // ==========================================
    // ALGORITHM DATA STRUCTURES
    // ==========================================
    struct BFSdataStruct
    {
        Vector<size_t> distances;               // Distance from each node to the source.
        Vector<size_t> traversal;               // Nodes in the BFS layer traversal order.
        Vector<size_t> predecessors;            // Node that first reached the current node. Useful for path reconstruction.
    };

    struct DFSdataStruct            
    {
        Vector<size_t> predecessors;
        Vector<bool> visited; 
    };

    struct dijkstraDataStruct
    {
        Vector<size_t> distances;               // Distance with weight from each node to the source.
        Vector<size_t> traversal;               // Nodes ordered from closest to furthest.
        Vector<size_t> predecessors;            // Node that better reached the current node. Useful for path reconstruction.
    };


    BFSdataStruct      _BFSdata;
    DFSdataStruct      _DFSdata;
    dijkstraDataStruct _dijkstraData;

public:
    // ==========================================
    // CONSTRUCTOR
    // ==========================================
    Graph(bool isSetToDirected = false) 
        : _isSetToDirected(isSetToDirected), 
          _isConnected(BOOL_STATES::DEPRECATED),
          _isDirected(BOOL_STATES::DEPRECATED),
          _hasCycle(BOOL_STATES::DEPRECATED) {}


    // ==========================================
    // CORE STRUCTURE MODIFIERS
    // ==========================================
    void addNode(const T& newNode = T())
    {
        if(_nodeToId.find(newNode) != nullptr) return;      // Nodes are unique

        invalidateCache(); 

        size_t newId = _graph.addNode();
        _nodeToId.insert(newNode, newId);

        if(newId < _idToNode.size())                        // Recycled idx
        {
            _idToNode[newId] = newNode;
            _inDegrees[newId] = 0;
        }
        else                                                // New idx
        {
            _idToNode.pushBack(newNode);
            _inDegrees.pushBack(0);
        }
    }

    void addEdge(const T& sourceNode, const T& destinyNode, const W weight = W())
    {
        size_t* sourceIdPtr  = _nodeToId.find(sourceNode);
        size_t* destinyIdPtr = _nodeToId.find(destinyNode);

        if(sourceIdPtr != nullptr && destinyIdPtr != nullptr)
        {
            invalidateCache(); 

            _graph.addEdge(*sourceIdPtr, *destinyIdPtr, weight);            
            _inDegrees[*destinyIdPtr]++;

            if(!_isSetToDirected)
            {
                _graph.addEdge(*destinyIdPtr, *sourceIdPtr, weight);
                _inDegrees[*sourceIdPtr]++;
            }
        }
    }

    void removeNode(const T& node)
    {
        size_t* idPtr = _nodeToId.find(node);
        if(idPtr != nullptr)
        {            
            invalidateCache(); 

            _graph.removeNode(*idPtr);
            _nodeToId.remove(node);
        }
    }

    void removeEdge(const T& sourceNode, const T& destinyNode)
    {
        size_t* sourceIdPtr  = _nodeToId.find(sourceNode);
        size_t* destinyIdPtr = _nodeToId.find(destinyNode);

        if(sourceIdPtr != nullptr && destinyIdPtr != nullptr)
        {
            invalidateCache(); 

            _graph.removeEdge(*sourceIdPtr, *destinyIdPtr);
            _inDegrees[*destinyIdPtr]--;

            if(!_isSetToDirected)
            {
                _graph.removeEdge(*destinyIdPtr, *sourceIdPtr);
                _inDegrees[*sourceIdPtr]--;
            }
        }           
    }


    // ==========================================
    // BREADTH-FIRST SEARCH (BFS)
    // ==========================================
    size_t runBFS(const T& sourceNode)
    {
        size_t* sourceIdPtr = _nodeToId.find(sourceNode);
        if(sourceIdPtr == nullptr) return 0;
        
        _BFSdata.distances.clear();
        _BFSdata.traversal.clear();
        _BFSdata.predecessors.clear();

        for(size_t i = 0; i < _graph.getCapacity(); i++)
        {
            _BFSdata.distances.pushBack(graph_commons::INFINITY_VAL);
            _BFSdata.predecessors.pushBack(graph_commons::INFINITY_VAL);
        }

        size_t numReachedNodes = _graph.runBFS(*sourceIdPtr, _BFSdata.distances, _BFSdata.predecessors, _BFSdata.traversal);

        if(numReachedNodes < _graph.getNumNodes()) 
            _isConnected = BOOL_STATES::FALSE;
        else
            _isConnected = BOOL_STATES::TRUE;
        
        return numReachedNodes;
    }

    size_t runBFS(const Vector<T>& sourceNodes)
    {
        _BFSdata.distances.clear();
        _BFSdata.traversal.clear();
        _BFSdata.predecessors.clear();

        Vector<size_t> sourceNodesIdx(sourceNodes.size());

        for(size_t i = 0; i < sourceNodes.size(); i++)
        {
            size_t* nodeIdPtr = _nodeToId.find(sourceNodes[i]);
            
            if(nodeIdPtr != nullptr)
            {
                sourceNodesIdx.pushBack(*nodeIdPtr);
            }
        }

        size_t numReachedNodes = _graph.runMultiSourceBFS(sourceNodesIdx, _BFSdata.distances, _BFSdata.predecessors, _BFSdata.traversal);

        if(numReachedNodes < _graph.getNumNodes()) 
            _isConnected = BOOL_STATES::FALSE;
        else
            _isConnected = BOOL_STATES::TRUE;

        return numReachedNodes;
    }

    size_t getBFSdistanceTo(const T& destinyNode)
    {   
        size_t* destinyIdPtr = _nodeToId.find(destinyNode);

        if(destinyIdPtr == nullptr || _BFSdata.distances.empty())
            return graph_commons::INFINITY_VAL;

        return _BFSdata.distances[*destinyIdPtr];
    }

    Vector<T> getBFSpathTo(const T& destinyNode)       // This is constructing the resultant vector on the caller function. This is the Named Return Value Optimization (NRVO) compiler optimization
    {
        size_t* destinyNodeIdPtr = _nodeToId.find(destinyNode);

        if(destinyNodeIdPtr == nullptr || _BFSdata.predecessors.empty())
            return Vector<T>();     


        Vector<size_t> pathIdx; pathIdx.pushBack(*destinyNodeIdPtr);
        size_t currentNodeIdx = *destinyNodeIdPtr;
        
        while(_BFSdata.predecessors[currentNodeIdx] != currentNodeIdx)      // Node that has itself as predecessor is a source node.
        {
            currentNodeIdx = _BFSdata.predecessors[currentNodeIdx];
            pathIdx.pushBack(currentNodeIdx);    
        }

        Vector<size_t> path(pathIdx.size());
        for(size_t i = pathIdx.size(); i > 0; i--)
        {
            path.pushBack(_idToNode[pathIdx[i - 1]]);
        }
        
        return path;        // NRVO compiler optimization
    }

    Vector<T> getBSTraversedPath()
    {
        Vector<T> taversedPath(_BFSdata.traversal.size());
        
        for(size_t i = 0; i < _BFSdata.traversal.size(); i++)
        {
            taversedPath.pushBack(_idToNode[_BFSdata.traversal[i]]);
        }

        return taversedPath;    // NRVO compiler optimization
    }    
    
    // ==========================================
    // DEPTH-FIRST SEARCH (DFS)
    // ==========================================
    void runDFS(const T& sourceNode)            // Change return type to size_t (num reached nodes)
    {
        size_t* sourceNodeIdx = _nodeToId.find(sourceNode);
        if(sourceNodeIdx == nullptr) return; 

        _DFSdata.visited.clear();
        for(size_t i = 0; i < _graph.topology.size(); i++)
        {
            _DFSdata.visited.pushBack(false);
        }

        _graph.DFS(*sourceNodeIdx, _DFSdata.visited); 
    }


    // ==========================================
    // DIJKSTRA
    // ==========================================
    void runDijkstra(const T& sourceNode)
    {
        size_t sourceIdxPtr = _nodeToId.find(sourceNode);
        
        if(sourceIdxPtr == nullptr)
            return;

        _dijkstraData.distances.clear();
        _dijkstraData.predecessors.clear();
        _dijkstraData.traversal.clear();

        for(size_t i = 0; i < _graph.getCapacity(); i++)
        {
            _dijkstraData.distances.pushBack(graph_commons::INFINITY_VAL);
            _dijkstraData.predecessors.pushBack(graph_commons::INFINITY_VAL);
        }


        _graph.runDijkstra(*sourceIdxPtr, _dijkstraData.distances, _dijkstraData.predecessors, _dijkstraData.traversal);
    }

    size_t getDijkstraDistanceTo(const T& destinyNode)
    {
        size_t destinyIdxPtr = _nodeToId.find(destinyNode);
        
        if(destinyIdxPtr == nullptr || _dijkstraData.distances.empty())
            return graph_commons::INFINITY_VAL;

        return _dijkstraData.distances[*destinyIdxPtr];
    }

    Vector<T> getDijkstraPathTo(const T& destinyNode)
    {
        size_t destinyIdxPtr = _nodeToId.find(destinyNode);        
        
        if(destinyIdxPtr == nullptr || _dijkstraData.distances.empty())
            return Vector<T>();


        size_t currentNode = *destinyIdxPtr;
        Vector<size_t> pathIdx; pathIdx.pushBack(currentNode);

        while(_dijkstraData.predecessors[currentNode] != currentNode)
        {
            currentNode = _dijkstraData.predecessors[currentNode];
            pathIdx.pushBack(currentNode);
        }


        Vector<T> path(pathIdx.size());

        for(size_t i = pathIdx.size(); i > 0; i--)
        {
            path.pushBack(_idToNode[pathIdx[i - 1]]);
        }
        return path;
    }

    // ==========================================
    // GRAPH PROPERTIES & QUERIES
    // ==========================================
    size_t getInDegree(const T& targetNode)
    {
        size_t* targetNodeIdx = _nodeToId.find(targetNode);
        if(targetNodeIdx == nullptr) return 0;

        return _inDegrees[*targetNodeIdx];
    }

    size_t getOutDegree(const T& targetNode) 
    {
        size_t* targetNodeIdx = _nodeToId.find(targetNode);
        if(targetNodeIdx == nullptr) return 0;

        return _graph.getNumAdjacentNodes(*targetNodeIdx);
    }

    bool isDirected() 
    {
        return _isSetToDirected; // TODO, really check if it is connected, the user can _isSetToDirected = true, but the graph might be undirected if he add edges in a certain way.
    }

    bool isConnected()
    {
        if (_isConnected == BOOL_STATES::DEPRECATED)
        {
            std::cout << "Warning: Graph structure changed, please run BFS/DFS to update connectivity.\n";
            return false; 
        }

        return _isConnected == BOOL_STATES::TRUE;
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

    G.runBFS(Recife);
    std::cout << "There are " << G.getBFSdistanceTo(Camaragibe) << "Roads From Recife to Camaragibe\n";
    G.getBFSpathTo(Camaragibe).print();
}