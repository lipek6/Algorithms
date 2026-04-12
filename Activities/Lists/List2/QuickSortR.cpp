#include <iostream>
#include <cstdlib>
#include <ctime>


template <typename T>
size_t PartitionR(T* array, size_t left, size_t right)
{
    size_t randomIdx = left + (std::rand() % (right - left + 1));
    std::swap(array[left], array[randomIdx]);
    
    T pivot = array[left];
    size_t i = left;
    size_t j = right + 1;

    while(true)
    {
        while(array[++i] < pivot)           // Searching for element GREATER than the pivot
            if(i == right) break;               // Avoiding going out of bounds to the right
        while(array[--j] > pivot)           // Searching for element SMALLER than the pivot
            if(j == left) break;                // Avoiding going out of bounds to the left

        if(i >= j)                          // When i and j crosses wach other, it's time for the final swap
            break;
        
        std::swap(array[i], array[j]);      // Swap the two out of place elements
    }
    std::swap(array[left], array[j]);
    return j;
}


template <typename T>
void RecursivelyQuickSortR(T* array, size_t left, size_t right)
{
    if(left >= right)
        return;

    size_t pivot = PartitionR(array, left, right);

    if(pivot > 0)
        RecursivelyQuickSortR(array, left, pivot - 1);

    RecursivelyQuickSortR(array, pivot + 1, right);
}


template <typename T>
void QuickSortR(T* array, size_t size)
{
    if(size > 1)
    {
        std::srand(std::time(nullptr));
        RecursivelyQuickSortR(array, 0, size - 1);
    }
}


int main(void)
{
    size_t numNumbers; std::cin >> numNumbers;
    size_t* array = new size_t[numNumbers];
    
    for(size_t i = 0; i < numNumbers; i++)
    {
        size_t number; std::cin >> number;
        array[i] = number;
    }

    QuickSortR(array, numNumbers);
    
    for(size_t i = 0; i < numNumbers; i++)
        std::cout << array[i] << "\n";    

    delete[] array;
}