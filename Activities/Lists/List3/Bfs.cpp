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
    const size_t INFINITY = std::numeric_limits<size_t>::max();
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
// AL.h
// ====================================================================================
template <typename W = graph_commons::NoWeight>
class AL
{
    template <typename T, typename _W> friend class Graph;

public:
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










// ====================================================================================
// Graph.h
// ====================================================================================
template <typename T = int, typename W = graph_commons::NoWeight>
class Graph
{
public:
    AL<W> graph;  
    bool isDirected;
    

    struct Degrees
    {
        size_t inDegree;
        size_t outDegree;

        Degrees(const size_t inDegree = 0, const size_t outDegree = 0) : inDegree(inDegree), outDegree(outDegree) {}
    };

    Vector<Degrees> NodeDegrees;

public:
    
    // PRIVATE MEMBERS. MOVED TO PUBLIC SPACE ONLY FOR THIS QUESTION!
    Vector<size_t> BFSdistances;
    HashTable_Closed<T, size_t> nodeToId;        // Need to add a custom hashing function later to satisfy the algorithms teacher
    Vector<T> idToNode;

    Graph(bool isDirected = false) : isDirected(isDirected) {}

    void addNode(const T& newNode = T())
    {
        size_t newId = graph.addNode();
        nodeToId.insert(newNode, newId);

        if(newId < idToNode.size())
        {
            idToNode[newId] = newNode;
            NodeDegrees[newId] = {0, 0};
        }
        else
        {
            idToNode.pushBack(newNode);
            NodeDegrees.pushBack({0, 0});
        }
    }

    void addEdge(const T& sourceNode, const T& destinyNode, const W weight = W())
    {
        size_t* sourceIdPtr  = nodeToId.find(sourceNode);
        size_t* destinyIdPtr = nodeToId.find(destinyNode);

        if(sourceIdPtr != nullptr && destinyIdPtr != nullptr)
        {
            graph.addEdge(*sourceIdPtr, *destinyIdPtr, weight);
            
            NodeDegrees[*sourceIdPtr].outDegree++;
            NodeDegrees[*destinyIdPtr].inDegree++;

            if(!isDirected)
            {
                graph.addEdge(*destinyIdPtr, *sourceIdPtr, weight);

                NodeDegrees[*destinyIdPtr].outDegree++;
                NodeDegrees[*sourceIdPtr].inDegree++;
            }
        }
    }

    void removeNode(const T& node)
    {
        size_t* idPtr = nodeToId.find(node);
        if(idPtr != nullptr)
        {            
            graph.removeNode(*idPtr);
            nodeToId.remove(node);
        }
    }

    void removeEdge(const T& sourceNode, const T& destinyNode)
    {
        size_t* sourceIdPtr  = nodeToId.find(sourceNode);
        size_t* destinyIdPtr = nodeToId.find(destinyNode);

        if(sourceIdPtr != nullptr && destinyIdPtr != nullptr)
        {
            graph.removeEdge(*sourceIdPtr, *destinyIdPtr);
            
            NodeDegrees[*sourceIdPtr].outDegree--;
            NodeDegrees[*destinyIdPtr].inDegree--;

            if(!isDirected)
            {
                graph.removeEdge(*destinyIdPtr, *sourceIdPtr);

                NodeDegrees[*destinyIdPtr].outDegree--;
                NodeDegrees[*sourceIdPtr].inDegree--;
            }
        }           
    }

    // Also return the amount of reached nodes
    size_t runBFS(const T& sourceNode)
    {
        size_t* sourceIdx = nodeToId.find(sourceNode);
        
        if(sourceIdx == nullptr) return 0;

        BFSdistances.clear();
        for(size_t i = 0; i < graph.topology.size(); i++)
            BFSdistances.pushBack(graph_commons::INFINITY);

        size_t numReachedNodes = graph.BFS(*sourceIdx, BFSdistances);

        return numReachedNodes;
    }

    size_t getBFSdistanceTo(const T& destinyNode)
    {   
        size_t* destinyIdx = nodeToId.find(destinyNode);

        if(destinyIdx == nullptr || BFSdistances.empty() || BFSdistances[*destinyIdx] == graph_commons::INFINITY)
            return graph_commons::INFINITY;

        return BFSdistances[*destinyIdx];
    }

    // void runDFS()

    size_t getInDegree(const T& targetNode)
    {
        size_t* targetNodeIdx = nodeToId.find(targetNode);
        
        if(targetNodeIdx == nullptr)
            return graph_commons::INFINITY;         // Might choose a better return value here

        return NodeDegrees[*targetNodeIdx].inDegree;
    }

    size_t getOutDegree(const T& targetNode)
    {
        size_t* targetNodeIdx = nodeToId.find(targetNode);
        
        if(targetNodeIdx == nullptr)
            return graph_commons::INFINITY;

        return NodeDegrees[*targetNodeIdx].outDegree;
    }

    size_t numNodes() const { return graph.activeNodesCount; }

};









int main()
{
    enum Relation {CHILD, PARENT};
    Graph<std::string> graphParent(true);
    Graph<std::string> graphChildHasParent(true);
    
    
    
    // Gather the graph data ------------------------------------------------------
    size_t numNodes, numEdges;
    std::cin >> numNodes >> numEdges;
    
    Vector<bool> isPatriarch(numNodes, false);

    for(size_t i = 0; i < numEdges; i++)
    {
        std::string parent, child;
        std::cin >> parent >> child;

        // Parent ---> Child
        graphParent.addNode(parent);
        graphParent.addNode(child);
        graphParent.addEdge(parent, child);

        graphChildHasParent.addNode(parent);
        graphChildHasParent.addNode(child);
        graphChildHasParent.addEdge(child, parent);
    }

    // Find patriarch ------------------------------------------------------------- 
    
    std::string namePatriarch = "";
    size_t idPatriarch = 0;
    size_t maxNumDescendants = 0;
    for(size_t i = 0; i < numNodes; i++)
    {
        if (graphParent.getInDegree(graphParent.idToNode[i]) == 0)
        {
            size_t numDescendants = graphParent.runBFS(graphParent.idToNode[i]);
            
            if(numDescendants > maxNumDescendants)
            {
                namePatriarch = graphParent.idToNode[i];
                maxNumDescendants = numDescendants;
                idPatriarch = i;
            }
        }
    }
    graphParent.runBFS(namePatriarch);
    graphChildHasParent.runBFS(namePatriarch);

    for(size_t i = 0; i < graphParent.numNodes(); i++)
    {
        switch(graphParent.BFSdistances[i])
        {
            case(graph_commons::INFINITY):
                break;
            case(0):
                break;
            case(1):
                std::cout << graphParent.idToNode[i] << " filho\n";
                break;
            case(2):
                std::cout << graphParent.idToNode[i] << " neto\n";
                break;
            case(3):
                std::cout << graphParent.idToNode[i] << " bisneto\n";
                break;
            default:
                std::cout << graphParent.idToNode[i] << " " <<graphParent.BFSdistances[i] << "-neto\n";
        }
    }


    // Runs the queries on the graph ----------------------------------------------
    /*size_t numQueries; std::cin >> numQueries;

    for(size_t i = 0; i < numQueries; i++)
    {
        std::string memberA, memberB;
        std::cin >> memberA >> memberB;
    
        // Output kinship from B in relation to A
        = graphParent.getBFSdistanceTo(memberA);
        = graphParent.getBFSdistanceTo(memberB);

    }
    */


}

// I NEED TO IMPLEMENT THE HASHING FUNCTION MANUALLY. USE A PERSONAL HASHER!!!!!