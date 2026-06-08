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
int main()
{
    // AMOUNT OF STATIONS ------------------------------
    long long stations_amount_per_line;
    long long stations_risk_pairs_amount;
    std::cin >> stations_amount_per_line >> stations_risk_pairs_amount;
    
    // ENTRY COST OF LINES -----------------------------    
    long long entry_cost_1;
    long long entry_cost_2;
    std::cin >> entry_cost_1 >> entry_cost_2;

    // EXIT COST OF LINES ------------------------------    
    long long exit_cost_1;
    long long exit_cost_2;
    std::cin >> exit_cost_1 >> exit_cost_2;

    // STATIONS COSTS ----------------------------------
    Vector<long long> station_cost_1(stations_amount_per_line);
    Vector<long long> station_cost_2(stations_amount_per_line);

    for(size_t i = 0; i < stations_amount_per_line; i++)
    {
        long long station_cost;
        std::cin >> station_cost;

        station_cost_1.pushBack(station_cost);
    }
    for(size_t i = 0; i < stations_amount_per_line; i++)
    {
        long long station_cost;
        std::cin >> station_cost;

        station_cost_2.pushBack(station_cost);
    }

    // TRANSITION COSTS --------------------------------
    Vector<long long> transition_cost_1(0);
    Vector<long long> transition_cost_2(0);
    
    if(stations_amount_per_line != 1)
    {
        transition_cost_1.resize(stations_amount_per_line - 1);
        transition_cost_2.resize(stations_amount_per_line - 1);

        for(size_t i = 0; i < stations_amount_per_line - 1; i++)
        {
            long long transition_cost;
            std::cin >> transition_cost;

            transition_cost_1.pushBack(transition_cost);
        }
        for(size_t i = 0; i < stations_amount_per_line - 1; i++)
        {
            long long transition_cost;
            std::cin >> transition_cost;

            transition_cost_2.pushBack(transition_cost);
        }
    }

    // AUDITION COSTS ----------------------------------
    Vector<long long> audition_cost_1(stations_amount_per_line);
    Vector<long long> audition_cost_2(stations_amount_per_line);
    
    for(size_t i = 0; i < stations_amount_per_line; i++)
    {
        long long audition_cost;
        std::cin >> audition_cost;

        audition_cost_1.pushBack(audition_cost);
    }
    for(size_t i = 0; i < stations_amount_per_line; i++)
    {
        long long audition_cost;
        std::cin >> audition_cost;

        audition_cost_2.pushBack(audition_cost);
    }

    // RISK PAIRS --------------------------------------
    Vector<long long> risk_u(stations_risk_pairs_amount);
    Vector<long long> risk_v(stations_risk_pairs_amount);

    for(size_t i = 0; i < stations_risk_pairs_amount; i++)
    {
        long long risk_1;
        long long risk_2;
        std::cin >> risk_1 >> risk_2;

        // Apenas converta para Base-0. Não tente adivinhar a linha ainda.
        risk_u.pushBack(risk_1 - 1);
        risk_v.pushBack(risk_2 - 1);
    }
    
    // =================================================
    // ================= END OF INPUT ==================
    // =================================================
    // DATA GATHERED:
    // stations_amount_per_line     ||      stations_risk_pairs_amount
    // entry_cost_1                 ||      entry_cost_2
    // exit_cost_1                  ||      exit_cost_2
    // station_cost_1               ||      station_cost_2
    // transition_cost_1            ||      transition_cost_2
    // audition_cost_1              ||      audition_cost_2
    // risk_pair_1                  ||      risk_pair_2

    
    // AUDITION HANDLING -------------------------------
    Vector<bool> is_audited(2 * stations_amount_per_line, false);
    long long audited_stations_count = 0;

    for(size_t i = 0; i < stations_risk_pairs_amount; i++)
    {
        long long u = risk_u[i];
        long long v = risk_v[i];

        if(is_audited[u] || is_audited[v])
            continue;

        is_audited[u] = true;
        is_audited[v] = true;
        audited_stations_count += 2;

        if(u < stations_amount_per_line) 
            station_cost_1[u] += audition_cost_1[u];
        else 
            station_cost_2[u - stations_amount_per_line] += audition_cost_2[u - stations_amount_per_line];

        if(v < stations_amount_per_line) 
            station_cost_1[v] += audition_cost_1[v];
        else 
            station_cost_2[v - stations_amount_per_line] += audition_cost_2[v - stations_amount_per_line];
    }

    // DP ----------------------------------------------
    Vector<long long> dp_1(stations_amount_per_line, 0);
    Vector<long long> dp_2(stations_amount_per_line, 0);

    dp_1[0] = entry_cost_1 + station_cost_1[0];     // BASE CASE
    dp_2[0] = entry_cost_2 + station_cost_2[0];     // BASE CASE


    if(stations_amount_per_line == 1)               // SPECIAL CASE
    {
        long long cost_1 = dp_1[0] + exit_cost_1;
        long long cost_2 = dp_2[0] + exit_cost_2;

        std::cout << audited_stations_count << " " << std::min(cost_1, cost_2);
        return 0;
    }

    // RECURSIVE STEP
    for(size_t i = 1; i < stations_amount_per_line; i++)
    {
        dp_1[i] = std::min(dp_1[i-1], dp_2[i-1] + transition_cost_2[i-1]) + station_cost_1[i];
        dp_2[i] = std::min(dp_2[i-1], dp_1[i-1] + transition_cost_1[i-1]) + station_cost_2[i];
    }

    // EXIT
    dp_1[stations_amount_per_line - 1] += exit_cost_1;
    dp_2[stations_amount_per_line - 1] += exit_cost_2;


    long long min_cost = std::min(dp_1.back(), dp_2.back());
    std::cout << audited_stations_count << " " << min_cost << std::endl;
}