#include <iostream>
#include <optional>
#include <stdexcept>

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

    void transplant(){}

public:

    BinaryTree() : root(nullptr), numNodes(0) {}
    ~BinaryTree(){}     // There is memory leak. should be fixed when I add a destructor

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

        if(delNode->left == nullptr)                                // No left child - 1 or 0 children
        {            
            if(delNode->right == nullptr)                           // No children
            {
                if(delNode->parent->right == delNode)
                    delNode->parent->right = nullptr;
                else
                    delNode->parent->left = nullptr;
            }
            else                                                    // 1 child, right
            {
                if(delNode->parent->right == delNode)
                    delNode->parent->right = delNode->right;
                else
                    delNode->parent->left  = delNode->right;

                delNode->right->parent = delNode->parent;
            }
        }
        else                                                    // Left child - 1 or 2 children
        {
            if(delNode->right == nullptr)                       // 1 child, left
            {
                if (delNode->parent->right == delNode)
                    delNode->parent->right = delNode->left;
                else
                    delNode->parent->left = delNode->left;

                delNode->left->parent = delNode->parent;
            }
            else                                                // 2 children
            {
                Node* successorNode = successor(delNode);

                if(successorNode == delNode->right)
                {
                    if (delNode->parent->right == delNode)
                        delNode->parent->right = successorNode;
                    else
                        delNode->parent->left = successorNode;

                    successorNode->parent = delNode->parent;
                    successorNode->left   = delNode->left;
                }
                else                                            // Successor is on the minimum of the right subtree.
                {
                    std::swap(successorNode, successorNode->right);
                    
                    successorNode->right  = delNode->right;
                    successorNode->left   = delNode->left;
                    successorNode->parent = delNode->parent;

                    if(delNode->parent->right == delNode)
                        delNode->parent->right = successorNode;
                    else
                        delNode->parent->left  = successorNode;
                }
            }
        }

        delete delNode;
        numNodes--;
    }
};






int main (void)
{
    BinaryTree<int, int> bt;
    
    int key;
    while(std::cin >> key)
    {
        bt.insert(key, 0);
    }    

    std::cin >> key;
    bt.remove(key);
    
    bt.printPostOrder();
 




}
// 22 28 25 20 26 35 34 33 32 31 29 30 42 40 49;
// 22 20 16 11 18 21 28 23 32 34 30 29