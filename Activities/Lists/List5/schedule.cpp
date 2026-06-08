#include <utility>
#include <iostream>
#include <exception>
#include <cstdlib>
#include <ctime>


template <typename T>
class Vector
{
private:
    T* array;
    size_t usedSize;
    size_t allocatedArraySize;
    size_t RESIZE_FACTOR;

    size_t PartitionR(T* array, size_t left, size_t right)
    {
        size_t randomIdx = left + (std::rand() % (right - left + 1));
        std::swap(array[left], array[randomIdx]);

        T pivot = array[left];
        size_t i = left;
        size_t j = right + 1;

        while(true)
        {
            while(array[++i] < pivot)           // Searching for element GREATER than the pivot
                if(i == right) break;               // Avoiding going out of bounds to the right
            while(array[--j] > pivot)           // Searching for element SMALLER than the pivot
                if(j == left) break;                // Avoiding going out of bounds to the left

            if(i >= j)                          // When i and j crosses wach other, it's time for the final swap
                break;

            std::swap(array[i], array[j]);      // Swap the two out of place elements
        }
        std::swap(array[left], array[j]);
        return j;
    }

    void RecursivelyQuickSortR(T* array, size_t left, size_t right)
    {
        if(left >= right)
            return;

        size_t pivot = PartitionR(array, left, right);

        if(pivot > 0)
            RecursivelyQuickSortR(array, left, pivot - 1);

        RecursivelyQuickSortR(array, pivot + 1, right);
    }

    void QuickSortR(T* array, size_t size)
    {
        if(size <= 1)
            return;
        RecursivelyQuickSortR(array, 0, size - 1);
    }

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

    // RULE OF 5: Add the move constructor and move assignment.
    Vector(Vector<T>&& other) noexcept
        : array(other.array)
        , usedSize(other.usedSize)
        , allocatedArraySize(other.allocatedArraySize)
        , RESIZE_FACTOR(other.RESIZE_FACTOR)
    {
        other.array              = nullptr;           // When the other destructor is called, it won't screw with our stole array, because it doesn't know it anymore.
        other.usedSize           = 0;                 // Golden rule for moved-from objects: They must be left in a "valid but unspecified state."
        other.allocatedArraySize = 0;

    }

    Vector& operator=(Vector<T>&& other) noexcept
    {
        if(this == &other) return *this;

        delete[] this->array;
        this->array              = other.array;
        this->usedSize           = other.usedSize;
        this->RESIZE_FACTOR      = other.RESIZE_FACTOR;
        this->allocatedArraySize = other.allocatedArraySize;

        other.array               = nullptr;            // When the other destructor is called, it won't screw with our stole array, because it doesn't know it anymore.
        other.usedSize            = 0;                  // Golden rule for moved-from objects: They must be left in a "valid but unspecified state."
        other.allocatedArraySize  = 0;

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

    void sort()
    {
        QuickSortR(array, usedSize);
    }

};



// CODE ---------------------------------------------------------------------------------------------------------------------------------------------
struct Event {
    long long start;
    long long end;
    long long value;
    bool is_valid = true;

    // Ordenação principal: Menor tempo de término primeiro
    bool operator<(const Event& other) const { return this->end < other.end; }
    bool operator>(const Event& other) const { return this->end > other.end; }
};

int main() {

    size_t num_events, num_incompatible_pairs;
    std::cin >> num_events >> num_incompatible_pairs;

    Vector<Event> events(num_events);

    for(size_t i = 0; i < num_events; i++) 
    {
        long long start, end, value; std::cin >> start >> end >> value;
        events.pushBack({start, end, value, true});
    }

    size_t num_cancelled_events = 0;
    for(size_t pair = 0; pair < num_incompatible_pairs; pair++)
    {
        size_t u, v; std::cin >> u >> v;
        u--; 
        v--;

        if (events[u].is_valid && events[v].is_valid)
        {
            events[u].is_valid = false;
            events[v].is_valid = false;
            num_cancelled_events += 2;
        }
    }

    Vector<Event> valid_events(num_events); 

    for(size_t i = 0; i < events.size(); i++)
        if(events[i].is_valid)
            valid_events.pushBack(events[i]);
    

    valid_events.sort();

    if (!valid_events.size())
    {
        std::cout << num_cancelled_events << " 0\n";
        return 0;
    }

    Vector<long long> dp(valid_events.size(), 0);
    dp[0] = valid_events[0].value;

    for (size_t i = 1; i < valid_events.size(); i++)
    {
        long long skip = dp[i - 1];
        long long pick = valid_events[i].value;
        long long left = 0;
        long long right = i - 1;
        long long last_compatible = -1;

        while (left <= right)
        {
            long long mid = left + (right - left) / 2;
            if (valid_events[mid].end <= valid_events[i].start)
            {
                last_compatible = mid; 
                left = mid + 1;        
            }
            else
            {
                right = mid - 1;       
            }
        }

        if (last_compatible != -1)
            pick += dp[last_compatible];
        

        dp[i] = std::max(skip, pick);
    }
    std::cout << num_cancelled_events << " " << dp[valid_events.size() - 1] << "\n";
}