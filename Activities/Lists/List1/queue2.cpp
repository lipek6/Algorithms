#include <stdexcept>
#include <optional>
#include <iostream>

template <typename K, typename D>
class BinaryTree
{
private:
    struct Node
    {
        K key;
        D data;
        Node* parent;
        Node* left;
        Node* right;

        Node(const K& key, const D& data, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr) : key(key), data(data), parent(parent), left(left), right(right){}
        ~Node(){}
    };

    Node* root;
    size_t numNodes;

    Node* searchNode(const K& targetKey)
    {
        Node* auxPtr1 = root;
        while(auxPtr1 != nullptr)
        {
            if(targetKey < auxPtr1->key)
                auxPtr1 = auxPtr1->left;
            else if(targetKey > auxPtr1->key)
                auxPtr1 = auxPtr1->right;
            else
                break;
        }
        return auxPtr1;
    }

    Node* minimum(Node* subtreeRoot)
    {            
        while(subtreeRoot->left != nullptr)
        {
            subtreeRoot = subtreeRoot->left;
        }
        return subtreeRoot;
    }

    Node* maximum(Node* subtreeRoot)
    {
        while(subtreeRoot->right != nullptr)
        {
            subtreeRoot = subtreeRoot->right;
        }
        return subtreeRoot;
    }

    Node* successor(Node* node)
    {
        if(node->right != nullptr)
            return minimum(node->right);

        Node* auxPtr1 = node->parent;
        while(auxPtr1 != nullptr && auxPtr1->right == node)
        {
            node = auxPtr1;
            auxPtr1 = auxPtr1->parent;
        }
        return auxPtr1;
    }

    Node* predecessor(Node* node)
    {
        if(node->left != nullptr)
            return maximum(node->left);

        Node* auxPtr1 = node->parent;
        while(auxPtr1 != nullptr && auxPtr1->left == node)
        {
            node = auxPtr1;
            auxPtr1 = auxPtr1->parent;
        }        
        return auxPtr1;
    }

    void printInOrder(Node* subtreeRoot, std::ostream& stream)
    {
        if(subtreeRoot == nullptr) return;

        printInOrder(subtreeRoot->left, stream);
        stream << subtreeRoot->key << "\n";
        printInOrder(subtreeRoot->right, stream);
    }

    void printPreOrder(Node* subtreeRoot, std::ostream& stream)
    {
        if(subtreeRoot == nullptr) return;

        stream << subtreeRoot->key << "\n";
        printPreOrder(subtreeRoot->left, stream);
        printPreOrder(subtreeRoot->right, stream);
    }
    
    void printPostOrder(Node* subtreeRoot, std::ostream& stream)
    {
        if(subtreeRoot == nullptr) return;
        
        printPostOrder(subtreeRoot->left, stream);
        printPostOrder(subtreeRoot->right, stream);
        stream << subtreeRoot->key << "\n";
    }

    void transplant(Node* subtreeRoot1, Node* subtreeRoot2)
    {
        if(subtreeRoot1->parent == nullptr)                     // If it is root, set root to subtreeRoot2
            root = subtreeRoot2;
        else if(subtreeRoot1->parent->left == subtreeRoot1)     // If it is left child, set parents left child to subtreeRoot2
            subtreeRoot1->parent->left = subtreeRoot2;
        else
            subtreeRoot1->parent->right = subtreeRoot2;         // If it is right child, set parents right child to subtreeRoot2
        
        if(subtreeRoot2 != nullptr)                             // Set subtreeRoot2 parent if subtreeRoot2 is not null
            subtreeRoot2->parent = subtreeRoot1->parent;
    }

    void clearRecursively(Node* delNode)            // Stack Overflow retarded
    {
        if(delNode != nullptr)
        {
            clearRecursively(delNode->left);
            clearRecursively(delNode->right);
            delete delNode;
        }
    }



public:
    // TODO: Add copy constructors for deep copy and maybe use some move constructors to make things faster (operator overloading would be cool too).
    BinaryTree() : root(nullptr), numNodes(0) {}
    ~BinaryTree() { clear(); }

    void clear()
    {
        clearRecursively(root);
        root = nullptr;
        numNodes = 0;
    }

    void insert(const K& newKey, const D& newData)
    {
        if(numNodes == 0)
        {
            root = new Node(newKey, newData, nullptr, nullptr, nullptr);
        }
        else
        {
            Node* auxPtr1 = root;
            Node* auxPtr2 = root;

            while(auxPtr1 != nullptr)
            {
                auxPtr2 = auxPtr1;
                if (newKey <= auxPtr1->key)
                    auxPtr1 = auxPtr1->left;
                else
                    auxPtr1 = auxPtr1->right;
            }
            
            if(newKey <= auxPtr2->key)
                auxPtr2->left = new Node(newKey, newData, auxPtr2, nullptr, nullptr);
            else
                auxPtr2->right = new Node(newKey, newData, auxPtr2, nullptr, nullptr);
        }
        numNodes++;
    }

    D* search(const K& targetKey)
    {
        Node* targetNode = searchNode(targetKey);
        
        if(targetNode != nullptr)
            return &(targetNode->data);
        return nullptr;
    }

    D* minimum(const K& subtreeRootKey)
    {
        Node* targetNode = searchNode(subtreeRootKey);
        if(targetNode == nullptr) return nullptr;
        
        Node* minNode = minimum(targetNode);
        if(minNode == nullptr) return nullptr;
        
        return &(minNode->data);
    }

    D* minimum()
    {
        if(root == nullptr) return nullptr;
        
        Node* minNode = minimum(root);
        if(minNode == nullptr) return nullptr;
        
        return &(minNode->data);
    }

    D* maximum(const K& subtreeRootKey)
    {
        Node* targetNode = searchNode(subtreeRootKey);
        if(targetNode == nullptr) return nullptr;

        Node* maxNode = maximum(targetNode);
        if(maxNode == nullptr) return nullptr;

        return &(maxNode->data);
    }

    D* maximum()
    {
        if(root == nullptr) return nullptr;

        Node* maxNode = maximum(root);
        if(maxNode == nullptr) return nullptr;

        return &(maxNode->data);
    }

    D* successor(const K& node)
    {
        Node* targetNode = searchNode(node);
        if(targetNode == nullptr) return nullptr;

        Node* succNode = successor(targetNode);
        if(succNode == nullptr) return nullptr;

        return &(succNode->data);
    }

    D* predecessor(const K& node)
    {
        Node* targetNode = searchNode(node);
        if(targetNode == nullptr) return nullptr;

        Node* predNode = predecessor(targetNode);
        if(predNode == nullptr) return nullptr;

        return &(predNode->data);
    }

    // TODO: I should add a version of these traverse functions that feeds a file stream, std::ofstream;
    void printInOrder(const K& subtreeRootKey, std::ostream& stream = std::cout) { printInOrder(searchNode(subtreeRootKey), stream); }

    void printInOrder(std::ostream& stream = std::cout) { printInOrder(root, stream); }

    void printPreOrder(const K& subtreeRootKey, std::ostream& stream = std::cout) { printPreOrder(searchNode(subtreeRootKey), stream); }
    
    void printPreOrder(std::ostream& stream = std::cout) { printPreOrder(root, stream); }

    void printPostOrder(const K& subtreeRootKey, std::ostream& stream = std::cout) { printPostOrder(searchNode(subtreeRootKey), stream);}

    void printPostOrder(std::ostream& stream = std::cout) { printPostOrder(root, stream); }

    void remove(const K& delKey)
    {
        Node* delNode = searchNode(delKey);
        if(delNode == nullptr) throw std::out_of_range("Key is not on the tree");
        
        if(delNode->left == nullptr)           
        {
            transplant(delNode, delNode->right);
        }                     
        else if(delNode->right == nullptr)
        {
            transplant(delNode, delNode->left);
        }
        else
        {
            Node* successorNode = successor(delNode);
            if(successorNode != delNode->right)
            {
                transplant(successorNode, successorNode->right);
                successorNode->right = delNode->right;
                successorNode->right->parent = successorNode;
            }   
            transplant(delNode, successorNode);
            successorNode->left = delNode->left;
            successorNode->left->parent = successorNode;
        }

        delete delNode;
        numNodes--;
    }

    bool empty() const{ return numNodes == 0; }
};


template <typename T>
struct Node
{
    T data;
    Node<T>* next;

    Node(const T& data, Node<T>* next = nullptr) : data(data), next(next) {}
    ~Node() {}
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

    // Adds the nodes of the rigth list to the end of the left list
    SingleLinkedList& operator+=(const SingleLinkedList& newNodes)
    {
        Node<T>* auxPtr = newNodes.begin;
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

    // ==========================================
    // CRAZY FUCKERY
    // ==========================================

    void overrideListSize(size_t newSize) { listSize = newSize; };      // THIS SHOULD NOT EXIST OUTSIDE OF THIS FILE. This is here just to allow me to pass this question, latter I really need to refactor everything here, this code is so fucked.

};




template <typename T, typename E>
class TeamQueue
{
private:
    // Basically a std::pair, but I don't know if I can use a std::pair.
    struct teamElement
    {
        T team;
        E element;

        teamElement(T team, E element) : team(team), element(element) {}
        ~teamElement() {}

        bool operator==(const teamElement& other) const { return this->team == other.team; }    // This overloading will make it easier to use the SingleLinkedList<teamElement>
        bool operator!=(const teamElement& other) const { return this->team != other.team; }    // This overloading will make it easier to use the SingleLinkedList<teamElement>
    };

    SingleLinkedList<teamElement> list;

public:
    void insert(const T& team, const E& element)
    {
        teamElement newMember(team, element);
        Node<teamElement>* teamPosition = list.find(newMember);
        
        if(teamPosition == nullptr)
            list.push_back(newMember);
        else
        {
            while(teamPosition != nullptr && teamPosition->next->data == newMember)
                teamPosition = teamPosition->next;
            
            teamPosition->next = new Node<teamElement>(newMember, teamPosition->next);
            list.overrideListSize(list.size()+1);
        }
    }

    E pop()
    {
        teamElement member = list.front();
        list.pop_front();
        return member.element;
    }

    TeamQueue(){}
    ~TeamQueue(){ list.~SingleLinkedList(); }
};





int main(void)
{
    int testCase = 1; 
    int numTeams; std::cin >> numTeams;

    while(numTeams > 0)                                         // For each test case
    {
        std::cout << "Scenario #" << testCase << "\n";
        BinaryTree<int, int> teamSet;                           // Each node has a element as key and a team as data. This will be used to figure out the team of a given element at ENQUEUE x
        
        // GET TEAMS AND ELEMENTS ---------------------------------------------------------------------
        for(int team = 0; team < numTeams; team++)              // For each team
        {
            int teamSize; std::cin >> teamSize;
            
            for(int element = 0; element < teamSize; element++) // For each element of the team
            {
                int newElementID; std::cin >> newElementID;
                teamSet.insert(newElementID, team);
            }
        }
        
        // PROCESS COMMANDS ---------------------------------------------------------------------
        TeamQueue<int, int> queue;                    // The real 'queue', I am using a SingleLinkedList (TeamQueue is basically that) because my queue doesn't allow to insert a guy where his team is, my queue can only insert at the end. 
        
        std::string command = " ";
        while(command != "STOP")
        {
            std::cin >> command;
            if(command == "ENQUEUE")
            {
                int element; std::cin >> element;
                int* teamPtr = teamSet.search(element);
                
                if(teamPtr == nullptr) continue;

                queue.insert(*teamPtr, element);
            }
            else if(command == "DEQUEUE")
            {
                int element = queue.pop();
                std::cout << element << "\n";
            }          
        }
        testCase++;
        std::cout << "\n";
        std::cin >> numTeams;
    }
}

