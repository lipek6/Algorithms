// #pragma once
#include "GraphsCommons.h"
#include <iostream>
#include "AL.h"
#include "AL_Generational.h"
#include "../Hash.h"

// Multigraph is not supported yet. This is the wrapper function.

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

    void invalidateCache()
    {
        _isConnected = BOOL_STATES::DEPRECATED;
        _hasCycle = BOOL_STATES::DEPRECATED;
    }

    // ==========================================
    // ALGORITHM DATA STRUCTURES
    // ==========================================
    struct BFSdataStruct
    {
        Vector<size_t> distances;
        Vector<size_t> traversal;
        Vector<size_t> predecessors;
        Vector<T> TraversedPath;
        Vector<T> path;
    };

    struct DFSdataStruct            
    {
        Vector<size_t> predecessors;
        Vector<bool> visited; 
    };

    BFSdataStruct _BFSdata;
    DFSdataStruct _DFSdata;


public:
    // ==========================================
    // CONSTRUCTOR
    // ==========================================
    Graph(bool isSetToDirected = false) 
        : _isSetToDirected(isSetToDirected), 
          _isConnected(BOOL_STATES::DEPRECATED),
          _hasCycle(BOOL_STATES::DEPRECATED) {}


    // ==========================================
    // CORE STRUCTURE MODIFIERS
    // ==========================================
    void addNode(const T& newNode = T())
    {
        invalidateCache(); 

        size_t newId = _graph.addNode();
        _nodeToId.insert(newNode, newId);

        if(newId < _idToNode.size())
        {
            _idToNode[newId] = newNode;
            _inDegrees[newId] = 0;
        }
        else
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
    void runBFS(const T& sourceNode)
    {
        size_t* sourceIdx = _nodeToId.find(sourceNode);
        if(sourceIdx == nullptr) return;
        
        _BFSdata.distances.clear();
        _BFSdata.traversal.clear();
        _BFSdata.predecessors.clear();

        for(size_t i = 0; i < _graph.getCapacity(); i++)
        {
            _BFSdata.distances.pushBack(graph_commons::INFINITY_VAL);
            _BFSdata.predecessors.pushBack(graph_commons::INFINITY_VAL);
        }

        size_t numReachedNodes = _graph.runBFS(*sourceIdx, _BFSdata.distances, _BFSdata.predecessors, _BFSdata.traversal);

        if(numReachedNodes < _graph.getNumNodes()) 
            _isConnected = BOOL_STATES::FALSE;
        else
            _isConnected = BOOL_STATES::TRUE;
    }

    size_t getBFSdistanceTo(const T& destinyNode)
    {   
        size_t* destinyIdx = _nodeToId.find(destinyNode);

        if(destinyIdx == nullptr || _BFSdata.distances.empty() || _BFSdata.distances[*destinyIdx] == graph_commons::INFINITY_VAL)
            return graph_commons::INFINITY_VAL;

        return _BFSdata.distances[*destinyIdx];
    }

    Vector<T>& getBFSpathTo(const T& destinyNode)
    {
        _BFSdata.path.clear();

        size_t* destinyNodeIdx = _nodeToId.find(destinyNode);

        if(destinyNodeIdx == nullptr || _BFSdata.distances[*destinyNodeIdx] == graph_commons::INFINITY_VAL)
            return _BFSdata.path; 


        Vector<size_t> pathIdx; pathIdx.pushBack(*destinyNodeIdx);
        size_t currentNodeIdx = *destinyNodeIdx;
        
        while(_BFSdata.predecessors[currentNodeIdx] != currentNodeIdx)
        {
            currentNodeIdx = _BFSdata.predecessors[currentNodeIdx];
            pathIdx.pushBack(currentNodeIdx);    
        }

        for(size_t i = pathIdx.size(); i > 0; i--)
        {
            _BFSdata.path.pushBack(_idToNode[pathIdx[i - 1]]);
        }
        
        return _BFSdata.path;
    }

    Vector<T>& getBFSTraversedPath()
    {
        _BFSdata.TraversedPath.clear();
        
        for(size_t i = 0; i < _BFSdata.traversal.size(); i++)
        {
            _BFSdata.TraversedPath.pushBack(_idToNode[_BFSdata.traversal[i]]);
        }
        return _BFSdata.TraversedPath;
    }

    // ==========================================
    // DEPTH-FIRST SEARCH (DFS)
    // ==========================================
    void runDFS(const T& sourceNode)
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