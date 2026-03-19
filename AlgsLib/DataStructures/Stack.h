#include "SingleLinkedList.h"

template <typename T>
class Stack
{
private:
    SingleLinkedList<T> list;

public:
    void pop()
    {
        list.pop_front();
    }

    void push(const T& newData)
    {
        list.push_front(newData);
    }

    T& top() const
    {
        return list.front();
    }

    size_t size() const
    {
        return list.size();
    }
};