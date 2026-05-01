// ====================================================================================
// INCLUDES
// ====================================================================================
#include <iostream>
#include <exception>
#include <limits>
#include <functional>
#include <optional>









// ====================================================================================
// GraphsCommons.h
// ====================================================================================
namespace graph_commons 
{
    const size_t INFINITY_VAL = std::numeric_limits<size_t>::max();
    struct NoWeight {};
}











// ====================================================================================
// Vector.h
// ====================================================================================
template <typename T>
class Vector
{
private:
    T* array;
    size_t usedSize;
    size_t allocatedArraySize;
    size_t RESIZE_FACTOR;

public:
    Vector(const size_t initialCapacity = 16)
        : array(new T[initialCapacity])
        , usedSize(0)
        , allocatedArraySize(initialCapacity)
        , RESIZE_FACTOR(2)
        {
        }

    Vector(const size_t count, const T& initialValue)
        : array(new T[count * 2])
        , usedSize(count)
        , allocatedArraySize(count * 2)
        , RESIZE_FACTOR(2)
        {
            for(size_t i = 0; i < count; i++)
                array[i] = initialValue;
        }

    ~Vector() { delete[] array; }

    // RULE OF 3: If a class uses a customized destructor, it NEEDS to have a copy constructor and a assignment operator customized for it.
    // This was fucking up the Vector<Vector<Edge>> because of double frees (the copies were just pointers being repeated and freed).

    // Copy constructor
    Vector(const Vector<T>& other) : allocatedArraySize(other.allocatedArraySize), usedSize(other.usedSize), array(new T[other.allocatedArraySize]), RESIZE_FACTOR(other.RESIZE_FACTOR)
    {
        for(size_t i = 0; i < other.usedSize; i++)
            this->array[i] = other.array[i];
    }

    // Assignment operator
    Vector& operator=(const Vector<T>& other)
    {
        if(this == &other) return *this;
        
        delete[] this->array;

        this->allocatedArraySize = other.allocatedArraySize;
        this->usedSize           = other.usedSize;
        this->RESIZE_FACTOR      = other.RESIZE_FACTOR;
        this->array              = new T[allocatedArraySize];

        for(size_t i = 0; i < other.usedSize; i++)
            this->array[i] = other.array[i];
        
        return *this;
    }

    void SetResizeFactor(const size_t newFactor) { RESIZE_FACTOR = (newFactor > 1) ? newFactor : 2; }

    T& operator[](size_t index) { return array[index]; }
    const T& operator[](size_t index) const { return array[index]; }

    void resize(const size_t newAllocatedSize)
    {
        if(allocatedArraySize >= newAllocatedSize) return;
        
        T* oldArray = array;
        array = new T[newAllocatedSize];

        for(size_t i = 0; i < usedSize; i++)
            array[i] = oldArray[i];

        allocatedArraySize = newAllocatedSize;
        delete[] oldArray;
    }


    void pushBack(const T& newData)
    {
        if(usedSize == allocatedArraySize)
            resize(allocatedArraySize * RESIZE_FACTOR);
        
        array[usedSize++] = newData;
    }

    T popBack()
    {
        usedSize--;
        return array[usedSize];
    }

    void remove(size_t index)
    {
        if(index >= usedSize) return;

        for(size_t i = index; i < usedSize - 1; i++)
            array[i] = array[i + 1];
                
        usedSize--;
    }

    // Constant time remove. Changes the idx of the last element. Do not use if the order of elements is important.
    void unorderedRemove(size_t index)
    {
        if(index >= usedSize) return;
    
        array[index] = array[usedSize - 1];
        popBack();
    }

    T& front()
    {
        if(usedSize != 0)
            return array[0]; 
        else
            throw std::out_of_range("Empty vector");
    }
    
    const T& front() const
    {
        if(usedSize != 0)
            return array[0]; 
        else
            throw std::out_of_range("Empty vector");
    }

    T& back()
    {
        if(usedSize != 0)
            return array[usedSize - 1];
        else
            throw std::out_of_range("Empty vector");
    }

    const T& back() const
    {
        if(usedSize != 0)
            return array[usedSize - 1];
        else
            throw std::out_of_range("Empty vector");
    }

    void reverse(const size_t beginIndex = 0, const size_t endIndex = size() - 1)
    {
        if(beginIndex >= endIndex || endIndex >= usedSize) return;

        size_t i = beginIndex;
        size_t j = endIndex;
        
        while(i < j)
            std::swap(array[i++], array[j--]);
    }

    void print(std::ostream& stream = std::cout) const
    {
        if(usedSize == 0) return;

        stream << "[";
        for(size_t i = 0; i < usedSize - 1; i++)
            stream << array[i] << ", ";    
        stream << array[usedSize - 1] << "]\n";
    }

    void printReverse(std::ostream& stream = std::cout) const
    {
        if(usedSize == 0) return;
        
        stream << "[";
        for(size_t i = usedSize - 1; i > 0; i--)
            stream << array[i] << ", ";    
        stream << array[0] << "]\n";
    }

    bool empty() const { return usedSize == 0; }
    size_t size() const { return usedSize; }
    size_t allocatedSize() const {return allocatedArraySize; }

    void clear() { usedSize = 0; }

};










// ====================================================================================
// Queue.h
// ====================================================================================
template <typename T>
class Queue
{
private:
    T* array;
    size_t usedSize;
    size_t allocatedArraySize;
    size_t RESIZE_FACTOR;
    size_t begin;
    size_t end;

public:
    Queue(const size_t initialSize = 16)
        : array(new T[initialSize])
        , usedSize(0)
        , allocatedArraySize(initialSize)
        , RESIZE_FACTOR(2)
        , begin(0)
        , end(0)
    {
    }
    
    ~Queue() { delete[] array; }

    Queue(const Queue<T>& other)
        : array(new T[other.allocatedArraySize])
        , usedSize(other.usedSize)
        , allocatedArraySize(other.allocatedArraySize)
        , RESIZE_FACTOR(other.RESIZE_FACTOR)
        , begin(0)
        , end(other.usedSize)
    {
        for(size_t i = 0; i < other.usedSize; i++)
            this->array[i] = other.array[(other.begin + i) % other.allocatedArraySize];
    }

    Queue& operator=(const Queue<T>& other)
    {
        if(this == &other) return *this;

        delete[] this->array;

        this->array                 = new T[other.allocatedArraySize];
        this->usedSize              = other.usedSize;
        this->allocatedArraySize    = other.allocatedArraySize;
        this->RESIZE_FACTOR         = other.RESIZE_FACTOR;
        this->begin                 = 0;
        this->end                   = other.usedSize;

        for(size_t i = 0; i < other.usedSize; i++)
            this->array[i] = other.array[(other.begin + i) % other.allocatedArraySize];

        return *this;
    }

    bool empty() const { return usedSize == 0; }

    size_t size() const { return usedSize; }

    size_t allocatedSize() const { return allocatedArraySize; }

    void SetResizeFactor(const size_t newFactor) { RESIZE_FACTOR = (newFactor > 1) ? newFactor : 2; }
    
    T& front()
    {
        if(usedSize == 0)
            throw std::out_of_range("Empty queue, there is no front");
        else
            return array[begin];
    } 
    
    const T& front() const
    {
        if(usedSize == 0)
            throw std::out_of_range("Empty queue, there is no front");
        else
            return array[begin];
    } 

    void pop()
    {
        if(usedSize == 0)
            throw std::out_of_range("Cannot remove from empty queue");
        
        begin = (begin + 1) % allocatedArraySize;
        usedSize--;
    }

    void push(const T& newData)
    {
        if(usedSize == allocatedArraySize)
            resize(allocatedArraySize * RESIZE_FACTOR);

        array[end] = newData;
        end = (end + 1) % allocatedArraySize;
        usedSize++;
    }

    void resize(const size_t newAllocatedSize)
    {
        T* oldArray = array;
        array = new T[newAllocatedSize];

        for(size_t i = 0; i < usedSize; i++)
            array[i] = oldArray[(begin + i) % allocatedArraySize];

        delete[] oldArray;
        
        begin = 0;
        end = usedSize;
        allocatedArraySize = newAllocatedSize;
    }

    void clear()
    {
        usedSize = 0;
        begin    = 0;
        end      = 0;
    }
};











// ====================================================================================
// ImplicitGraph.h
// ====================================================================================
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
    
    Vector<size_t> getHeatMap(const Vector<cell>& sourceCells, const T& obstacle)
    {
        Vector<T> singleObstacle(1, obstacle);
        return getHeatMap(sourceCells, singleObstacle);
    }

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
                

                if(heatMap[neighborId] != graph_commons::INFINITY_VAL)
                    continue;
                
                
                for(size_t i = 0; i < obstacles.size(); i++)
                {
                    if(_grid[neighborId] != obstacles[i])
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



int main(void)
{
    size_t numRows, numCols; std::cin >> numRows >> numCols;
    
    enum class CELL_STATE {CLEAN, BLOCKED, POST};
    
    ImplicitGraph<CELL_STATE> map(numRows, numCols);
    Vector<cell> posts;

    for(size_t row = 0; row < numRows; row++)
    {
        for(size_t col = 0; col < numCols; col++)
        {
            char cell; std::cin >> cell;

            switch(cell)
            {
                case('.'):
                    map.set(row, col, CELL_STATE::CLEAN);
                    break;

                case('#'):
                    map.set(row, col, CELL_STATE::BLOCKED);
                    break;

                case('P'):
                    map.set(row, col, CELL_STATE::POST);
                    posts.pushBack({row, col});
                    break;

                default:
                    throw std::invalid_argument("Bad input");
            }
        }
    }

    Vector<size_t> heatMap = map.getHeatMap(posts, CELL_STATE::BLOCKED);

    struct cellInfo
    {
        size_t row;
        size_t col;
        size_t distance;
    };

    cellInfo worstCell = {posts[0].row + 1, posts[0].col + 1, 0};

    for(size_t row = 0; row < numRows; row++)
    {
        for(size_t col = 0; col < numCols; col++)
        {
            size_t cellIdx = row * numCols + col;

            if(heatMap[cellIdx] == graph_commons::INFINITY_VAL)
                std::cout << "-1";
            else
            {
                if(worstCell.distance < heatMap[cellIdx])
                    worstCell = {row + 1, col + 1, heatMap[cellIdx]};
                
                std::cout << heatMap[cellIdx];
            }

            if(col != numCols - 1) std::cout << " ";

        }
        std::cout << "\n";
    }
    std::cout << worstCell.row << " " << worstCell.col << " " << worstCell.distance << "\n";
}