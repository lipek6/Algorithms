#include <iostream>
#include "../../../AlgsLib/DataStructures/Includes/Heap.h"


int main()
{
    Heap<size_t> maxHeap(16);

    size_t element;
    while(std::cin >> element)
        maxHeap.insert(element);
    
    maxHeap.print();
}