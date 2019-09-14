#include "Matrix.h"
#include <assert.h>
#include <sstream>

using namespace std;

// Operators
double& Matrix::operator()(int i, int j) {
	assert(numRows >= 0 && numCols >= 0);

	if(i < 0 || j < 0)
		return _nan;

	if(i > numRows || j > numCols)
		return _nan;

	return array[i - 1][j - 1];
}  

Matrix Matrix::operator+(Matrix const &m) const {
	
	assert(numRows == m.numRows && numCols == m.numCols);

	Matrix result(numRows, numCols);

	for(int i = 0; i < numRows; i++)	{
		for(int j = 0; j < numCols; j++)
			result.array[i][j] = array[i][j] + m.array[i][j];
	}

	return result;
}

Matrix Matrix::operator-(Matrix const &m) const {
	
	assert(numRows == m.numRows && numCols == m.numCols);

	Matrix result(numRows, numCols);

	for(int i = 0; i < numRows; i++)	{
		for(int j = 0; j < numCols; j++)
			result.array[i][j] = array[i][j] - m.array[i][j];
	}

	return result;
}

Matrix Matrix::operator*(Matrix const &m) const {
	
	assert(numRows == m.numRows && numCols == m.numCols);

	Matrix result(numRows, numCols);

	for(int i = 0; i < numRows; i++)	{
		for(int j = 0; j < numCols; j++)
			result.array[i][j] = array[i][j] * m.array[i][j];
	}

	return result;
}

Matrix Matrix::operator*(double const &value)	{

	Matrix result(numRows, numCols);

	for(int i = 0; i < numRows; i++)	{
		for(int j = 0; j < numCols; j++)
			result.array[i][j] = array[i][j] * value;
	}

	return result;
}

Matrix Matrix::dot(Matrix const &m) const {

	assert(numCols == m.numRows);	

	Matrix result(numRows, m.numCols);

	for(int i = 0; i < numRows; i++)	{
		for(int j = 0; j < m.numCols; j++)	{
			double w = 0;
			for(int k = 0; k < numCols; k++)
				w += array[i][k] * m.array[k][j];
			result.array[i][j] = w;
		}
	}

	return result;
}

Matrix Matrix::applyFunction(double (*function)(double)) const {
	assert(numRows >= 0 && numCols >= 0);
	Matrix result(numRows, numCols);

	for(int i = 0; i < numRows; i++)	{
		for(int j = 0; j < numCols; j++)	
			result.array[i][j] = (*function)(array[i][j]);
	}

	return result;
}

Matrix Matrix::transpose() const {
	assert(numRows >= 0 && numCols >= 0);

	Matrix result(numCols, numRows);

	for(int i = 0; i < numRows; i++)	{
		for(int j = 0; j < numCols; j++)
			result.array[j][i] = array[i][j];
	}

	return result;
}

void Matrix::print(ostream &flux) const {
	int maxLength[numCols] = {};
	stringstream ss;

	for(int i = 0; i < numRows; i++)	{
		for(int j = 0; j < numCols; j++)	{
			ss << array[i][j];
			if(maxLength[j] < ss.str().size())	
				maxLength[j] = ss.str().size();
			ss.str(string());
		}
	}

	for(int i = 0; i < numRows; i++)	{
		for(int j = 0; j < numCols; j++)	{
			flux << array[i][j];
			ss << array[i][j];
			for(int k = 0; k < maxLength[j] - ss.str().size() + 1; k++)
				flux << " ";
			ss.str(string());
		}
		flux << endl;
	}
}

ostream& operator<<(ostream &flux, Matrix const &m)	{
	m.print(flux);
	return flux;
}

// Constructors/Destructor
Matrix::Matrix() {
	numRows = 0;
	numCols = 0;
}


Matrix::Matrix(const int rows, const int cols) {
	this -> numRows = rows;
	this -> numCols = cols;
	this -> array = vector <vector <double>> (numRows, vector <double> (numCols));
}

Matrix::Matrix(vector <vector <double>> const &array)	{
	
	assert(array.size() != 0);
	this -> numRows = array.size();
	this -> numCols = array[0].size();
	this -> array = array;
}

Matrix::~Matrix() {}