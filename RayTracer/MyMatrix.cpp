#include "MyMatrix.h"


//Output and visualize matrix
ostream& operator<<(ostream& out, const MyMatrix& matrix)
{
	
	string output = "";
	
	for (int r = 0; r < matrix.rowSize; ++r)
	{
		for (int c = 0; c < matrix.colSize; ++c)
		{
			out << matrix.matrix[r][c] << "  ";	
		}
		out << "\n";
	}
	
	
	return out;
}


//Access operation
float& MyMatrix::operator()(int r, int c)
{
	return matrix[r][c];
}

//Assign operator of regular matrices of the same size
MyMatrix& MyMatrix::operator=(MyMatrix& otherMatrix)
{
	rowSize = otherMatrix.rowSize;
	colSize = otherMatrix.colSize;

	for (int r = 0; r < rowSize; ++r) {
		for (int c = 0; c < colSize; ++c) {
			matrix[r][c] = otherMatrix(r, c);
		}
	}
	return *this;
}

//Comparison operation
bool MyMatrix::operator== (MyMatrix otherMatrix)
{
	bool isEqual = true;
	if (rowSize == otherMatrix.rowSize && colSize == otherMatrix.colSize)
	{
		
		for (int r = 0; r < rowSize; ++r){
			for (int c = 0; c < colSize; ++c){
				cout << isEqual << endl;
				if (!isEqual)
					break;
				else
					isEqual = equalFloat(matrix[r][c], otherMatrix(r,c));
			}
		}

		return isEqual;
	}
	else
		return false;
}

bool MyMatrix::operator!= (MyMatrix otherMatrix)
{
	bool equal = *this == otherMatrix;

	return !equal;
}

//Multiplying two regular matrices of same size 
MyMatrix MyMatrix::operator*(const MyMatrix& otherMatrix)
{
	int matSize = colSize;
	float cellValue;
	MyMatrix resultMatrix(matSize);
	for (int r = 0; r < matSize; ++r) {
		for (int c = 0; c < matSize; ++c) {
			cellValue = 0;
			//Depending on the size of the matrix we multiply more or less values
			for (int i = 0; i < matSize; ++i){
				cellValue += matrix[r][i] * otherMatrix.matrix[i][c];
			}
			resultMatrix.matrix[r][c] = cellValue;

		}
	}
	return resultMatrix;
}
