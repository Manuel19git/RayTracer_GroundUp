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
MyMatrix MyMatrix::operator=(MyMatrix& otherMatrix)
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

MyMatrix MyMatrix::operator=(MyMatrix otherMatrix)
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

//Multiplying a matrix 4x4 with a tuple 
Tuple MyMatrix::operator*(const Tuple& otherTuple) 
{
	Tuple resultTuple;
	
	//Each value of the tuple initialized to zero
	float cellValue[4];
	fill_n(cellValue, 4, 0);
	


	for (int r = 0; r < 4; ++r) {

		//Tuples are always the same size
		cellValue[r] += matrix[r][0] * otherTuple.x;
		cellValue[r] += matrix[r][1] * otherTuple.y;
		cellValue[r] += matrix[r][2] * otherTuple.z;
		cellValue[r] += matrix[r][3] * otherTuple.w;

	}
	resultTuple.x = cellValue[0];
	resultTuple.y = cellValue[1];
	resultTuple.z = cellValue[2];
	resultTuple.w = cellValue[3];


	return resultTuple;
}
//Scalar multiplication
MyMatrix MyMatrix::operator*(float const& scalar)
{
	MyMatrix resultMatrix(rowSize, colSize);

	for (int r = 0; r < rowSize; ++r)
	{
		for (int c = 0; c < colSize; ++c)
		{
			resultMatrix(r, c) = matrix[r][c] * scalar;
		}
	}
	return resultMatrix;
}

//Scalar division
MyMatrix MyMatrix::operator/(float const& scalar)
{
	MyMatrix resultMatrix(rowSize, colSize);

	for (int r = 0; r < rowSize; ++r)
	{
		for (int c = 0; c < colSize; ++c)
		{
			resultMatrix(r, c) = matrix[r][c] / scalar;
		}
	}
	return resultMatrix;
}

//Non member functions
//Identity Matrix
MyMatrix I_Matrix()
{
	MyMatrix identity(4);

	for (int i = 0; i < 4; ++i)
	{
		identity(i, i) = 1;
	}
	return identity;
}

//Transposing matrices
MyMatrix transpose(MyMatrix matrix)
{
	int matSize = matrix.colSize;
	MyMatrix resultMatrix(matSize);

	for (int r = 0; r < matSize; ++r)
	{
		for (int c = 0; c < matSize; ++c)
		{
			resultMatrix(r, c) = matrix(c, r);
		}
	}

	return resultMatrix;
}

//Return determinant of a 2x2 matrix
float determinant(MyMatrix matrix)
{
	float determinant = 0;
	if (matrix.rowSize == 2 && matrix.colSize == 2)
		return (matrix(0, 0) * matrix(1, 1)) - (matrix(0, 1) * matrix(1, 0));
	else
	{
		for (int c = 0; c < matrix.colSize; ++c)
		{
			determinant += matrix(0, c) * cofactor(matrix, 0, c);
		}

		return determinant;
	}
	
}

//Return submatrix deleting row and column of selected cell
MyMatrix submatrix(MyMatrix matrix, int row, int col)
{
	int submatrix_handler = 0; //index of matrix but in 1D
	//We create smaller matrix
	if (matrix.rowSize > 2 && matrix.colSize > 2)
	{
		MyMatrix subMatrix(matrix.rowSize - 1, matrix.colSize - 1);
		
		for (int r = 0; r < matrix.rowSize; ++r)
		{
			for (int c = 0; c < matrix.colSize; ++c)
			{
				int submatrix_row = submatrix_handler / subMatrix.colSize;
				int submatrix_col = submatrix_handler - (submatrix_row * subMatrix.colSize);

				if (r != row && c != col)
				{
					subMatrix(submatrix_row, submatrix_col) = matrix(r, c);
					submatrix_handler++;
				}
					
			}
		}

		return subMatrix;

	}
	else
	{
		MyMatrix zero(0);
		return zero;
	}

		
}

//Minor -> determinant of submatrix
float minor(MyMatrix matrix, int row, int col)
{
	return determinant(submatrix(matrix, row, col));
}

//Cofactor of a 3x3 matrix
float cofactor(MyMatrix matrix, int row, int col)
{
	float cofactor = minor(matrix, row, col);
	if ((row + col) % 2 == 0) //Not an odd number
		return cofactor;
	else                      //Odd number
		return -cofactor;
}

//Check if a matrix is invertible
bool isInvertible(MyMatrix matrix)
{
	float d = determinant(matrix);

	if (d == 0) return false;
	else return true;
}

//Return the inverse of a matrix
MyMatrix inverse(MyMatrix matrix)
{
	if (!isInvertible(matrix))
	{
		printf("Matrix not invertible!\n");
		MyMatrix zero(0);
		return zero;
	}
		
	else
	{
		MyMatrix inverse_matrix(matrix.rowSize, matrix.colSize);
		float d = determinant(matrix);
		
		for (int r = 0; r < inverse_matrix.rowSize; ++r)
		{
			for (int c = 0; c < inverse_matrix.colSize; ++c)
			{
				//Matrix of cofactors
				float cof = cofactor(matrix, r, c);

				//Transpose operation and divide determinant of matrix
				inverse_matrix(c, r) = cof / d;
			}
		}
		return inverse_matrix;
	}
}

//------------------Transform operations------------------
//Translation
MyMatrix translation(int x, int y, int z)
{
	//Create 4x4 Identity matrix
	MyMatrix transform = I_Matrix();

	transform(0, 3) = x;
	transform(1, 3) = y;
	transform(2, 3) = z;

	return transform;
}

//Scaling
MyMatrix scaling(int x, int y, int z)
{
	//Create 4x4 Identity matrix
	MyMatrix transform = I_Matrix();

	transform(0, 0) = x;
	transform(1, 1) = y;
	transform(2, 2) = z;

	return transform;
}
