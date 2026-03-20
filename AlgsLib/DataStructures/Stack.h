/*
   STACK

    What is this?
	~~~~~~~~~~~~~
    This code implements a stack that can hold any kind of data/object.
    It uses a single linked list as its internal implementation.
*/

#include "SingleLinkedList.h"

template <typename T>
class Stack
{
private:
    SingleLinkedList<T> list;

public:
    void pop() { list.pop_front(); }
    void push(const T& newData) { list.push_front(newData); }

    T& top() { return list.front(); }
    const T& top() const { return list.front(); }

    size_t size() const { return list.size(); }
    bool empty() const { return list.empty(); }
};