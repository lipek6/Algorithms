#include <iostream>
#include <cassert>
#include "../Includes/SingleLinkedList.h"

void TestSingleLinkedList()
{
    std::cout << "-> Testing: Single Linked List..." << std::endl;

    SingleLinkedList<int> list;

    assert(list.empty() == true);
    assert(list.size() == 0);

    bool expectedExceptionThrown = false;
    try
    {
        list.front();
        std::cerr << "Failed: Exception not throw when using .front() in an empty list";
    }
    catch (const std::out_of_range& )
    {

    }


    assert(list.front() == std::out_of_range)

}