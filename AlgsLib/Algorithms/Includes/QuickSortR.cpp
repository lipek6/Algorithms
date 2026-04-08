#include <iostream>

template <typename T>
size_t Partition(T* array, size_t left, size_t right)
{
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
void RecursivelyQuickSort(T* array, size_t left, size_t right)
{
    if(left >= right)
        return;

    size_t pivot = Partition(array, left, right);

    if(pivot > 0)
        RecursivelyQuickSort(array, left, pivot - 1);

    RecursivelyQuickSort(array, pivot + 1, right);
}


template <typename T>
void QuickSort(T* array, size_t size)
{
    if(size <= 1)
        return;
    RecursivelyQuickSort(array, 0, size - 1);
}


int main(void)
{
    int array[9] = {54, 26, 93, 17, 77, 31, 44, 55, 20};
    QuickSort(array, 9);

    std::cout << "Sorted Array: [";
    for(size_t i = 0; i < 8; i++) std::cout << array[i] << ", ";
    std::cout << array[8] << "]\n";
    
    return 0;
}