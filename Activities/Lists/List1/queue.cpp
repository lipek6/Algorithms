// TODO LATER: Refactorize this code to create a proper TeamQueue data structure (for usage outside this question).
#include <stdexcept>
#include <optional>
#include <iostream>

template <typename T>
class SingleLinkedList
{
private:
    struct Node
    {
        T data;
        Node* next;

        Node(const T& data, Node* next = nullptr) : data(data), next(next) {}
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

        Node* auxPtr = source.begin;
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

        Node* auxPtr = source.begin;
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
        
        if(pos == listSize - 1) return end->data;

        Node* auxPtr = begin;        
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

        Node* auxPtr = begin;        
        for(size_t i = 0; i < pos; i++)
        {
            auxPtr = auxPtr->next;
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

    // Deletes the node at the given position
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
    SingleLinkedList& operator+=(const SingleLinkedList& newNodes)
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
    SingleLinkedList operator+(const SingleLinkedList& newNodes) const
    {
        SingleLinkedList<T> newList;
        newList += *this;
        newList += newNodes;
        return newList;
    }
};

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

int elementToTeam[1000000];                                     // Each index represents an element, the data at an index is the team of the element

int main(void)
{
    int testCase = 1;
    int numTeams;
    while(std::cin >> numTeams && numTeams > 0)
    {
        std::cout << "Scenario #" << testCase << "\n";
        
        // SETUP
        for(int team = 0; team < numTeams; team++)
        {
            int teamSize; std::cin >> teamSize;
            for(int i = 0; i < teamSize; i++)
            {
                int element; std::cin >> element;
                elementToTeam[element] = team;
            }
        }

        // COMANDS
        Queue<int> teamsQueue;                                          // Queue that represents the order of the teams.
        Queue<int>* internalTeamQueue = new Queue<int>[numTeams];       // Array where each index has a queue that represents how the elements are organized in their team.


        std::string cmd; std::cin >> cmd;
        while(cmd != "STOP")
        {
            if(cmd == "ENQUEUE")
            {
                int x; std::cin >> x;
                if(!internalTeamQueue[elementToTeam[x]].empty())        // If the team is present on the queue 
                {
                    internalTeamQueue[elementToTeam[x]].push(x);
                }
                else
                {
                    teamsQueue.push(elementToTeam[x]);             // Now the team is present on the queue
                    internalTeamQueue[elementToTeam[x]].push(x);
                }
            }
            else
            {
                std::cout << internalTeamQueue[teamsQueue.front()].front() << "\n";
                internalTeamQueue[teamsQueue.front()].pop();
                
                if(internalTeamQueue[teamsQueue.front()].empty())       // If the top team has no more elements, pop the team out.
                    teamsQueue.pop();
            }

            std::cin >> cmd;
        }

        std::cout << "\n"; testCase++;
        delete []internalTeamQueue;                                     // Memory leak solved, poor server...
    }
}