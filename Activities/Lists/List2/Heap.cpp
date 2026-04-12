#include <iostream>
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

    T& top()
    {
        if(usedSize > 0)
            return heap[0];
        else
            throw std::out_of_range("Empty heap, there is no top element");
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

struct Task
{
    std::string id;
    long long priority;             // Needs to be signed because of the priorityBoostAccumulator
    size_t executionTime;

    Task() {}
    Task(const std::string& id, const long long priority, const size_t executionTime)
        : id(id), priority(priority), executionTime(executionTime) {}

    // Used inside my Heap. If I am less prioritary (true) than my parent, I must stay where I am. If I am not less prioritary (false) than my parent, We must swap.
    bool operator<(const Task& other) const         
    {
        if(this->priority < other.priority)
            return true;
        if(this->priority == other.priority && this->executionTime > other.executionTime)
            return true;
        if(this->priority == other.priority && this->executionTime == other.executionTime && this->id > other.id)
            return true;
        
        return false;
    }
};


int main(void)
{
    size_t numEvents; std::cin >> numEvents;
    
    Heap<Task> scheduler(numEvents);
    size_t priorityBoostAccumulator = 0;            // When we add a new task, it will be weakened by this factor (making the tasks that received the boost comparatively stronger) 
    
    for(size_t i = 0; i < numEvents; i++)
    {
        std::string command; std::cin >> command;
        
        if(command == "ADD")
        {
            std::string id; long long priority; size_t executionTime; std::cin >> id >> priority >> executionTime;
            Task newTask(id, priority - priorityBoostAccumulator, executionTime);
            scheduler.insert(newTask);
        }
        else if(command == "RUN")
        {
            if(!scheduler.empty())
            {
                Task topTask = scheduler.pop();
                std::cout << topTask.id << "\n"; 
            }
            else
                std::cout << "IDLE\n";
        }
        else //if(command == "BOOST")   This only affects when a new task appears after the boost, because it won't have received it  
        {
            size_t priorityBoost; std::cin >> priorityBoost;
            priorityBoostAccumulator += priorityBoost;
        }
    }
}