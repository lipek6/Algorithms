#include <stdexcept>
#include <optional>
#include <iostream>

template <typename T>
struct Node
{
    T data;
    Node<T>* next;

    Node<T>(const T& data, Node<T>* next = nullptr) : data(data), next(next) {}
    ~Node<T>() {}
};

template <typename T>
class SingleLinkedList
{
private:  
    Node<T>* begin;
    Node<T>* end;
    size_t listSize;

public:
    // ==========================================
    // CONSTRUCTORS & DESTRUCTORS 
    // ==========================================

    // Default constructor
    SingleLinkedList() : begin(nullptr), end(nullptr), listSize(0) {}
    
    // Default destructor
    ~SingleLinkedList()
    {
        clear();
    }

    // Deep copy constructor
    SingleLinkedList(const SingleLinkedList& source) : begin(nullptr), end(nullptr), listSize(0)
    {
        if(source.begin == nullptr) return;

        Node<T>* auxPtr = source.begin;
        while(auxPtr != nullptr)
        {
            this->push_back(auxPtr->data);
            auxPtr = auxPtr->next;
        }
    }
    
    // Copy assignment operator
    SingleLinkedList& operator=(const SingleLinkedList& source)
    {
        if(this == &source) return *this;

        this->clear();

        Node<T>* auxPtr = source.begin;
        while(auxPtr != nullptr)
        {
            this->push_back(auxPtr->data);
            auxPtr = auxPtr->next;
        }
        return *this;
    }

    // Move constructor
    SingleLinkedList(SingleLinkedList&& source) noexcept : begin(source.begin), end(source.end), listSize(source.listSize)
    {
        source.begin = nullptr;
        source.end = nullptr;
        source.listSize = 0;
    }

    // Move assignment operator
    SingleLinkedList& operator=(SingleLinkedList&& source) noexcept
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

    // Returns the number of node<T>s on list
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

    // Index operator (uses linear search)
    T& operator[](const size_t pos) 
    {
        if(pos >= listSize) throw std::out_of_range("Index out of range");
        
        if(pos == listSize - 1) return end->data;

        Node<T>* auxPtr = begin;        
        for(size_t i = 0; i < pos; i++)
        {
            auxPtr = auxPtr->next;
        }        
        return auxPtr->data;
    }

    const T& operator[](const size_t pos) const
    {
        if(pos >= listSize) throw std::out_of_range("Index out of range");
        
        if(pos == listSize - 1) return end->data;

        Node<T>* auxPtr = begin;        
        for(size_t i = 0; i < pos; i++)
        {
            auxPtr = auxPtr->next;
        }        
        return auxPtr->data;
    }

    // Linear search (return position of the element)
    std::optional<size_t> find(const T &target) const
    {
        if(listSize == 0) return std::nullopt;

        size_t pos = 0;
        Node<T>* auxPtr = begin;

        while(pos < listSize && auxPtr->data != target)
        {
            auxPtr = auxPtr->next;
            pos++;
        }

        if(pos < listSize) 
            return pos;
        return std::nullopt;
    }

    // Linear search (return a pointer to the node<T> where the element is). Added for question 2 of the list 1. Node<T> needs to be a struct defined outside the SingleLinkedList
    Node<T>* find(const T &target) 
    {
        if(listSize == 0) return nullptr;

        size_t pos = 0;
        Node<T>* auxPtr = begin;

        while(pos < listSize && auxPtr->data != target)
        {
            auxPtr = auxPtr->next;
            pos++;
        }

        if(pos < listSize) 
            return auxPtr;
        return nullptr;
    }

    // ==========================================
    // MODIFIERS
    // ==========================================

    // Removes all the node<T>s of the list
    void clear()
    {
        if(begin == nullptr) return;

        Node<T>* auxPtr = begin->next;
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

    // Creates a node<T> with the given data at the given position
    void insert(const T &newData, const size_t pos)
    {
        if(pos > listSize) throw std::out_of_range("Index out of range");

        if(pos == 0)
        {
            begin = new Node<T>(newData, begin);
            if(listSize == 0) end = begin;
        }
        else if(pos == listSize)
        {
            end->next = new Node<T>(newData, nullptr);
            end = end->next;
        }
        else
        {
            Node<T>* auxPtr = begin;
            for(size_t i = 0; i < pos-1; i++)
            {
                auxPtr = auxPtr->next;
            }
            auxPtr->next = new Node<T>(newData, auxPtr->next);
        }
        listSize++;
    }

    // Deletes the node<T> at the given position
    void remove(const size_t pos)
    {
        if(pos >= listSize) throw std::out_of_range("Index out of range");

        Node<T>* auxPtr1 = begin;
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
            Node<T>* auxPtr2 = auxPtr1->next;
            auxPtr1->next = auxPtr2->next;
            
            if(auxPtr2 == end) end = auxPtr1;

            delete auxPtr2;
        }  
        listSize--;
    }

    // Creates a node<T> with the given data at the end of the list
    void push_back(const T &newData) { insert(newData, listSize); }

    // Creates a node<T> with the given data at the begin of the list
    void push_front(const T &newData) { insert(newData, 0); }
    
    // Deletes the last node<T> of the list
    void pop_back(){ remove(listSize - 1); }

    // Deletes the first node<T> of the list
    void pop_front() { remove(0); }

    // ==========================================
    // OVERLOADINGS
    // ==========================================

    // Adds the node<T>s of the rigth list to the end of the left list
    SingleLinkedList& operator+=(const SingleLinkedList& newNode<T>s)
    {
        Node<T> *auxPtr = newNode<T>s.begin;
        while(auxPtr != nullptr)
        {
            this->push_back(auxPtr->data);
            auxPtr = auxPtr->next;
        }
        return *this;
    }

    // Adds the node<T>s of the rigth list to the end of the left list
    SingleLinkedList operator+(const SingleLinkedList& newNode<T>s) const
    {
        SingleLinkedList<T> newList;
        newList += *this;
        newList += newNode<T>s;
        return newList;
    }
};


// Basically a std::pair, but I don't know if I can use a std::pair.
struct teamElement
{
    int team;
    int element;

    teamElement(int team, int element) : team(team), element(element) {}
    ~teamElement() {}

    bool operator==(const teamElement& other) const { return this->team == other.team; }
};

int main(void)
{
    int numTeams; std::cin >> numTeams;

    while(numTeams > 0)                                         // For each test case
    {
        teamElement** teamsArray = new teamElement*[numTeams];  // Basically a set with all the teams in it
        SingleLinkedList<teamElement> queue;                    // The real 'queue', I am using a SingleLinkedList because my queue doesn't allow to insert a guy where his team is, my queue can only insert at the end. 

        for(int i = 0; i < numTeams; i++)                       // For each team
        {
            int teamSize; std::cin >> teamSize;
            teamsArray[i] = new teamElement[teamSize];          // Dynamically allocated Array that will hold all the members of a given team; 

            for(int j = 0; j < teamSize; j++)                   // For each element of the team
                std::cin >> teamsArray[i][j];
        }
        // All teams are already ready at this point

        std::string command; std::cin >> command;
        while(command != "STOP")
        {
            if(command == "ENQUEUE")
            {
                int element; std::cin >> element;
                
                queue.
                queue.find()
                queue(element);
            }




            std::cin >> command;
        }



        std::cin >> numTeams;
    }

}