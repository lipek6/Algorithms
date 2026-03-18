/*
    SINGLE LINKED LIST

    What is this?
	~~~~~~~~~~~~~
    This code implements a single linked list.
*/

#include <iostream>
#include <stdexcept>
#include <optional>

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
    Node* end;
    size_t listSize;

    public:
    LinkedList() : begin(nullptr), end(nullptr), listSize(0) {}
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

    bool empty() const
    {
        if(begin == nullptr)
            return true;
        else
            return false;
    }

    size_t size() const
    {
        return listSize;
    }

    void insert(const T &newData, const size_t pos)
    {
        if(pos == 0)
        {
            begin = new Node(newData, begin);
            if
        }
        else
        {
            Node* auxPtr = begin;
            for(size_t i = 0; i < pos-1; i++)
            {
                auxPtr = auxPtr->next;
            }
            auxPtr->next = new Node(newData, auxPtr->next);
            if(auxPtr == end) end = auxPtr->next;
        }
    }

    void remove(const size_t pos)
    {
        if(pos >= listSize) throw std::out_of_range("Index out of range");

        Node* auxPtr1 = begin;
        if(pos == 0)
        {
            begin = begin->next;
            delete auxPtr1;

            if(begin == nullptr) end = nullptr;
        }
        else
        {
            for(size_t i = 0; i < pos-1; i++)
            {
                auxPtr1 = auxPtr1->next;
            }
            Node* auxPtr2 = auxPtr1->next;
            auxPtr1->next = auxPtr2->next;
            
            if(auxPtr2 == end) end = auxPtr1;

            delete auxPtr2;
        }  

        listSize--;
    }

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
        else
            return std::nullopt;
    }

    T& operator[](const size_t pos)
    {
        if(pos >= listSize) throw std::out_of_range("Index out of range");
        
        if(pos == listSize - 1) return end->data;

        Node* auxPtr = begin;        
        for(size_t i = 0; i < pos; i++)
        {
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