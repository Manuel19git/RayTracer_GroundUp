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
	/*
	~MyMatrix()
	{
		for (int r = 0; r < rowSize; ++r)
		{
			delete []matrix[r];
		}
		delete []matrix;
	}
	*/
	//output matrix operation
	friend ostream& operator<<(ostream& out, const MyMatrix& matrix);

	//Access operator (return pointer will allow for insertion as well)
	float& operator()(int r, int c);

	//Size
	float getSize()
	{
		return colSize * rowSize;
	}

	//Assign operator
	MyMatrix operator=(MyMatrix& otherMatrix);
	//Assign operator
	MyMatrix operator=(MyMatrix otherMatrix);

	//Comparing matrix
	bool operator== (MyMatrix otherMatrix);
	bool operator!= (MyMatrix otherMatrix);

	//Multiplication
	MyMatrix operator*(const MyMatrix& otherMatrix);

	//Tuple multiplication
	Tuple operator*(const Tuple& otherTuple);

	//Scalar multiplication
	MyMatrix operator*(float const& scalar);
	
	//Scalar division
	MyMatrix operator/(float const& scalar);

	void remove()
	{
		for (int r = 0; r < rowSize; ++r)
		{
			delete[]matrix[r];
		}
		delete[]matrix;
	}

	int colSize;
	int rowSize;
	float** matrix;
};

//Non class members
/*
//Scalar multiplication
template<typename T>
MyMatrix operator*(T const& scalar, MyMatrix matrix)
{
	MyMatrix resultMatrix(matrix.rowSize, matrix.colSize);

	for (int r = 0; r < matrix.rowSize; ++r)
	{
		for (int c = 0; c < matrix.colSize; ++c)
		{
			resultMatrix(r, c) = matrix(r, c) * scalar;
		}
	}
	return resultMatrix;
}
//Scalar division
template<typename T>
MyMatrix operator/(T const& scalar, MyMatrix matrix)
{
	MyMatrix resultMatrix(matrix.rowSize, matrix.colSize);

	for (int r = 0; r < matrix.rowSize; ++r)
	{
		for (int c = 0; c < matrix.colSize; ++c)
		{
			resultMatrix(r, c) = scalar / matrix(r, c);
		}
	}
}
*/
//Identity matrix
MyMatrix I_Matrix();

//Transposing matrices
MyMatrix transpose(MyMatrix matrix);

//Return determinant of a matrix
float determinant(MyMatrix matrix);

//Return submatrix deleting row and column of selected cell
MyMatrix submatrix(MyMatrix matrix, int row, int col);

//Minor -> determinant of cell submatrix of a matrix
float minor(MyMatrix matrix, int row, int col);

//Cofactor of a matrix
float cofactor(MyMatrix matrix, int row, int col);

//Check if a matrix is invertible
bool isInvertible(MyMatrix matrix);

//Return the inverse of a matrix
MyMatrix inverse(MyMatrix matrix);

//View transform matrix
MyMatrix view_transform(myPoint from, myPoint to, myVector up);

//------------------Transform operations------------------
//Translation
MyMatrix translation(float x, float y, float z);

//Scaling
MyMatrix scaling(float x, float y, float z);

//Rotation matrix in x-axis
MyMatrix rotation_x(float radians);

//Rotation matrix in y-axis
MyMatrix rotation_y(float radians);

//Rotation matrix in z-axis
MyMatrix rotation_z(float radians);

//Shearing
MyMatrix shearing(float x_y, float x_z, float y_x, float y_z, float z_x, float z_y);
