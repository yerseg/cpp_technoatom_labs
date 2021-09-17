#ifndef _MATRIX_H
#define _MATRIX_H

#include <vector>

class MatrixProxy {
  
  public:
	MatrixProxy(std::vector<int> * matrix, size_t columns)
			: matr(matrix),
			  columns(columns) {}
	
	~MatrixProxy() {}
	
	int &operator[](int i) {
		if (i >= columns)
			throw std::out_of_range("Выход за пределы матрицы\n");
		return (*matr)[i];
	}
	
	const int &operator[](int i) const {
		if (i >= columns)
			throw std::out_of_range("Выход за пределы матрицы\n");
		return (*matr)[i];
	}
  
  private:
	std::vector<int> * matr;
	size_t columns;
};

class Matrix {
  
  public:
	Matrix(size_t rows = 0, size_t columns = 0) {
		rowsNumber = rows;
		columnsNumber = columns;
		if (rows != 0 && columns != 0) {
			matrix = new std::vector<int>[rows];
			for (int i = 0; i < rows; i++)
				matrix[i].resize(columns, 0);
		}
	}
	
	~Matrix() {
		if (columnsNumber != 0 && rowsNumber != 0)
			delete[] matrix;
	}
	
	size_t getRows() {
		return rowsNumber;
	}
	
	size_t getColumns() {
		return columnsNumber;
	}
	
	MatrixProxy operator[](int i) {
		if (i >= rowsNumber)
			throw std::out_of_range("Выход за пределы матрицы\n");
		return MatrixProxy(&matrix[i], columnsNumber);
	}
	
	const MatrixProxy operator[](int i) const {
		if (i >= rowsNumber)
			throw std::out_of_range("Выход за пределы матрицы\n");
		return MatrixProxy(&matrix[i], columnsNumber);
	}
	
	bool operator==(const Matrix &other) const {
		if (other.columnsNumber != columnsNumber || other.rowsNumber != rowsNumber)
			return false;
		for (int i = 0; i < rowsNumber; i++)
			for (int j = 0; j < columnsNumber; j++)
				if (other.matrix[i][j] != matrix[i][j])
					return false;
		return true;
	}
	
	bool operator!=(const Matrix &other) const {
		return !(this->operator==(other));
	}
	
	Matrix &operator*=(int num) {
		for (int i = 0; i < rowsNumber; i++)
			for (int j = 0; j < columnsNumber; j++)
				matrix[i][j] *= num;
		return *this;
	}
	
	friend std::ostream &operator<<(std::ostream &s, Matrix &matr) {
		for (int i = 0; i < matr.rowsNumber; i++) {
			for (int j = 0; j < matr.columnsNumber; j++)
				s << matr.matrix[i][j] << "  ";
			s << std::endl;
		}
		return s;
	}
  
  private:
	size_t rowsNumber;
	size_t columnsNumber;
	std::vector<int> * matrix;
};

#endif //_MATRIX_H
