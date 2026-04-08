#include <iostream>
#include "../Includes/QuickSortR.h"

int main(void)
{
    std::srand(std::time(nullptr));

    int array[9] = {54, 26, 93, 17, 77, 31, 44, 55, 20};
    QuickSortR(array, 9);

    std::cout << "Sorted Array: [";
    for(size_t i = 0; i < 8; i++) std::cout << array[i] << ", ";
    std::cout << array[8] << "]\n";
    
    return 0;
}