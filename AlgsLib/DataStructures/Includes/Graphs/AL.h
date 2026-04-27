/*
    - This is a structure that will be used internally. 
    - There will be a wrapper for it to be used in a higher level of abstraction.
    - Directed / Undirected graphs are gonna be handled by the wrapper class.


    I am avoiding to use a stack or queue in place of the vectors of isActive and freeIds.
    While using a stack or queue is more logically accurate, it would create a unecessary
    dependency. I think that avoiding using these is a better choice for a low level thing 
    like this, so I will probably stick with it.

*/
#pragma once
#include "GraphsCommons.h"
#include "../Vector.h"
#include "../Queue.h"

template <typename W = graph_commons::NoWeight>
class AL
{
    template <typename T, typename _W> friend class Graph;

private:
    struct Edge { size_t node; W weight; };
    Vector<Vector<Edge>> topology;

    Vector<bool> isActive;
    Vector<size_t> freeIds;
    size_t activeNodesCount;
    
public:
    AL() : activeNodesCount(0) {}
    ~AL() {}
    
    size_t addNode()
    {
        if(!freeIds.empty())
        {
            size_t recycledId = freeIds.popBack();
            isActive[recycledId] = true;
            activeNodesCount++;
            return recycledId;
        }
        else
        {
            topology.pushBack(Vector<Edge>());
            isActive.pushBack(true);
            activeNodesCount++;
            return topology.size() - 1;
        }

    }

    void addEdge(const size_t sourceIdx, const size_t destinyIdx, const W weight = W())
    {
        if((sourceIdx < topology.size() && destinyIdx < topology.size()) && (isActive[sourceIdx] && isActive[destinyIdx]))
            topology[sourceIdx].pushBack({destinyIdx, weight});
    }

    void removeNode(const size_t idx) 
    { 
        if(idx < topology.size() && isActive[idx])
        {
            for(size_t i = 0; i < topology.size(); i++)
                if(isActive[i] && i != idx)    
                    removeEdge(i, idx);
            

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

    size_t BFS(const size_t sourceIdx, Vector<size_t>& distancesVector) 
    {
        Queue<size_t> queue; queue.push(sourceIdx);
        distancesVector[sourceIdx] = 0;
        size_t numReachedNodes = 0;

        while(!queue.empty())
        {
            size_t currentNodeIdx = queue.front();
            queue.pop();

            for(size_t i = 0; i < topology[currentNodeIdx].size(); i++)
            {
                size_t neighborIdx = topology[currentNodeIdx][i].node;

                if(distancesVector[neighborIdx] != graph_commons::INFINITY)
                    continue;

                distancesVector[neighborIdx] = distancesVector[currentNodeIdx] + 1;
                queue.push(neighborIdx);
                numReachedNodes++;
            }
        }
        return numReachedNodes;
    }

    void DFS(const size_t sourceIdx, Vector<size_t>& visiteds)
    {
        visiteds[sourceIdx] = 0;

        for(size_t i = 0; i < topology[sourceIdx].size(); i++)
        {
            size_t neighborIdx = topology[sourceIdx][i].node;

            if(visiteds[neighborIdx] == graph_commons::INFINITY)
                DFS(neighborIdx, visiteds);
        }
    }
};