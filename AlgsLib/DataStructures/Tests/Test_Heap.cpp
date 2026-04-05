#include "../Includes/Heap.h"


int main(void)
{
    int array[8] = {14, 32, 62, 2, 3445, 431, 2, -12093};
    
    Heap<int, std::greater<int>>::sort(array, 8);

    for(int a : array)
        std::cout << a << " ";
    std::cout << "\n";
}
