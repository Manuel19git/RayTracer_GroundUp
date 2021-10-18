#include "MyMatrix.h"


//Output and visualize matrix
ostream& operator<<(ostream& out, MyMatrix& matrix)
{
	
	string output = "";
	for (int r = 0; r < matrix.rowSize; ++r)
	{
		for (int c = 0; c < matrix.colSize; ++c)
		{
			out << matrix(r, c) << "  ";
		}
		out << "\n";
	}
	
	
	return out;
}


//Access operation
float& MyMatrix::operator()(const int r, const int c)
{
	return matrix[r][c];
}
