#pragma once
#include "Vector.h"

template <typename T>
class Stack
{
private:
    Vector<T> vector;

public:
    void push(const T& newData) { vector.pushBack(newData); }
    T pop() { return vector.popBack(); }
    
    T& top() { return vector.back(); }
    T& top() const { return vector.back(); }

    size_t size() const { return vector.size(); }
    bool empty() const { return vector.empty(); }
};