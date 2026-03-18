/*
    SIMPLEST SINGLE LINKED LIST

    This code implements a linked list that has only a pointer to the begining of the list and nodes.


*/

#include <iostream>
#include <stdexcept>


template <typename T>
class LinkedList
{
private:
    struct Node
    {
        Node* next;
        T data;

        Node() : next(nullptr) {}
        Node(const T& data, Node* next = nullptr) : data(data), next(next) {}
        ~Node() {}
    };
    Node* begin;

public:
    LinkedList() : begin(nullptr) {}
    ~LinkedList()
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
        return;
    }

    void insert(const T &newData, const size_t pos)
    {
        if(pos == 0 || begin == nullptr)
        {
            begin = new Node(newData, begin);
            return;
        }

        Node* auxPtr = begin;
        for(size_t i = 0; i < pos-1; i++)
        {
            if(auxPtr->next == nullptr) break;
            auxPtr = auxPtr->next;
        }

        auxPtr->next = new Node(newData, auxPtr->next);
        return;
    }

    void remove(const size_t pos)
    {
        if(begin == nullptr) return;
        
        Node* auxPtr1 = begin;
        if(pos == 0)
        {
            begin = begin->next;
            delete auxPtr1;
            return;
        }
        
        for(size_t i = 0; i < pos-1; i++)
        {
            if(auxPtr1 == nullptr) return;

            auxPtr1 = auxPtr1->next;
        }
        
        if(auxPtr1 != nullptr && auxPtr1->next != nullptr)
        {
            Node* auxPtr2 = auxPtr1->next;
            auxPtr1->next = auxPtr2->next;
            delete auxPtr2;
        }
        return;
    }

    T& operator[](const size_t pos)
    {
        if(begin == nullptr) error;

        Node* auxPtr = begin;
        
        for(size_t i = 0; i < pos; i++)
        {
            if(auxPtr == nullptr) return begin->data;

            auxPtr = auxPtr->next;
        }

        return auxPtr->data;
    }
};








int main(void)
{
    LinkedList<std::string> list;

    list.insert("Felipe", 0);
    list.insert("Sara", 0);
    list.insert("Fernanda", 0);
    list.insert("Ferreira", 10);
    list.insert("Augusto", 1);
    std::cout << list[0] << " " << list[1] << " " << list[2] << std::endl;    
    list.remove(0);
    list.remove(0);
    list.remove(10);
    list.remove(1);

    return 0;    
}