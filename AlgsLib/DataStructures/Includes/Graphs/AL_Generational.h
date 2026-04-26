#pragma once

// I am connected with a node that is of the 4th generation, but our edge is of the 2th generation, that means that this edge is a ghost!
#include "../Vector.h"


struct NoWeight {};

template <typename W = NoWeight>
class AL_Generational
{
    template <typename T, typename _W> friend class Graph;

private:
    struct Edge { size_t node; W weight; size_t targetGeneration; };
    Vector<Vector<Edge>> topology;
    
    Vector<bool> isActive;
    Vector<size_t> freeIds;
    Vector<size_t> generation;
    size_t activeNodesCount;

public:
    AL_Generational() : activeNodesCount(0) {}
    ~AL_Generational() {}
    
    size_t addNode()
    {
        if(!freeIds.empty())
        {
            size_t recycledId = freeIds.popBack();
            isActive[recycledId] = true;
            activeNodesCount++;
            generation[recycledId]++;
            return recycledId;
        }
        else
        {
            topology.pushBack(Vector<Edge>());
            isActive.pushBack(true);
            activeNodesCount++;
            generation.pushBack(0);
            return topology.size() - 1;
        }

    }

    void addEdge(const size_t sourceIdx, const size_t destinyIdx, const W weight = W())
    {
        if((sourceIdx < topology.size() && destinyIdx < topology.size()) && (isActive[sourceIdx] && isActive[destinyIdx]))
            topology[sourceIdx].pushBack({destinyIdx, weight, generation[destinyIdx]});
    }

    void removeNode(const size_t idx) 
    { 
        if(idx < topology.size() && isActive[idx])
        {            
            isActive[idx] = false;
            topology[idx] = Vector<Edge>();
            freeIds.pushBack(idx);
            activeNodesCount--;
        }
    }

    void removeEdge(const size_t sourceIdx, const size_t destinyIdx)
    {
        for(size_t i = 0; i < topology[sourceIdx].size(); i++)
        {
            if(topology[sourceIdx][i].node == destinyIdx)
            {
                topology[sourceIdx].unorderedRemove(i);
                break;
            }
        }
    }
};