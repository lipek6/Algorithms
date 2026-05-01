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
// Stack.h
// ====================================================================================
template <typename T>
class Stack
{
private:
    Vector<T> vector;

public:
    void push(const T& newData) { vector.pushBack(newData); }
    T pop() { return vector.popBack(); }
    
    T& top() { return vector.back(); }
    T& top() const { return vector.back(); }

    size_t size() const { return vector.size(); }
    bool empty() const { return vector.empty(); }
};









struct Edge { size_t node; W weight; };
// ====================================================================================
// AL.h
// ====================================================================================
template <typename W = graph_commons::NoWeight>
class AL
{
private:
    // ==========================================
    // CORE DATA STRUCTURES
    // ==========================================
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

    const Vector<Edge>& getNeighbors(size_t nodeIdx) const
    {
        if(nodeIdx < _topology.size() && _isActive[nodeIdx])
            return _topology[nodeIdx];
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







int main(void)
{
    size_t numPeople; std::cin >> numPeople;


    Vector<long long> influence(numPeople + 1, 0);
    
    for(size_t i = 1; i <= numPeople; i++)
        std::cin >> influence[i];
    

    AL tree;


    for(size_t i = 0; i < numPeople - 1; i++)
    {
        std::cin >> boss >> subordinate;
        tree.addEdge(boss, subordinate);
        tree.addEdge(subordinate, boss);
    }

    Vector<size_t> distances(numPeople + 1, graph_commons::INFINITY_VAL);
    Vector<size_t> predecessors(numPeople + 1, graph_commons::INFINITY_VAL);
    Vector<size_t> traversal;
    tree.runBFS(1, distances, predecessors, traversal);


}