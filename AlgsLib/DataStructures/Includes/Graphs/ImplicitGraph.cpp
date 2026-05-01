// #pragma once
#include <iostream>
#include "GraphsCommons.h"
#include "Vector.h"
#include "Queue.h"

struct cell {size_t row; size_t col;};

template <typename T = char>
class ImplicitGraph
{
private:
    Vector<T> _grid;
    size_t _numRows;
    size_t _numCols;

    inline size_t idx(const size_t row, const size_t col) const { return row * _numCols + col; }

public:
    ImplicitGraph(size_t rows, size_t cols) : _grid(rows * cols, T()), _numRows(rows), _numCols(cols) {}

    void set(const size_t row, const size_t col, const T& newData)
    {
        _grid[idx(row, col)] = newData;
    }

    T& get(const size_t row, const size_t col)
    {
        return _grid[idx(row, col)];
    }

    const T& get(const size_t row, const size_t col) const
    {
        return _grid[idx(row, col)];
    }

    void buildFromVector(const Vector<T>& inVector, const size_t numRows, const size_t numCols)
    {
        if(inVector.size() == numRows * numCols)
        {
            _numRows = numRows;
            _numCols = numCols;
            
            _grid = inVector;
        }
    }

    void moveFromVector(Vector<T>& inVector, const size_t numRows, const size_t numCols)
    {
        if(inVector.size() == numRows * numCols)
        {
            _numRows = numRows;
            _numCols = numCols;

            _grid = std::move(inVector);
        }
    }

    void addRow(const size_t numNewRows = 1)
    {
        size_t totalNewCells = numNewRows * _numCols;

        for(size_t i = 0; i < totalNewCells; i++)
        {
            _grid.pushBack(T());
        }

        _numRows += numNewRows;
    }

    void addCol(const size_t numNewCols = 1)
    {
        const size_t newTotalNumCols = numNewCols + _numCols;
        Vector<T> newGrid(newTotalNumCols * _numRows, T());         // This "T()" initialization is worth it? It will cause some overhead...
        
        for(size_t row = 0; row < _numRows; row++)
        {
            for(size_t col = 0; col < _numCols; col++)
            {
                newGrid[row * newTotalNumCols + col] = std::move(_grid[row * _numCols + col]);
            }
        }

        _numCols = newTotalNumCols;
        _grid = std::move(newGrid);
    }
    
    size_t getNumRows() const { return _numRows; }

    size_t getNumCols() const { return _numCols; }
    
    Vector<size_t> getHeatMap(const Vector<cell>& sourceCells, const Vector<T>& obstacles)
    {
        Vector<size_t> heatMap(_numRows * _numCols, graph_commons::INFINITY_VAL);
        
        Queue<size_t> queue;
        for(size_t i = 0; i < sourceCells.size(); i++)
        {
            size_t startId = idx(sourceCells[i].row, sourceCells[i].col);
            
            queue.push(startId);
            heatMap[startId] = 0;
        }

        int dRow[4] = {-1, +1,  0,  0};       // UP, DOWN, LEFT, RIGHT
        int dCol[4] = { 0,  0, -1, +1};       // UP, DOWN, LEFT, RIGHT

        while(!queue.empty())
        {
            size_t currentId = queue.front(); queue.pop();
            int currentRow = currentId / _numCols;      // Review this later
            int currentCol = currentId % _numCols; 

            for(int dir = 0; dir < 4; dir++)
            {
                int neighborRow = currentRow + dRow[dir];
                int neighborCol = currentCol + dCol[dir];

                if(neighborCol < 0 || neighborRow < 0 || neighborCol >= (int)_numCols || neighborRow >= (int)_numRows)
                    continue;
                

                size_t neighborId = idx(neighborRow, neighborCol);
                

                if(_grid(neighborId) != graph_commons::INFINITY_VAL)
                    continue;
                
                
                for(size_t i = 0; i < obstacles.size(); i++)
                {
                    if(_grid(neighborId) != obstacles[i])
                    {
                        heatMap[neighborId] = heatMap[currentId] + 1;
                        queue.push(neighborId);
                    }
                    // Else, mark it as an obstacle. That will need to be something different from INFINITY. hmmm.....
                }    
            }
        }
        return heatMap;
    }
};