#include <iostream>
#include <string>
#include "../Includes/MergeSort.h"

int main(void)
{
    int n; std::cin >> n;
    int* array = new int[n];

    std::string strArray;
    
    char newChar;
    while(std::cin >> newChar)
        strArray += newChar;

    size_t j = 0;
    std::string strCurrentNumber = "";
    for(size_t i = 0; i <= strArray.size(); i++)
    {
        if(strArray[i] == '|' || i == strArray.size())
        {
            array[j++] = std::stoi(strCurrentNumber);
            strCurrentNumber = "";
        }
        else
            strCurrentNumber += strArray[i];
    }
    MergeSort(array, n);
    
    std::cout << "[";
    for(size_t i = 0; i < n - 1; i++)
    {
        std::cout << array[i] << ", ";
    }
    std::cout << array[n - 1] << "]\n";
}