#include <iostream>
#include <stdexcept>

// Using 1 as the initial height of the nodes

template <typename K, typename D>
class BinaryTree_Avl
{
private:
    struct Node
    {
        K key;
        D data;
        int height;

        Node* parent;
        Node* left;
        Node* right;

        Node(const K& key, const D& data, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr) : key(key), data(data), height(1), parent(parent), left(left), right(right){}
        ~Node(){}
    };

    Node* root;
    size_t numNodes;


    int getHeight(Node* node)
    {
        if(node == nullptr) return 0;
        return node->height;
    }

    int getBalanceFactor(Node* node)
    {
        if(node == nullptr) return 0;
        return getHeight(node->right) - getHeight(node->left); 
    }

    void updateHeight(Node* node)
    {
        if(node == nullptr) return;
        
        int rightHeight = getHeight(node->right);           // I can't use std::max in this class
        int leftHeight  = getHeight(node->left);

        if(rightHeight >= leftHeight)
            node->height = rightHeight + 1;
        else
            node->height = leftHeight + 1;
    }

    void rightRotate(Node* unbalancedNode)
    {
        Node* leftNode  = unbalancedNode->left;
        Node* leftRightNode = leftNode->right;
        
        unbalancedNode->left = leftRightNode;

        if(leftRightNode != nullptr)
            leftRightNode->parent = unbalancedNode;

        leftNode->right = unbalancedNode;

        if(unbalancedNode->parent == nullptr)
            root = leftNode;
        else if(unbalancedNode == unbalancedNode->parent->right)
            unbalancedNode->parent->right = leftNode;
        else
            unbalancedNode->parent->left = leftNode;

        leftNode->right = unbalancedNode;
        unbalancedNode->parent = leftNode;

        updateHeight(unbalancedNode);
        updateHeight(leftNode);
    }

    void leftRotate(Node* y)
    {

    }

    // Place we want to move, then node we want to move there
    void transplant(Node* subtreeRoot1, Node* subtreeRoot2)     // Might need some changes on the AVL tree
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

    void insert(const K& newKey, const D& newData) // Might need some changes on the AVL tree
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

    void remove(const K& delKey) // Might need some changes on the AVL tree
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