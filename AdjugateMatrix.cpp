#include <iostream>

template <typename T>
class Matrix
{
private:
    size_t numRows;
    size_t numCols;
    T* matrix;

public:
    Matrix(size_t numRows, size_t numCols) : numRows(numRows), numCols(numCols), matrix(new T[numRows * numCols])
    ~Matrix() {delete[] matrix;}

}




int main(void)
{

}