#include <iostream>
#include <exception>
#include <fstream>
#include <string>


int main(void)
{
    std::ifstream inputFile("zin.in");
    std::ofstream outputFile("zin.out");
    
    if(!inputFile.is_open())
        throw std::runtime_error("Couldn't open input file");

    if(!outputFile.is_open())
        throw std::runtime_error("Couldn't open output file");



    outputFile.clear();

    while(!inputFile.eof())
    {
        std::string trash = "";
        std::string valid = ""; 
        


        // Get Id
        inputFile  >> valid;
        outputFile << valid << " ";
        


        // Get country name
        while(trash[trash.size() - 2] != '\'')
        {
            inputFile >> trash;
        }
        
        while(valid[valid.size() - 2] != '\'')
        {
            inputFile  >> valid;
            outputFile << valid << " ";
        }



        // Get phone code
        inputFile  >> trash;
        inputFile  >> trash;
        inputFile  >> valid;        
        outputFile << valid;



        // Jump line
        outputFile << "\n";
    }
}