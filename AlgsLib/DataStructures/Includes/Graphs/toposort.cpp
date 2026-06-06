#include <iostream>
#include "AL.h"

int main()
{   
   AL graph;
   size_t num_nodes = 15;

    for(size_t i = 0; i < num_nodes; i++)
        graph.addNode();

    // EDGES:
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);
    graph.addEdge(1, 4);
    graph.addEdge(1, 5);
    graph.addEdge(4, 7);
    graph.addEdge(7, 9);
    graph.addEdge(5, 7);
    graph.addEdge(5, 8);
    graph.addEdge(8, 9);
    graph.addEdge(2, 5);
    graph.addEdge(2, 6);
    graph.addEdge(6, 8);
    graph.addEdge(3, 6);
    graph.addEdge(10, 11);
    graph.addEdge(10, 12);
    graph.addEdge(11, 13);
    graph.addEdge(12, 13);
    graph.addEdge(13, 14);
    graph.addEdge(12, 14);


    Vector<size_t> topologically_sorted_vector;
    graph.toposort(0, true, topologically_sorted_vector);


    std::cout << "Topological order:" << std::endl;
    
    for(size_t i = 0; i < topologically_sorted_vector.size(); i++)
    {
        std::cout << topologically_sorted_vector[i] << "  ";
    }
    
    std::cout << std::endl;
}



