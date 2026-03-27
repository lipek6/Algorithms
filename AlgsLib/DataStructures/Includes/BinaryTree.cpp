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
    size_t height;

    void transplant(){}

public:

    BinaryTree() : root(nullptr), numNodes(0), height(0){}
    ~BinaryTree(){}


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
        // HEIGTH NEEDS TO BE INCREMENTED 
    }

    D* search(const K& targetKey)
    {
        Node* auxPtr1 = root;
        Node* auxPtr2 = root;

        while(auxPtr1 != nullptr)
        {
            auxPtr2 = auxPtr1;
            if(targetKey < auxPtr1->key)
                auxPtr1 = auxPtr1->left;
            else if()
                auxPtr1 = auxPtr1->right;
        }
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