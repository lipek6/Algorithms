/*
    - This is a structure that will be used internally. 
    - There will be a wrapper for it to be used in a higher level of abstraction.
    - Directed / Undirected graphs are gonna be handled by the wrapper class.
*/
#pragma once
#include "../Vector.h"


struct NoWeight {};
template <typename W = NoWeight>
class AL
{
    template <typename T, typename _W> friend class Graph;


private:
    struct Edge { size_t node; W weight; };
    Vector<Vector<Edge>> topology;
    Vector<bool> isActive;
    size_t activeNodesCount;

// I should probably seek to reuse the !isActive slots of the AL.topology
public:
    AL() : activeNodesCount(0) {}
    ~AL() {}
    
    size_t addNode()
    {
        topology.pushBack(Vector<Edge>());
        isActive.pushBack(true);
        activeNodesCount++;

        return topology.size() - 1;
    }

    void addEdge(const size_t sourceIdx, const size_t destinyIdx, const W weight = W())
    {
        if((sourceIdx < topology.size() && destinyIdx < topology.size()) && (isActive[sourceIdx] && isActive[destinyIdx]))
            topology[sourceIdx].pushBack({destinyIdx, weight});
    }

    void removeNode(const size_t delIdx) 
    { 
        if(delIdx < topology.size() && isActive[delIdx])
        {
            isActive[delIdx] = false;
            topology[delIdx] = Vector<Edge>();
            activeNodesCount--;
        }
    }

    void removeEdge(const size_t sourceIdx, const size_t destinyIdx)
    {
        for(size_t i = 0; i < topology[sourceIdx].size(); i++)
        {
            if(topology[sourceIdx][i].node == destinyIdx)
            {
                topology[sourceIdx][i] = topology[sourceIdx][topology[sourceIdx].size() - 1];
                topology[sourceIdx].popBack();
                break;
            }
        }
    }
};