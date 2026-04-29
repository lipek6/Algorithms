#pragma once
#include "GraphsCommons.h"
#include "../Vector.h"


// flattening the 2D data into a single 1D vector (width * height) and calculating offsets manually for better memory locality.

template <typename W = graph_commons::NoWeight>
class AM
{
private:
    Vector<size_t> _matrix;

public:
    AM() {}

    size_t addNode()
    {

    }

    void addEdge(const size_t sourceIdx, const size_t destinyIdx, const W weight = W())
    {

    }

    void removeNode(const size_t idx) 
    {

    }

    void removeEdge(const size_t sourceIdx, const size_t destinyIdx)
    {

    }

    size_t getNumNodes() const 
    {

    }

    size_t getCapacity() const 
    {

    }

    size_t getNumAdjacentNodes(const size_t nodeIdx) const
    {
        
    }

    size_t runBFS(const size_t sourceIdx, Vector<size_t>& distancesVector, Vector<size_t>& predecessorsVector, Vector<size_t>& traversalVector) 
    {

    }

    size_t runDFS(const size_t sourceIdx, Vector<size_t>& visiteds)
    {

    }

};