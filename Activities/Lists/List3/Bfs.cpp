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









// ====================================================================================
// Hash.h
// ====================================================================================
template <typename K, typename V, typename HashFunction = std::hash<K>>
class HashTable_Closed
{
private:
    // ==========================================
    // INTERNAL DATA STRUCTURES & STATE
    // ==========================================
    
    enum State {EMPTY, OCCUPIED, DELETED};                         

    struct HashEntry
    {
        K key;
        V value;
        State state;

        HashEntry() : state(EMPTY) {}
        HashEntry(K key, V value, State state) : key(key), value(value), state(state) {}
    };

    HashEntry* table;
    HashFunction hasher; 

    size_t allocatedSize;
    size_t usedSize;
    size_t totalNodes;

    size_t RESIZE_FACTOR;
    float MAX_LOAD_FACTOR;

    // ==========================================
    // INTERNAL HELPERS
    // ==========================================

    size_t getHashedIndex(const K& key) const { return hasher(key) % allocatedSize; }
   
    void insertUnchecked(const K& newKey, const V& newData)
    {
        size_t index = getHashedIndex(newKey);
        std::optional<size_t> firstDeletedSlot = std::nullopt;

        while(table[index].state != EMPTY)
        {
            if(table[index].state == OCCUPIED && table[index].key == newKey) // Update value
            {
                table[index].value = newData;
                return;
            }
            if(table[index].state == DELETED && !firstDeletedSlot.has_value())
                firstDeletedSlot = index;

            index = (index + 1) % allocatedSize;
        }
        
        if(firstDeletedSlot.has_value())
            table[firstDeletedSlot.value()] = HashEntry(newKey, newData, OCCUPIED);
        else
        {
            table[index] = HashEntry(newKey, newData, OCCUPIED);
            totalNodes++;
        }
    
        usedSize++;
    }

    void rehashToExactSize(const size_t newAllocatedSize)
    {
        size_t oldAllocatedSize = allocatedSize;
        HashEntry* oldTable = table;
        
        allocatedSize = newAllocatedSize;
        table = new HashEntry[allocatedSize];
        usedSize = 0;

        for(size_t i = 0; i < oldAllocatedSize; i++)
        {
            if(oldTable[i].state == OCCUPIED)
                this->insertUnchecked(oldTable[i].key, oldTable[i].value);
        }

        delete[] oldTable;
        totalNodes = usedSize;
    }
    
    std::optional<size_t> findIndex(const K& targetKey) const
    {
        size_t index = getHashedIndex(targetKey);
        
        while(table[index].state != EMPTY)
        {
            if(table[index].state == OCCUPIED && table[index].key == targetKey)
                return index;

            index = (index + 1) % allocatedSize;
        }

        return std::nullopt;
    }

public:
    // ==========================================
    // CONSTRUCTORS & DESTRUCTORS 
    // ==========================================
    
    HashTable_Closed(const size_t initialSize = 16, const float maxLoadFactor = 0.5, const size_t resizeFactor = 2)     
        : allocatedSize(initialSize), usedSize(0), totalNodes(0), MAX_LOAD_FACTOR(maxLoadFactor > 0 ? maxLoadFactor : 0.5), RESIZE_FACTOR(resizeFactor > 1 ? resizeFactor : 2), table(new HashEntry[initialSize]) {}

    ~HashTable_Closed() { delete[] table; }

    // ==========================================
    // CAPACITY MANAGEMENT
    // ==========================================

    size_t getUsedSize() const { return usedSize; }
    size_t getAllocatedSize() const { return allocatedSize; }
    bool empty() const { return usedSize == 0; }

    void reserve(const size_t newCapacity)
    {
        if(newCapacity > allocatedSize)
            rehashToExactSize(newCapacity);
    }

    // Puts the size on the edge of resizing
    void shrinkToFit()
    {
        size_t idealSize = static_cast<size_t>(usedSize / MAX_LOAD_FACTOR) + 1;
        
        if(idealSize < 16)
            idealSize = 16;
        if(idealSize < allocatedSize)
            rehashToExactSize(idealSize);
    }

    // Add function to change resize_fact
    // Add function to change max_capacity_factor
    
    // ==========================================
    // MODIFIERS
    // ==========================================

    void insert(const K& newKey, const V& newData)
    {
        if(float(usedSize) / allocatedSize > MAX_LOAD_FACTOR)
            rehashToExactSize(allocatedSize * RESIZE_FACTOR);

        insertUnchecked(newKey, newData);
    }

    void remove(const K& delKey)
    {
        std::optional<size_t> delIndex = findIndex(delKey);

        if(!delIndex.has_value()) throw std::out_of_range("Key to be removed is not present in the Hash Table");
        
        table[delIndex.value()].state = DELETED;              // Lazy deletion, that data is still there, but that probably is enough for most use cases, I might implement a bool option about this latter
        usedSize--;
    }

    // ==========================================
    // ELEMENT ACCESS
    // ==========================================

    V* find(const K& targetKey)
    {
        std::optional<size_t> targetIndex = findIndex(targetKey);
        if(targetIndex.has_value())
            return &table[targetIndex.value()].value;
        
        return nullptr;
    }

    const V* find(const K& targetKey) const
    {
        std::optional<size_t> targetIndex = findIndex(targetKey);
        if(targetIndex.has_value())
            return &table[targetIndex.value()].value;
        
        return nullptr;
    }
};








size_t nameHasher(const std::string& string)
{
    for(size_t i = 0; i < string.size(); i++)
        


}

// ====================================================================================
// Graph.h
// ====================================================================================
template <typename T = int, typename W = graph_commons::NoWeight>
class Graph
{
    friend class Family;
private:
    // ==========================================
    // INTERNAL ENGINE & CORE MAPPINGS
    // ==========================================
    AL<W> _graph;  
    
    HashTable_Closed<T, size_t, nameHasher> _nodeToId;              // Using custom hasher just because of this question        
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
        if(_nodeToId.find(newNode) != nullptr) return;

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
    size_t runBFS(const T& sourceNode)
    {
        size_t* sourceIdx = _nodeToId.find(sourceNode);
        if(sourceIdx == nullptr) return 0;
        
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
        
        return numReachedNodes;
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

    Vector<T>& getBSTraversedPath()
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










class Family
{
private:
    Graph<std::string> _parents{true};
    Graph<std::string> _childs{true};

    std::string _patriarchName;
    size_t _patriarchId;

public:
    void addParentChild(const std::string& parent, const std::string& child)
    {
        _parents.addNode(parent);
        _parents.addNode(child);
        _parents.addEdge(parent, child);

        _childs.addNode(parent);
        _childs.addNode(child);
        _childs.addEdge(child, parent);
    }

    std::string& getPatriarch() 
    {
        size_t numPatriarchDescendants = 0;
        size_t patriarchId = 0;
        for(size_t i = 0; i < _parents._graph.getCapacity(); i++)
        {
            if(_parents._inDegrees[i] == 0)
            {
                size_t reachedDescendants = _parents.runBFS(_parents._idToNode[i]);
                if(reachedDescendants > numPatriarchDescendants)
                {
                    numPatriarchDescendants = reachedDescendants;   
                    patriarchId = i;
                }
            }
        }
        _patriarchId = patriarchId;
        _patriarchName = _parents._idToNode[_patriarchId];
        return _patriarchName;
    }

    std::string getKinship(const std::string& memberA, const std::string& memberB)
    {
        // Go DOWN from A to see if B is a descendant
        _parents.runBFS(memberA);
        size_t distance = _parents.getBFSdistanceTo(memberB);

        switch(distance)
        {
            case(graph_commons::INFINITY_VAL):          // Maybe on the other graph
                break;
            case(1):
                return "filho";
            case(2):
                return "neto";
            case(3):
                return "bisneto";
            default:
                return std::to_string(distance - 1) + "-neto";
        }   

        // Go UP from A to see if B is an ancestor
        _childs.runBFS(memberA);
        distance = _childs.getBFSdistanceTo(memberB);

        switch(distance)
        {
            case(graph_commons::INFINITY_VAL):          // There really isn't a kinship
                return "sem parentesco";
            case(1):
                return "pai";
            case(2):
                return "avo";
            case(3):
                return "bisavo";
            default:
                return std::to_string(distance - 1) + "-avo";
        }
        return "sem parentesco";
    }


    void printDescendants()
    {
        _parents.runBFS(_patriarchName);
        Vector<std::string> traversedPath = _parents.getBSTraversedPath();
        
        for(size_t i = 1; i < traversedPath.size(); i++)
        {
            std::string member = traversedPath[i];
            size_t distance = _parents.getBFSdistanceTo(member);

            std::string title;
            switch(distance)
            {
                case(graph_commons::INFINITY_VAL):
                    title = "sem parentesco"; break;
                case(1):
                    title = "filho"; break;
                case(2):
                    title = "neto"; break;
                case(3):
                    title = "bisneto"; break;
                default:
                    title = std::to_string(distance - 1) + "-neto";
            }

            std::cout << member << " " << title << "\n";
        }    
    }
};





int main()
{
    Family joseFamily;



    // Gather the graph data ------------------------------------------------------
    size_t numNodes, numEdges;
    std::cin >> numNodes >> numEdges;
    
    for(size_t i = 0; i < numEdges; i++)
    {
        std::string parent, child;
        std::cin >> parent >> child;

        joseFamily.addParentChild(parent, child);
    }



    // Find the patriarch ------------------------------------------------------
    std::string patriarchName = joseFamily.getPatriarch();



    // Print all kinships ------------------------------------------------------
    joseFamily.printDescendants();



    // Process the queries ------------------------------------------------------
    size_t numQueries; std::cin >> numQueries;

    for(size_t i = 0; i < numQueries; i++)
    {
        std::string memberA, memberB;
        std::cin >> memberA >> memberB;

        std::cout << joseFamily.getKinship(memberA, memberB) << "\n";
    }
}