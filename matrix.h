#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "frac.h"
#include "exception.h"
using std::vector;

#ifdef decimal
typedef long double ld;
#else
typedef frac ld;
#endif

class Simplex;

struct RowOperation {
	ld num1, denom1, num2, denom2;
	int r1, r2;
	bool swap;
	RowOperation(ld num1, ld denom1, int r1, ld num2, ld denom2, int r2, bool swap = false) : 
	num1{num1}, denom1{denom1}, r1{r1}, num2{num2}, denom2{denom2}, r2{r2}, swap{swap} {}
};

class Matrix {
	vector<vector<ld>> m;
	int r;
	int c;
	vector<ld> getRow(int row) const;
	vector<ld> getCol(int col) const;
public:
	Matrix(int r, int c);
	Matrix(const Matrix &other);
	Matrix(const Matrix &other, int col);
	Matrix(Matrix &&other);
	Matrix &operator=(const Matrix &other);
	Matrix &operator=(Matrix &&other);
	int getr() const;
	int getc() const;
	Matrix transpose() const;
	friend Matrix operator*(Matrix lhs, Matrix rhs);
	Matrix &operator*=(ld cons);
	bool operator==(const Matrix &other);
	bool operator>=(const Matrix &other);
	bool operator>=(const int &other);
	bool operator<=(const Matrix &other);
	bool operator<=(const int &other);
	Matrix basis(vector<int> &b) const;
	ld det() const;
	friend Matrix operator+(Matrix lhs, Matrix rhs);
	friend Matrix operator-(Matrix lhs, Matrix rhs);
	void rowOp(ld num1, ld denom1, int r1, ld num2, ld denom2, int r2, bool swap = false);
	void operation(ld num1, ld denom1, int r1, ld num2, ld denom2, vector<ld> row);
	Matrix inverse() const;
	vector<RowOperation> instructions(int row, int col) const;
	friend std::ostream &operator<<(std::ostream &out, const Matrix &A);
	void readMatrix(std::istream &in = std::cin);
	friend class Simplex;
};

ld dot(vector<ld> a, vector<ld> b);
Matrix operator*(Matrix lhs, Matrix rhs);
Matrix operator+(Matrix lhs, Matrix rhs);
Matrix operator-(Matrix lhs, Matrix rhs);
std::ostream &operator<<(std::ostream &out, const Matrix &A);

#endif
