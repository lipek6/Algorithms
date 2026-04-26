#include <iostream>

int main()
{
    Graph<std::string, int> G(false);
    
    std::string Recife = "Recife";
    std::string Olinda = "Olinda";
    std::string Paulista = "Paulista";
    std::string Camaragibe = "Camaragibe";
    std::string Petrolina = "Petrolina";

    G.addNode(Recife);
    G.addNode(Olinda);
    G.addNode(Paulista);
    G.addNode(Camaragibe);
    G.addNode(Petrolina);
    G.addEdge(Recife, Olinda, 2);
    G.addEdge(Olinda, Paulista, 2);
    G.addEdge(Paulista, Camaragibe, 2);
    G.addEdge(Camaragibe, Petrolina, 5);
    G.addEdge(Recife, Petrolina, 10);
    G.removeNode(Petrolina);
    G.removeEdge(Recife, Olinda);
}