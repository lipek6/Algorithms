/*
    ===========================================================================
    HEAP DATA STRUCTURE
    ===========================================================================

    What is this?
	~~~~~~~~~~~~~
    This class implements a Binary Heap that satisfies the Heap property. Although 
    conceptually a complete Binary Tree, it is implemented using a 1D zero-indexed array.
    
    This version of the Heap is the simplest availble. It doesn't use a std::vector, relying
    entirely on a classic array and its size allocated on the constructor by the user.


    Usage & Initialization:
    ~~~~~~~~~~~~~~~~~~~~~~~
    By default, this class behaves as a MAX-HEAP using std::less.
    
    To instantiate a Max-Heap (Default):
        option 1: Heap<int> maxHeap(100);
        option 2: Heap<int, std::less<int>> maxHeap(100);

    To instantiate a Min-Heap:
        option 1: Heap<int, std::greater<int>> minHeap(100);

    The usage of std::less places the smaller elements at the bottom
    and bigger elements at the top, where the top element is the biggest.

    The usage of std::greater places the bigger elements at the bottom
    and smaller elements at the top, where the top element is the smallest.


    Time Complexities:
    ~~~~~~~~~~~~~~~~~~
    - Insert:   O(log N)
    - Pop:      O(log N)
    - Top:      O(1)
    - Build:    O(N)            // Uses the Floyd's Algorithm
    - Sort:     O(N log N)
    ===========================================================================
*/
#pragma once
#include <functional>
#include <stdexcept>

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