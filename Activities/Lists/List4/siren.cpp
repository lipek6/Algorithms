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




// ====================================================================================
// Quick and dirty damn Dijkstra
// ====================================================================================
template <typename W = size_t>
class WeightedNodesGraph
{
private:
    struct Edge
    {
        size_t node;
        W weight;

        bool operator< (const Edge& other) const { return this->weight <  other.weight; }
        bool operator<=(const Edge& other) const { return this->weight <= other.weight; }
        bool operator> (const Edge& other) const { return this->weight >  other.weight; }
        bool operator>=(const Edge& other) const { return this->weight >= other.weight; }
    };        


    Vector<W> nodeWeights;
    Vector<Vector<Edge>> _AL;

public:
    void addNode(W nodeWeight)
    {
        _AL.pushBack(Vector<Edge>());
        nodeWeights.pushBack(nodeWeight);
    }

    void addEdge(size_t sourceNode, size_t destinyNode, W weight)
    {
        _AL[sourceNode].pushBack({destinyNode, weight});
    }

    size_t findCost(size_t sourceNode, size_t destinyNode, Vector<size_t>& distances, Vector<size_t>& predecessors)
    {
        Heap<Edge> minHeap(500000);
        distances[sourceNode]    = nodeWeights[sourceNode];
        predecessors[sourceNode] = sourceNode;
        minHeap.insert({sourceNode, nodeWeights[sourceNode]});
        
        while(!minHeap.empty())
        {
            Edge current = minHeap.pop();

            if(current.weight > distances[current.node])
                continue;
            
            for(size_t i = 0; i < _AL[current.node].size(); i++)
            {
                Edge neighbor = _AL[current.node][i];

                W newWeight = current.weight + neighbor.weight;
                
                if(neighbor.node != destinyNode)
                    newWeight += nodeWeights[neighbor.node];

                if(newWeight < distances[neighbor.node])
                {
                    distances[neighbor.node]    = newWeight;
                    predecessors[neighbor.node] = current.node;
                    minHeap.insert({neighbor.node, newWeight});
                }
            }
        }
        return distances[destinyNode];
    }



    size_t size() const
    {
        return _AL.size();
    }

};





int main()
{
    WeightedNodesGraph<size_t> map;
    size_t numCitys, numStreets, source, destiny;
    std::cin >> numCitys >> numStreets >> source >> destiny;
    
    source--;
    destiny--;

    for(size_t i = 0; i < numCitys; i++)
    {
        size_t sign; std::cin >> sign;
        map.addNode(sign);
    }

    for(size_t i = 0; i < numStreets; i++)
    {
        size_t street1, street2, traverseTime;
        std::cin >> street1 >> street2 >> traverseTime;
        
        map.addEdge(street1 - 1, street2 - 1, traverseTime);
        map.addEdge(street2 - 1, street1 - 1, traverseTime);
    }
    
    Vector<size_t> distances(map.size(), graph_commons::INFINITY_VAL);
    Vector<size_t> predecessors(map.size(), graph_commons::INFINITY_VAL);

    size_t cost = map.findCost(source, destiny, distances, predecessors);

    if(cost == graph_commons::INFINITY_VAL)
        std::cout << "IMPOSSIVEL\n";
    else
        std::cout << cost << "\n";
}