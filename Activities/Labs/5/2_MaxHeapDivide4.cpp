#include <iostream>
#include "../../../AlgsLib/DataStructures/Includes/Heap.h"

int main(void)
{
    Heap<size_t> maxHeap(16);

    size_t element;
    while(std::cin >> element)
        maxHeap.insert(element);
    
    std::cout << "Antes\n   ";
    maxHeap.print();
    
    maxHeap.topEdit(maxHeap.top() / 4);
    
    std::cout << "Depois\n   ";
    maxHeap.print();
}