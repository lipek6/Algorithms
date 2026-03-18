#include "SingleLinkedList.h"

template <typename T>
class Queue
{
private:
    SingleLinkedList<T> list;    

public:
    void push(const T& newData)
    {
        list.push_back(newData);
    }

    void pop()
    {
        list.pop_front();
    }

    T& front() const
    {
        return list.front();
    }

    T& back() const
    {
        return list.back();
    }

    size_t size() const
    {
        return list.size();
    }
};