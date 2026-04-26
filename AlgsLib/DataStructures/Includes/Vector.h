#pragma once
#include <exception>

template <typename T>
class Vector
{
private:
    T* array;
    size_t usedSize;
    size_t allocatedSize;
    size_t RESIZE_FACTOR;

public:
    Vector(const size_t initialSize = 16, const size_t resizeFactor = 2)
        : allocatedSize(initialSize), usedSize(0), array(new T[initialSize]), RESIZE_FACTOR(resizeFactor > 1 ? resizeFactor : 2) {}

    ~Vector() { delete[] array; }


    // RULE OF 3: If a class uses a customized destructor, it NEEDS to have a copy constructor and a assignment operator customized for it.
    // This was fucking up the Vector<Vector<Edge>> because of double frees (the copies were just pointers being repeated and freed).

    // Copy constructor
    Vector(const Vector<T>& other) : allocatedSize(other.allocatedSize), usedSize(other.usedSize), array(new T[other.allocatedSize]), RESIZE_FACTOR(other.RESIZE_FACTOR)
    {
        for(size_t i = 0; i < other.usedSize; i++)
            this->array[i] = other.array[i];
    }

    // Assignment operator
    Vector& operator=(const Vector<T>& other)
    {
        if(this == &other) return *this;
        
        delete[] this->array;

        this->allocatedSize = other.allocatedSize;
        this->usedSize      = other.usedSize;
        this->RESIZE_FACTOR = other.RESIZE_FACTOR;
        this->array         = new T[allocatedSize];

        for(size_t i = 0; i < other.usedSize; i++)
            this->array[i] = other.array[i];
        
        return *this;
    }

    T& operator[](size_t index) { return array[index]; }
    const T& operator[](size_t index) const { return array[index]; }

    void resize(const size_t newAllocatedSize)
    {
        if(allocatedSize >= newAllocatedSize) return;
        
        T* oldArray = array;
        array = new T[newAllocatedSize];

        for(size_t i = 0; i < usedSize; i++)
            array[i] = oldArray[i];

        allocatedSize = newAllocatedSize;
        delete[] oldArray;
    }

    void pushBack(const T& newData)
    {
        if(usedSize == allocatedSize)
            resize(allocatedSize * RESIZE_FACTOR);
        
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
};

