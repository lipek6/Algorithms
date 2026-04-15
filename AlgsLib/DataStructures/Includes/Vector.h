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

    T& popBack()            // Not fully functional, but it will probably be enough here
    {
        array[usedSize--];
        return array[usedSize + 1];
    }

    bool empty() const { return usedSize == 0; }
    size_t size() const { return usedSize; }
};

