#include "GraphsCommons.h"
#include "../Vector.h"


// flattening the 2D data into a single 1D vector (width * height) and calculating offsets manually for better memory locality.
// INFINITY_VAL == Invalid edge



int main()
{
    AM am;

    am.addNode();
    am.addNode();
    am.addNode();
    am.addNode();
    am.addNode();
    am.addEdge(0, 1);
    am.addEdge(0, 2);
    am.addEdge(0, 3);
    am.addEdge(0, 4);
    am.addEdge(4, 3);
    am.addEdge(3, 2);
    am.addEdge(2, 1);
    am.removeEdge(0, 1);
    am.removeNode(0);
}