#include <iostream>
#include <optional>
#include <stdexcept>

template <typename T>
class BinaryTree
{
private:
    struct Node
    {
        T data;
        Node* parent;
        Node* left;
        Node* rigth;

        Node(const T& data, Node* parent = nullptr, Node* left = nullptr, Node* rigth = nullptr) : data(data), parent(parent), left(left), rigth(rigth){}
        ~Node(){}
    };

    Node* root;
    size_t numNodes;
    size_t height;

    void transplant(){}

public:

    BinaryTree() : root(nullptr), numNodes(0), height(0){}
    ~BinaryTree(){}


    void insert(const T& newData)
    {
        if(numNodes == 0)
        {
            root = new Node(newData, nullptr, nullptr, nullptr);
        }
        else
        {
            Node* auxPtr = root;
            // Add another auxiliar node, it shall point to the node before auxPtr, because when auxPtr becomes nullptr, we can't go back to the parent of nullptr via the auxPtr.
            while(auxPtr != nullptr)
            {
                if (newData <= auxPtr->data)
                    auxPtr = auxPtr->left;
                else
                    auxPtr = auxPtr->rigth;
            }
            
            if(newData <= auxPtr->data)
                auxPtr->left = new Node(newData, auxPtr, nullptr, nullptr);
            else
                auxPtr->rigth = new Node(newData, auxPtr, nullptr, nullptr);
        }
        numNodes++;
        // HEIGTH NEEDS TO BE INCREMENTED 
    }

    void remove(const T& delData)
    {

    }
};






int main (void)
{
    BinaryTree<int> bt;
    bt.insert(1);
    bt.insert(2);
    bt.insert(3);
    bt.insert(5);
    bt.insert(4);
}