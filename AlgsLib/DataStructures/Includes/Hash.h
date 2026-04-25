#pragma once
#include "Vector.h"
#include <functional>
#include <optional>
#include <exception>


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