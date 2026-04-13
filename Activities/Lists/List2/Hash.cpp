#include <iostream>

struct Chemical
{
    std::string name;
    size_t amount;

    std::istream& operator>>(std::istream& input, std::string& name, size_t amount) : name(name), amount(amount) {}

};

int main(void)
{
    size_t numOperations; std::cin >> numOperations;
    
    for(size_t i = 0; i < numOperations; i++)
    {
        std::string operation; std::cin >> operation;
        if(operation == "ENTRADA")
        {
            Chemical newChemical; std::cin >> newChemical;
        }
        else if(operation == "CONSUMO")
        {
            continue;
        }
        else if(operation == "CONSULTA")
        {
            continue;
        }
        else //if(operation == "CRITICOS")
        {
            continue;
        }
    }



}