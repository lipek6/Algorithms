#include <iostream>
#include <functional>
#include <optional>
#include <exception>
#include <cstdlib>
#include <ctime>


// ====================================================================================
// CHEMICAL STRUCT (Its sole purpose is make easier to use a sort algorithm)
// ====================================================================================

struct Chemical
{
    std::string name;
    long long amount;

    Chemical() : name(""), amount(0) {}
    Chemical(const std::string& name, const long long amount) : name(name), amount(amount) {}

    bool operator<(const Chemical& other) const
    {
        if(this->amount < other.amount)
            return true;
        else if(this->amount == other.amount && this->name < other.name)
            return true;
        
        return false;
    }

    bool operator>(const Chemical& other) const
    {
        if(this->amount > other.amount)
            return true;
        else if(this->amount == other.amount && this->name > other.name)
            return true;
        
        return false;
    }
};




// ====================================================================================
// IMPROVISED VECTOR (I should really implement a proper vector later)
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
    Vector(const size_t initialSize = 16, const size_t resizeFactor = 2)
        : allocatedArraySize(initialSize), usedSize(0), array(new T[initialSize]), RESIZE_FACTOR(resizeFactor > 1 ? resizeFactor : 2) {}

    ~Vector() { delete[] array; }

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
        
        array[usedSize] = newData;
        usedSize++;
    }

    T& popBack()            // Not fully functional, but it will probably be enough here
    {
        array[usedSize--];
        return array[usedSize + 1];
    }

    bool empty() const { return usedSize == 0; }
    size_t size() const { return usedSize; }
};





// ====================================================================================
// QUICK SORT (WITH RANDOM PIVOT), (Adapted for my new vector, let's hope it doesn't break)
// ====================================================================================

template <typename T>
size_t PartitionR(Vector<T>& array, size_t left, size_t right)
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


template <typename T>
void RecursivelyQuickSortR(Vector<T>& array, size_t left, size_t right)
{
    if(left >= right)
        return;

    size_t pivot = PartitionR(array, left, right);

    if(pivot > 0)
        RecursivelyQuickSortR(array, left, pivot - 1);

    RecursivelyQuickSortR(array, pivot + 1, right);
}


template <typename T>
void QuickSortR(Vector<T>& array)
{
    if(array.size() <= 1)
        return;
    RecursivelyQuickSortR(array, 0, array.size() - 1);
}





// ====================================================================================
// HASH FUNCTION
// ====================================================================================

struct stringHasher
{
    size_t operator()(const std::string& str) const
    {
        size_t hashResult = 0;
        for(size_t i = 0; i < str.size(); i++)
            hashResult += (str[i] * (i + 1));

        return hashResult;
    }
};





// ====================================================================================
// HASH TABLE (Made before I made an improvised vector, so it uses a classic array with dynamic resizing)
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
            if(table[index].state == OCCUPIED && table[index].key == newKey)
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
        if(float(totalNodes) / allocatedSize > MAX_LOAD_FACTOR)
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

    void getCriticals(const long long criticalFactor, Vector<Chemical>& outputVector) const
    {
        for(size_t i = 0; i < allocatedSize; i++)
            if(table[i].state == OCCUPIED && table[i].value <= criticalFactor)
                outputVector.pushBack({table[i].key, table[i].value});
    }
};





// ====================================================================================
// MAIN
// ====================================================================================

int main(void)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    size_t numOperations; std::cin >> numOperations;
    HashTable_Closed<std::string, long long, stringHasher> stock;

    for(size_t i = 0; i < numOperations; i++)
    {
        std::string operation; std::cin >> operation;

        if(operation == "ENTRADA")
        {
            std::string name; long long newAmount; std::cin >> name >> newAmount;
            long long* oldAmount = stock.find(name);
            
            if(oldAmount != nullptr)
                stock.insert(name, *oldAmount + newAmount);
            else
                stock.insert(name, newAmount);
        }
        else if(operation == "CONSUMO")
        {
            std::string name; long long usedAmount; std::cin >> name >> usedAmount;
            long long* oldAmount = stock.find(name);

            if(oldAmount == nullptr)
                continue;
            
            if(usedAmount >= *oldAmount)                            
                stock.remove(name);
            else
                stock.insert(name, *oldAmount - usedAmount);
        }
        else if(operation == "CONSULTA")
        {
            std::string name; std::cin >> name;
            long long* amount = stock.find(name);
            
            if(amount != nullptr)
                std::cout << *amount << "\n";
            else
                std::cout << "AUSENTE\n";
        }
        else //if(operation == "CRITICOS")
        {
            long long criticalAmount; std::cin >> criticalAmount;
            Vector<Chemical> criticalChemicals;    
            
            stock.getCriticals(criticalAmount, criticalChemicals);          

            if(!criticalChemicals.empty())
            {
                QuickSortR(criticalChemicals);
                for(size_t i = 0; i < criticalChemicals.size(); i++)
                    std::cout << criticalChemicals[i].name << " " << criticalChemicals[i].amount << "\n";
            }
            else
                std::cout << "NENHUM\n";
        }
    }
}