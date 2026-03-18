/*
    SINGLE LINKED LIST

    What is this?
	~~~~~~~~~~~~~
    This code implements a single linked list.
*/

#include <stdexcept>
#include <optional>

template <typename T>
class SingleLinkedList
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
    SingleLinkedList() : begin(nullptr), end(nullptr), listSize(0) {}
    SingleLinkedList(const SingleLinkedList& source) : begin(nullptr), end(nullptr), listSize(0)
    {
        if(source.begin() == nullptr) return;

        Node* auxPtr = source.begin;
        while(auxPtr != nullptr)
        {
            this->push_back(auxPtr->data);
            auxPtr = auxPtr->next;
        }
    }

    SingleLinkedList& operator=(const SingleLinkedList& source)    // Remember that this func must return SingleLinkedList&, so we can do chain assignments and don't lose time making shallow useless copys
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

    ~SingleLinkedList()
    {
        clear();
    }

    bool empty() const
    {
        if(size == 0)
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
            if(listSize == 0) end = begin;
        }
        else if(pos == listSize)
        {
            end->next = new Node(newData, nullptr);
            end = end->next;
        }
        else
        {
            Node* auxPtr = begin;
            for(size_t i = 0; i < pos-1; i++)
            {
                auxPtr = auxPtr->next;
            }
            auxPtr->next = new Node(newData, auxPtr->next);
        }
        listSize++;
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

    void push_back(const T &newData)
    {
        insert(newData, listSize);
    }

    void push_front(const T &newData)
    {
        insert(newData, 0);
    }

    T& front()
    {
        if(begin != nullptr)
            return begin->data;
        else
            throw std::out_of_range("Empty list");
    }

    const T& front() const
    {
        if(begin != nullptr)
            return begin->data;
        else
            throw std::out_of_range("Empty list");
    }

    T& back()
    {
        if(end != nullptr)
            return end->data;
        else
            throw std::out_of_range("Empty list");
    }

    const T& back() const
    {
        if(end != nullptr)
            return end->data;
        else
            throw std::out_of_range("Empty list");
    }

    void pop_back()
    {
        remove(listSize-1);
    }

    void pop_front()
    {
        remove(0);
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