#pragma once
#include "GraphsCommons.h"
#include "../Vector.h"
#include "../Queue.h"
#include "../Stack.h"



// flattening the 2D data into a single 1D vector (width * height) and calculating offsets manually for better memory locality.

template <typename W = graph_commons::NoWeight>
class AM
{
private:
    struct EdgeCell 
    {
        bool exists = false; 
        W weight = W();
    };

    Vector<EdgeCell> _matrix;
    Vector<bool> _isActive;
    Vector<size_t> _freeIds;
    
    size_t _gridCapacity;
    size_t _activeNodesCount;
    size_t _RESIZE_FACTOR;

    inline size_t idx(const size_t row, const size_t col) const { return row * _gridCapacity + col; }
    
    void expandGrid()
    {
        size_t oldCapacity = _gridCapacity;
        size_t newCapacity = (oldCapacity == 0) ? 4 : oldCapacity * _RESIZE_FACTOR;

        Vector<size_t> newMatrix(newCapacity * newCapacity, EdgeCell());

        for(size_t row = 0; row < oldCapacity; row++)
        {
            for(size_t col = 0; col < oldCapacity; col++)
            {
                newMatrix[(row * newCapacity) + col] = _matrix[(row * oldCapacity) + col];
            }
        }

        _gridCapacity = newCapacity;
        _matrix = std::move(newMatrix);
    }

public:
    AM() : _gridCapacity(0), _activeNodesCount(0), _RESIZE_FACTOR(2) {}

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
            if(_gridCapacity == _activeNodesCount)
                expandGrid();

            _isActive.pushBack(true);
            _activeNodesCount++;

            return _activeNodesCount - 1;
        }
    }

    void addEdge(const size_t sourceIdx, const size_t destinyIdx, const W weight = W())
    {
        if((sourceIdx < _gridCapacity && destinyIdx < _gridCapacity) && (_isActive[sourceIdx] && _isActive[destinyIdx]))
            _matrix[idx(sourceIdx, destinyIdx)] = {true, weight};
    }
    
    void removeNode(const size_t nodeIdx) 
    {
        if(nodeIdx < _gridCapacity && _isActive[nodeIdx])
        {
            _isActive[nodeIdx] = false;
            _freeIds.pushBack(nodeIdx);
            _activeNodesCount--;

            for(size_t col = 0; col < _gridCapacity; col++)                 // Honest cleanup
                _matrix[idx(nodeIdx, col)] = EdgeCell();           

            for(size_t row = 0; row < _gridCapacity; row++)
                _matrix[idx(row, nodeIdx)] = EdgeCell();
        }
    }

    void removeEdge(const size_t sourceIdx, const size_t destinyIdx)
    {
        if(sourceIdx < _gridCapacity && destinyIdx < _gridCapacity)     // HEY, hould I check if they are active? I think not, because if it's a inactive connection, I don't care how if you clean it again            
            _matrix[idx(sourceIdx, destinyIdx)] = EdgeCell();
    }

    size_t getNumNodes() const 
    {
        return _activeNodesCount;
    }

    size_t getCapacity() const 
    {
        return _gridCapacity;
    }

    size_t getNumAdjacentNodes(const size_t nodeIdx) const
    {
        if(nodeIdx >= _gridCapacity || !_isActive[nodeIdx])
            return 0;        
        
        size_t numAdjacentNodes = 0;
        
        for(size_t col = 0; col < _gridCapacity; col++)
            if(_matrix[idx(nodeIdx, col)].exists)
                numAdjacentNodes++;
        
        return numAdjacentNodes;
    }

    size_t runBFS(const size_t sourceIdx, Vector<size_t>& distancesVector, Vector<size_t>& predecessorsVector, Vector<size_t>& traversalVector) 
    {
        if(sourceIdx >= _gridCapacity || !_isActive[sourceIdx])
            return 0;

        Queue<size_t> queue;
        queue.push(sourceIdx);
        
        distancesVector[sourceIdx] = 0;
        predecessorsVector[sourceIdx] = sourceIdx;
        traversalVector.pushBack(sourceIdx);

        size_t numReachedNodes = 1;

        while(!queue.empty())
        {
            size_t currentRow = queue.front(); 
            queue.pop();

            for(size_t neighborCol = 0; neighborCol < _gridCapacity; neighborCol++)     // The neighborCol is the col itself 
            {
                if(!_matrix[idx(currentRow, neighborCol)].exists || distancesVector[neighborCol] != graph_commons::INFINITY_VAL)
                    continue;

                queue.push(neighborCol);
                distancesVector[neighborCol] = distancesVector[currentRow] + 1;
                predecessorsVector[neighborCol] = currentRow;
                traversalVector.pushBack(neighborCol);
                
                numReachedNodes++;
            }
        }
        return numReachedNodes;
    }

    size_t runDFS(const size_t sourceIdx, Vector<size_t>& visiteds)
    {
        // Will do later
        return 0;
    }

    // Maybe add a function to check if a certain node isActive? I can't see a use for it now to be honest.

};