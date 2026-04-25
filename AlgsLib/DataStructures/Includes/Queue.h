/*
   QUEUE

    What is this?
	~~~~~~~~~~~~~
    This code implements a queue that can hold any kind of data/object.
    It uses a single linked list as its internal implementation.
*/
#pragma once
#include "SingleLinkedList.h"

template <typename T>
class Queue
{
private:
    SingleLinkedList<T> list;    

public:
    void push(const T& newData) { list.push_back(newData); }
    void pop() { list.pop_front(); }

    T& front() { return list.front(); }
    T& back() { return list.back(); }

    const T& front() const { return list.front(); }
    const T& back() const { return list.back(); }

    size_t size() const { return list.size(); }
    bool empty() const { return list.empty(); }
};