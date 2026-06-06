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
    const long long LL_INFINITY_VAL = std::numeric_limits<long long>::max() / 2;
    
    struct NoWeight {};

    template <typename T>
    struct NoHashing
    {
        size_t operator()(const T& input) const
        {
            return static_cast<size_t> (input);
        }
    };


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
// Heap.h
// ====================================================================================
template <typename T, typename Compare = std::less<T>>
class Heap
{
private:
    T* heap;
    size_t usedSize;
    size_t allocatedSize;
    Compare compare;

    // ==========================================
    // INTERNAL INDEX HELPERS
    // ==========================================
  
    size_t leftIdx   (const size_t idx) const { return (idx * 2) + 1; }     // Returns the index of the left child of the given node
    size_t rightIdx  (const size_t idx) const { return (idx * 2) + 2; }     // Returns the index of the right child of the given node
    size_t parentIdx (const size_t idx) const { return (idx - 1) / 2; }     // Returns the index of the parent of the given node (WARNING: Don't give it idx = 0, it will fuck up the size_t returned value)

    // ==========================================
    // INTERNAL HEAPIFY ALGORITHMS
    // ==========================================
   
    // Compares the node at the given index with its parent and swaps with parent until it finds its correct place (Used on Insertions).
    void heapifyUp(const size_t idx)
    {
        if(idx == 0 || compare(heap[idx], heap[parentIdx(idx)]))
            return;

        std::swap(heap[parentIdx(idx)], heap[idx]);
        heapifyUp(parentIdx(idx));
    }

    // Compares the node at the given index with its children and swaps with the child that is able to keep the heap property, after being swaped, until it finds its correct place (Used on Pop and Build).
    void heapifyDown(const size_t idx)
    {
        size_t target = idx;
        size_t left   = leftIdx(idx);
        size_t right  = rightIdx(idx);


        if(left < usedSize && compare(heap[target], heap[left]))
            target = left;
        if(right < usedSize && compare(heap[target], heap[right]))
            target = right; 

        if(target != idx)
        {
            std::swap(heap[target], heap[idx]);
            heapifyDown(target);
        }
    }
    
public:
    // ==========================================
    // CONSTRUCTORS & DESTRUCTORS 
    // ==========================================
    
    Heap(const size_t sizeToAllocate) : heap(new T[sizeToAllocate]), allocatedSize(sizeToAllocate), usedSize(0) {}
    ~Heap() { delete[] heap; }

    // ==========================================
    // CAPACITY
    // ==========================================
    
    bool empty() const { return usedSize == 0; }
    size_t size() const { return usedSize; }
    
    // ==========================================
    // GETTERS
    // ==========================================

    const T& top() const
    {
        if(usedSize > 0)
            return heap[0];
        else
            throw std::out_of_range("Empty heap, there is no top element");
    }

    void print() const
    {
        std::cout << "[";
        
        for(size_t i = 0; i < usedSize - 1; i++)
            std::cout << heap[i] << ", ";
        
        std::cout << heap[usedSize - 1] << "]\n";
    }

    // ==========================================   
    // MODIFIERS
    // ==========================================

    void insert(const T& newData)
    {
        if(usedSize < allocatedSize)
            heap[usedSize] = newData;
        else
            throw std::length_error("Allocated size is fully used. Cannot insert more elements");

        heapifyUp(usedSize);
        usedSize++;
    }

    T pop()
    {
        if(usedSize < 1)
            throw std::out_of_range("Empty heap, unnable to remove something");
        
        T topElement = heap[0];
        heap[0] = heap[usedSize - 1];
        usedSize--;
        
        heapifyDown(0);
        return topElement;
    }

    void topEdit(const size_t newValue)
    {
        if(usedSize > 0)
        {
            heap[0] = newValue;
            heapifyDown(0);
        }
        else
            throw std::out_of_range("Empty heap, there is no top element");
    }

    void buildFromArray(T* array, const size_t size)
    {
        if(size > allocatedSize) 
            throw std::length_error("Heap has an allocatedSize smaller than the size of the given array." );
        
        for(size_t i = 0; i < size; i++)
            heap[i] = array[i];
        
        usedSize = size;
        
        for(size_t i = usedSize / 2; i > 0; i--)
            heapifyDown(i - 1);
    }

    static void sort(T* array, const size_t size)
    {
        if(array == nullptr || size < 2)
            return;

        Heap<T, Compare> sorterHeap(size);
        sorterHeap.buildFromArray(array, size);

        for(size_t i = size; i > 0; i--)
            array[i - 1] = sorterHeap.pop();
    }
};



enum class RouteStatus { VALID, IMPOSSIBLE, INVALID_CYCLE };
struct BellmanResult
{
    RouteStatus status;
    long long cost;
};


template <typename W = graph_commons::NoWeight>
class AL
{
private:
    // ==========================================
    // CORE DATA STRUCTURES
    // ==========================================
    struct Edge
    {
        size_t node;
        W weight;

        bool operator< (const Edge& other) const { return this->weight <  other.weight; }
        bool operator<=(const Edge& other) const { return this->weight <= other.weight; }
        bool operator> (const Edge& other) const { return this->weight >  other.weight; }
        bool operator>=(const Edge& other) const { return this->weight >= other.weight; }
    };        
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
        if((sourceIdx < _topology.size() && destinyIdx < _topology.size()) && (_isActive[sourceIdx] && _isActive[destinyIdx]))
            _topology[sourceIdx].pushBack({destinyIdx, weight});
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
        Vector<size_t> singleSource(1, sourceIdx);
        return runMultiSourceBFS(singleSource, distancesVector, predecessorsVector, traversalVector);
    }

    size_t runMultiSourceBFS(Vector<size_t> sourcesIdxVector, Vector<size_t>& distancesVector, Vector<size_t>& predecessorsVector, Vector<size_t>& traversalVector) 
    {
        Queue<size_t> queue; 
        size_t numReachedNodes = 0;

        for(size_t i = 0; i < sourcesIdxVector.size(); i++)
        {
            size_t sourceIdx = sourcesIdxVector[i];

            if(sourceIdx < _topology.size() && _isActive[sourceIdx])
            {
                if(distancesVector[sourceIdx] == graph_commons::INFINITY_VAL)     // Uniqueness of the sources
                {
                    queue.push(sourcesIdxVector[i]);
                    distancesVector[sourceIdx] = 0;
                    predecessorsVector[sourceIdx] = sourceIdx;
                    traversalVector.pushBack(sourceIdx);
                    numReachedNodes++;
                }
            }   
        }

        while(!queue.empty())
        {
            size_t currentNodeIdx = queue.front();
            queue.pop();

            for(size_t i = 0; i < _topology[currentNodeIdx].size(); i++)
            {
                size_t neighborIdx = _topology[currentNodeIdx][i].node;

                if(distancesVector[neighborIdx] != graph_commons::INFINITY_VAL)
                    continue;

                queue.push(neighborIdx);
                distancesVector[neighborIdx] = distancesVector[currentNodeIdx] + 1;
                predecessorsVector[neighborIdx] = currentNodeIdx;
                traversalVector.pushBack(neighborIdx);
                
                numReachedNodes++;
            }
        }
        return numReachedNodes;
    }

    size_t runDFS(const size_t sourceIdx, Vector<bool>& visiteds)
    {
        if (sourceIdx >= _topology.size() || !_isActive[sourceIdx]) 
            return 0;

        size_t numReachedNodes = 1; 
        visiteds[sourceIdx] = true;

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

    void runDijkstra(const size_t sourceIdx, Vector<size_t>& distancesVector, Vector<size_t>& predecessorsVector, Vector<size_t>& traversalVector)
    {
        if (sourceIdx >= _topology.size() || !_isActive[sourceIdx]) 
            return;

        Heap<Edge ,std::less<Edge>> minHeap;
        minHeap.insert({sourceIdx, 0});

        distancesVector[sourceIdx]    = 0;
        predecessorsVector[sourceIdx] = sourceIdx;

        while(!minHeap.empty())
        {
            Edge current = minHeap.pop();
            
            if(current.weight > distancesVector[current.node])
                continue;
            
            traversalVector.pushBack(current.node);

            for(size_t i = 0; i < _topology[current.node].size(); i++)
            {
                Edge neighbor = _topology[current.node][i];
                
                size_t newWeight = current.weight + neighbor.weight;
                size_t oldWeight = distancesVector[neighbor.node];

                if(newWeight < oldWeight)
                {
                    distancesVector[neighbor.node]    = newWeight;
                    predecessorsVector[neighbor.node] = current.node;
                    
                    minHeap.insert({neighbor.node, newWeight});
                }
            }
        }
    }


    BellmanResult solveLogistics(const size_t sourceIdx, const size_t destinyIdx, Vector<long long>& distances)
    {
        if (sourceIdx >= _topology.size() || !_isActive[sourceIdx]) 
            return {RouteStatus::IMPOSSIBLE, 0};

        distances[sourceIdx] = 0;
        size_t V = _activeNodesCount;

        for(size_t i = 0; i < V - 1; i++)
        {
            bool relaxedAny = false;
            for(size_t u = 0; u < _topology.size(); u++)
            {
                if(!_isActive[u] || distances[u] == graph_commons::LL_INFINITY_VAL)
                    continue;

                for(size_t i = 0; i < _topology[u].size(); i++)
                {
                    size_t v = _topology[u][i].node;
                    long long weight = _topology[u][i].weight;

                    if(distances[u] + weight < distances[v])
                    {
                        distances[v] = distances[u] + weight;
                        relaxedAny = true;
                    }
                }
            }
            if(!relaxedAny) break; 
        }

        Queue<size_t> infectedNodes;
        Vector<bool> isInfected(_topology.size(), false);

        for(size_t u = 0; u < _topology.size(); u++)
        {
            if(!_isActive[u] || distances[u] == graph_commons::LL_INFINITY_VAL)
                continue;

            for(size_t i = 0; i < _topology[u].size(); i++)
            {
                size_t v = _topology[u][i].node;
                long long weight = _topology[u][i].weight;

                if(distances[u] + weight < distances[v])
                {
                    if(!isInfected[v])
                    {
                        isInfected[v] = true;
                        infectedNodes.push(v);
                    }
                }
            }
        }

        while(!infectedNodes.empty())
        {
            size_t current = infectedNodes.front();
            infectedNodes.pop();

            if(current == destinyIdx)
                return {RouteStatus::INVALID_CYCLE, 0};

            for(size_t i = 0; i < _topology[current].size(); i++)
            {
                size_t neighbor = _topology[current][i].node;
                if(!isInfected[neighbor])
                {
                    isInfected[neighbor] = true;
                    infectedNodes.push(neighbor);
                }
            }
        }
        if(distances[destinyIdx] == graph_commons::LL_INFINITY_VAL)
            return {RouteStatus::IMPOSSIBLE, 0};

        return {RouteStatus::VALID, distances[destinyIdx]};
    }
};



int main()
{
    size_t N, M, S, T; std::cin >> N >> M >> S >> T; S--; T--;

    AL<long long> map;
    for(size_t i = 0; i < N; i++)
        map.addNode();

    for(size_t i = 0; i < M; i++)
    {
        size_t u, v;
        long long w;
        std::cin >> u >> v >> w;
        
        map.addEdge(u - 1, v - 1, w); 
    }

    Vector<long long> distances(N, graph_commons::LL_INFINITY_VAL);
    BellmanResult result = map.solveLogistics(S, T, distances);

    if(result.status == RouteStatus::INVALID_CYCLE)
        std::cout << "ROTA INVALIDA\n";
    else if(result.status == RouteStatus::IMPOSSIBLE)
        std::cout << "IMPOSSIVEL\n";
    else
        std::cout << result.cost << "\n";

    return 0;
}