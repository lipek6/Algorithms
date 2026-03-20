#include <iostream>
#include <cassert>
#include <string>
#include "../Includes/SingleLinkedList.h"

// ANSI colors definition
const std::string GREEN = "\033[32m";
const std::string RED = "\033[31m";
const std::string YELLOW = "\033[33m";
const std::string RESET = "\033[0m";

void TestSingleLinkedList()
{
    std::cout << YELLOW << "-> Testing: Single Linked List..." << RESET << "\n";

    SingleLinkedList<int> list;

    // ==========================================
    // INITIAL STATE (CONSTRUCTORS)
    // ==========================================
    assert(list.empty() == true);
    assert(list.size() == 0);

    bool expectedExceptionThrown = false;
    try
    {
        list.front();
        std::cerr << RED << "Failed: Exception not thrown when using .front() in an empty list" << RESET << "\n";
    }
    catch (const std::out_of_range& )
    {
        expectedExceptionThrown = true;
    }
    assert(expectedExceptionThrown == true);

    // ==========================================
    // 2. INSERÇÕES (push_back e push_front)
    // ==========================================
    list.push_back(10);  // [10]
    list.push_back(20);  // [10, 20]
    list.push_front(5);  // [5, 10, 20]
    list.insert(15, 2);  // [5, 10, 15, 20]

    assert(list.size() == 4);
    assert(list.front() == 5);
    assert(list.back() == 20);
    assert(list[2] == 15);

    // ==========================================
    // 3. BUSCA (find)
    // ==========================================
    auto busca1 = list.find(15);
    assert(busca1.has_value() == true);
    assert(busca1.value() == 2);

    auto busca2 = list.find(99); // Valor inexistente
    assert(busca2.has_value() == false);

    // ==========================================
    // 4. REMOÇÕES (pop_front, pop_back e remove)
    // ==========================================
    list.pop_front();    // [10, 15, 20]
    assert(list.front() == 10);
    assert(list.size() == 3);

    list.pop_back();     // [10, 15]
    assert(list.back() == 15);
    assert(list.size() == 2);

    list.remove(0);      // [15]
    assert(list.front() == 15);
    assert(list.size() == 1);

    list.remove(0);      // [] Lista volta a ficar vazia
    assert(list.empty() == true);

    // ==========================================
    // 5. REGRA DOS CINCO (Testando cópia)
    // ==========================================
    list.push_back(100);
    list.push_back(200);

    SingleLinkedList<int> listaCopia = list; // Invoca o Copy Constructor
    
    assert(listaCopia.size() == 2);
    assert(listaCopia[1] == 200);
    
    // Garantindo que é uma "Deep Copy" (Cópia profunda e independente)
    listaCopia[0] = 999;
    assert(list[0] == 100); // A lista original deve permanecer intacta!

    // ==========================================
    // FINALIZAÇÃO
    // ==========================================
    std::cout << GREEN << "   [PASSED] All Single Linked List tests passed successfully!" << RESET << "\n\n";
}