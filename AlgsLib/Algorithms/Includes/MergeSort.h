// Auxiliar function for TopDownMergeSort that writes on the array on the right order
template <typename T>
void Merge(T* array, T* auxArray, size_t left, size_t right)
{
    for(size_t i = left; i <= right; i++) auxArray[i] = array[i];       // Update auxArray with the sorted array version
    
    size_t middle = left + (right - left) / 2;
    size_t i = left;                                    // Iterator for the left  sorted part of the array
    size_t j = middle + 1;                              // Iterator for the right sorted part of the array

    for(size_t k = left; k <= right; k++)
    {
        if(i > middle)                                  // If remains only elements on the right part 
            array[k] = auxArray[j++];
        
        else if (j > right)                             // If remains only elements on the left part
            array[k] = auxArray[i++];
        
        else if(auxArray[i] < auxArray[j])
            array[k] = auxArray[i++];
        
        else
            array[k] = auxArray[j++];
    }
}


// Auxiliar function for MergeSort that recursively divides the the array and then calls Merge to glue them together on the right order.
template <typename T>
void TopDownMergeSort(T* array, T* auxArray, size_t left, size_t right)
{
    if(left >= right) return;                                                     // Base case, single element array is always sorted

    size_t middle = left + (right - left) / 2;
    TopDownMergeSort(array, auxArray, left, middle);                              // Sort the left  side 
    TopDownMergeSort(array, auxArray, middle + 1, right);                         // Sort the right side

    Merge(array, auxArray, left, right);
}

// Sorts a given array in O(N*logN)
template <typename T>
void MergeSort(T* array, size_t size)
{
    if(size <= 1) return;

    T* auxArray = new T[size];

    TopDownMergeSort(array, auxArray, 0, size - 1);
    delete[] auxArray;
}