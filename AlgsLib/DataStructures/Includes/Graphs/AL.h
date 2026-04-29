#pragma once
#include "GraphsCommons.h"
#include "../Vector.h"
#include "../Queue.h"
#include "../Stack.h"

template <typename W = graph_commons::NoWeight>
class AL
{
private:
    // ==========================================
    // CORE DATA STRUCTURES
    // ==========================================
    struct Edge { size_t node; W weight; };
    Vector<Vector<Edge>> _topology;

    Vector<bool> _isActive;
    Vector<size_t> _freeIds;
    size_t _activeNodesCount;
    
public:
    // ==========================================
    // CONSTRUCTOR & DESTRUCTOR
    // ==========================================
    AL() : _activeNodesCount(0) {}
    ~AL() {}
    
    // ==========================================
    // STRUCTURE MODIFIERS (ADD/REMOVE)
    // ==========================================
    size_t addNode()
    {
        if(!_freeIds.empty())
        {
            size_t recycledId = _freeIds.popBack();
            _isActive[recycledId] = true;
            _activeNodesCount++;
            return recycledId;
        }
        else
        {
            _topology.pushBack(Vector<Edge>());
            _isActive.pushBack(true);
            _activeNodesCount++;
            return _topology.size() - 1;
        }
    }

    void addEdge(const size_t sourceIdx, const size_t destinyIdx, const W weight = W())
    {
        if((sourceIdx < _topology.size() && destinyIdx < _topology.size()) && 
           (_isActive[sourceIdx] && _isActive[destinyIdx]))
        {
            _topology[sourceIdx].pushBack({destinyIdx, weight});
        }
    }

    void removeNode(const size_t idx) 
    { 
        if(idx < _topology.size() && _isActive[idx])
        {
            for(size_t i = 0; i < _topology.size(); i++)
            {
                if(_isActive[i] && i != idx)    
                    removeEdge(i, idx);
            }

            _isActive[idx] = false;
            _topology[idx] = Vector<Edge>();
            _freeIds.pushBack(idx);
            _activeNodesCount--;
        }
    }

    void removeEdge(const size_t sourceIdx, const size_t destinyIdx)
    {
        for(size_t i = 0; i < _topology[sourceIdx].size(); i++)
        {
            if(_topology[sourceIdx][i].node == destinyIdx)
            {
                _topology[sourceIdx].unorderedRemove(i);
                break;
            }
        }
    }    

    // ==========================================
    // GRAPH QUERIES (GETTERS)
    // ==========================================
    size_t getNumNodes() const 
    { 
        return _activeNodesCount; 
    }

    size_t getCapacity() const 
    { 
        return _topology.size(); 
    }

    size_t getNumAdjacentNodes(const size_t nodeIdx) const
    { 
        if (nodeIdx < _topology.size() && _isActive[nodeIdx])
            return _topology[nodeIdx].size(); 
        return 0;
    }

    // ==========================================
    // GRAPH ALGORITHMS
    // ==========================================
    size_t runBFS(const size_t sourceIdx, Vector<size_t>& distancesVector, Vector<size_t>& predecessorsVector, Vector<size_t>& traversalVector) 
    {
        if (sourceIdx >= _topology.size() || !_isActive[sourceIdx]) 
            return 0;

        Queue<size_t> queue; 
        queue.push(sourceIdx);
        
        distancesVector[sourceIdx] = 0;
        predecessorsVector[sourceIdx] = sourceIdx;
        traversalVector.pushBack(sourceIdx);
        
        size_t numReachedNodes = 1;

        while(!queue.empty())
        {
            size_t currentNodeIdx = queue.front();
            queue.pop();

            for(size_t i = 0; i < _topology[currentNodeIdx].size(); i++)
            {
                size_t neighborIdx = _topology[currentNodeIdx][i].node;

                if(distancesVector[neighborIdx] != graph_commons::INFINITY_VAL)
                    continue;

                distancesVector[neighborIdx] = distancesVector[currentNodeIdx] + 1;
                predecessorsVector[neighborIdx] = currentNodeIdx;
                
                queue.push(neighborIdx);

                traversalVector.pushBack(neighborIdx);
                numReachedNodes++;
            }
        }
        return numReachedNodes;
    }

    size_t runDFS(const size_t sourceIdx, Vector<size_t>& visiteds)
    {
        if (sourceIdx >= _topology.size() || !_isActive[sourceIdx]) 
            return 0;

        size_t numReachedNodes = 1; 
        visiteds[sourceIdx] = 0;

        for(size_t i = 0; i < _topology[sourceIdx].size(); i++)
        {
            size_t neighborIdx = _topology[sourceIdx][i].node;

            if(visiteds[neighborIdx] == graph_commons::INFINITY_VAL)
            {
                numReachedNodes += runDFS(neighborIdx, visiteds); 
            }
        }
        
        return numReachedNodes; 
    }
};