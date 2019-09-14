////////////////////////////////////////////////////////////////////////////////
//
// Matrix.h
// Defines class Matrix
// The Matrix class should provide safe access to 2D arrays
// as well as the elementary Matrix operations +, * and transpose
//

#ifndef DEF_MATRIX
#define DEF_MATRIX

#include <vector>
#include <iostream>

using namespace std;

/////////////////////////////////
//
// Matrix Class
//

class Matrix {
private:
	double _nan = 0.0/0.0;
	vector <vector <double>> array;
	int numRows;
	int numCols;

public:

// Operators
double& operator()(int row, int col);
Matrix operator+ (Matrix const &m) const;
Matrix operator- (Matrix const &m) const;							// Subtraction
Matrix operator* (Matrix const &m) const;							// Multiplies individual elements
Matrix operator* (double const &value);								// Scalar Multiplication
Matrix dot(Matrix const &m)	const;									// Dot Product

Matrix applyFunction(double (*function)(double)) const;				// Applies a function to the whole matrix

Matrix transpose() const;											// Transposes the Matrix

void print(ostream &flux) const;									// Prints the Matrix

// Constructors/Destructor
Matrix();                                                         // Size zero matrix
Matrix(const int rows, const int cols);                           // Uninitialized matrix
Matrix(vector <vector <double>> const &array);                       
~Matrix();
};

ostream& operator<<(ostream &flux, const Matrix &m);		//For overloading the << operator

#endif
