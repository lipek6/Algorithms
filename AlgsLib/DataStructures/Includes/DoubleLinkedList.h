/*
    DOUBLE LINKED LIST

    What is this?
	~~~~~~~~~~~~~
    This code implements a double linked list that can hold any kind of data/object.
*/
#pragma once
#include <stdexcept>
#include <optional>

template <typename T>
class DoubleLinkedList
{
private:
    struct Node
    {
        T data;
        Node* last;
        Node* next;

        Node(const T& data, Node* last = nullptr, Node* next = nullptr) : data(data), last(last), next(next) {}
        ~Node() {}
    };
  
    Node* begin;
    Node* end;
    size_t listSize;

public:
    // ==========================================
    // CONSTRUCTORS & DESTRUCTORS 
    // ==========================================

    // Default constructor
    DoubleLinkedList() : begin(nullptr), last(nullptr), end(nullptr), listSize(0) {}
    
    // Default destructor
    ~DoubleLinkedList()
    {
        clear();
    }

    // Deep copy constructor
    DoubleLinkedList(const DoubleLinkedList& source) : begin(nullptr), end(nullptr), listSize(0)
    {
        if(source.begin == nullptr) return;

        Node* auxPtr = source.begin;
        while(auxPtr != nullptr)
        {
            this->push_back(auxPtr->data);
            auxPtr = auxPtr->next;
        }
    }
    
    // Copy assignment operator
    DoubleLinkedList& operator=(const DoubleLinkedList& source)
    {
        if(this == &source) return *this;

        this->clear();

        Node* auxPtr = source.begin;
        while(auxPtr != nullptr)
        {
            this->push_back(auxPtr->data);
            auxPtr = auxPtr->next;
        }
        return *this;
    }

    // Move constructor
    DoubleLinkedList(DoubleLinkedList&& source) noexcept : begin(source.begin), end(source.end), listSize(source.listSize)
    {
        source.begin = nullptr;
        source.end = nullptr;
        source.listSize = 0;
    }

    // Move assignment operator
    DoubleLinkedList& operator=(DoubleLinkedList&& source) noexcept
    {
        if(this == &source) return *this;

        this->clear();

        begin = source.begin;
        end = source.end;
        listSize = source.listSize;

        source.begin = nullptr;
        source.end = nullptr;
        source.listSize = 0;

        return *this;
    }

    // ==========================================
    // CAPACITY  
    // ==========================================

    // Checks if the list is empty
    bool empty() const
    {
        return listSize == 0;
    }

    // Returns the number of nodes on list
    size_t size() const
    {
        return listSize;
    }

    // ==========================================
    // GETTERS
    // ==========================================

    // Returns the first element of the list
    T& front()
    {
        if(begin != nullptr)
            return begin->data;
        throw std::out_of_range("Empty list");
    }

    const T& front() const
    {
        if(begin != nullptr)
            return begin->data;
        throw std::out_of_range("Empty list");
    }

    // Returns the last element of the list
    T& back()
    {
        if(end != nullptr)
            return end->data;
        throw std::out_of_range("Empty list");
    }

    const T& back() const
    {
        if(end != nullptr)
            return end->data;
        throw std::out_of_range("Empty list");
    }

    // Index operator
    T& operator[](const size_t pos) 
    {
        if(pos >= listSize) throw std::out_of_range("Index out of range");
        
        Node* auxPtr;        
        if(pos <= listSize / 2)
        {
            auxPtr = begin;
            for(size_t i = 0; i < pos; i++)
            {
                auxPtr = auxPtr->next;
            }        
        }
        else
        {
            auxPtr = end;
            for(size_t i = listSize - 1; i > pos; i--)
            {
                auxPtr = auxPtr->last;
            }            
        }
        return auxPtr->data;
    }

    const T& operator[](const size_t pos) const
    {
        if(pos >= listSize) throw std::out_of_range("Index out of range");
        
        Node* auxPtr;        
        if(pos <= listSize / 2)
        {
            auxPtr = begin;
            for(size_t i = 0; i < pos; i++)
            {
                auxPtr = auxPtr->next;
            }        
        }
        else
        {
            auxPtr = end;
            for(size_t i = listSize - 1; i > pos; i--)
            {
                auxPtr = auxPtr->last;
            }            
        }
        return auxPtr->data;
    }

    // Linear search
    std::optional<size_t> find(const T &target) const
    {
        if(listSize == 0) return std::nullopt;

        size_t pos = 0;
        Node* auxPtr = begin;

        while(pos < listSize && auxPtr->data != target)
        {
            auxPtr = auxPtr->next;
            pos++;
        }

        if(pos < listSize) 
            return pos;
        return std::nullopt;
    }

    // ==========================================
    // MODIFIERS
    // ==========================================

    // Removes all the nodes of the list
    void clear()
    {
        if(begin == nullptr) return;

        Node* auxPtr = begin->next;
        while(auxPtr != nullptr)
        {
            delete begin;
            begin  = auxPtr;            
            auxPtr = auxPtr->next;
        }
        delete begin;
        begin = nullptr;
        end = nullptr;
        listSize = 0;
    }

    // Creates a node with the given data at the given position
    void insert(const T &newData, const size_t pos)
    {
        if(pos > listSize) throw std::out_of_range("Index out of range");

        if(listSize == 0)
        {
            begin = new Node(newData, nullptr, nullptr);
            end = begin;
        }
        else if(pos == 0) 
        {
            begin = new Node(newData, nullptr, begin);
            begin->next->last = begin;
        }
        else if(pos == listSize)
        {
            end->next = new Node(newData, end, nullptr);
            end = end->next;
        }
        else if(pos <= listSize / 2)
        {
            Node* auxPtr = begin;
            for(size_t i = 0; i < pos-1; i++)
            {
                auxPtr = auxPtr->next;
            }
            auxPtr->next = new Node(newData, auxPtr, auxPtr->next);
            auxPtr->next->next->last = auxPtr->next;
        }
        else
        {
            Node* auxPtr = end;
            for(size_t i = listSize - 1; i > pos; i--)
            {
                auxPtr = auxPtr->last;
            }
            auxPtr->last = new Node(newData, auxPtr->last, auxPtr);
            auxPtr->last->last->next = auxPtr->last; 
        }
        listSize++;
    }

    // Deletes the node at the given position
    void remove(const size_t pos)
    {
        if(pos >= listSize) throw std::out_of_range("Index out of range");

        Node* auxPtr1;
        if(pos == 0)
        {
            auxPtr1 = begin;
            begin = begin->next;
            delete auxPtr1;
            
            if(begin == nullptr)
                end = nullptr;
            else
                begin->last = nullptr;
        }
        else if(pos == listSize - 1)
        {
            auxPtr1 = end;
            end = end->last;
            delete auxPtr1;
            end->next = nullptr;
        }
        else if(pos <= listSize / 2)
        {
            auxPtr1 = begin;
            for(size_t i = 0; i < pos-1; i++)
            {
                auxPtr1 = auxPtr1->next;
            }
            Node* auxPtr2 = auxPtr1->next;
            auxPtr1->next = auxPtr2->next;
            auxPtr2->next->last = auxPtr1;
            delete auxPtr2;
        }  
        else
        {
            auxPtr1 = end;
            for(size_t i = listSize - 1; i > pos + 1; i--)
            {
                auxPtr1 = auxPtr1->last;
            }
            Node* auxPtr2 = auxPtr1->last;
            auxPtr2->last->next = auxPtr1;
            auxPtr1->last = auxPtr2->last; 
            delete auxPtr2;
        }
        listSize--;
    }

    // Creates a node with the given data at the end of the list
    void push_back(const T &newData) { insert(newData, listSize); }

    // Creates a node with the given data at the begin of the list
    void push_front(const T &newData) { insert(newData, 0); }
    
    // Deletes the last node of the list
    void pop_back(){ remove(listSize - 1); }

    // Deletes the first node of the list
    void pop_front() { remove(0); }

    // ==========================================
    // OVERLOADINGS
    // ==========================================

    // Adds the nodes of the rigth list to the end of the left list
    DoubleLinkedList& operator+=(const DoubleLinkedList& newNodes)
    {
        Node *auxPtr = newNodes.begin;
        while(auxPtr != nullptr)
        {
            this->push_back(auxPtr->data);
            auxPtr = auxPtr->next;
        }
        return *this;
    }

    // Adds the nodes of the rigth list to the end of the left list
    DoubleLinkedList operator+(const DoubleLinkedList& newNodes) const
    {
        DoubleLinkedList<T> newList;
        newList += *this;
        newList += newNodes;
        return newList;
    }
};