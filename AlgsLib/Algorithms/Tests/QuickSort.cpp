#include <iostream>
#include "../Includes/QuickSort.h"

int main(void)
{
    //int array[9] = {54, 26, 93, 17, 77, 31, 44, 55, 20};
    int array[6] = {1, 2, 3, 4, 5, 6};
    QuickSort(array, 6);

    std::cout << "Sorted Array: [";
    for(size_t i = 0; i < 5; i++) std::cout << array[i] << ", ";
    std::cout << array[5] << "]\n";
    
    return 0;
}