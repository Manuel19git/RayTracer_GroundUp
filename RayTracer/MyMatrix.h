#pragma once
#include <iostream>
#include "tuples.h"

using namespace std;

class MyMatrix
{
public:
	//Constructor
	MyMatrix(int size) : rowSize(size), colSize(size) {
		matrix = new float* [size];
		for (int r = 0; r < size; ++r)
		{
			matrix[r] = new float[size];
			std::fill_n(matrix[r], size, 0);
		}
	}
	MyMatrix(int rows, int cols) : rowSize(rows), colSize(cols) {
		matrix = new float* [rows];
		for (int r = 0; r < rows; ++r)
		{
			matrix[r] = new float[cols];
			std::fill_n(matrix[r], cols, 0);
		}
	}

	//Destructor
	~MyMatrix()
	{
		for (int r = 0; r < rowSize; ++r)
		{
			delete matrix[r];
		}
		delete matrix;
	}

	//output matrix operation
	friend ostream& operator<<(ostream& out, MyMatrix& matrix);

	//Access operator (return pointer will allow for insertion as well)
	float& operator()(const int r, const int c);

	//Size
	float getSize()
	{
		return colSize * rowSize;
	}

	int colSize;
	int rowSize;
	float** matrix;
};