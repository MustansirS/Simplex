#include "matrix.h"

Matrix::Matrix(int r, int c) : r{r}, c{c}, m{vector<vector<ld>>(r,vector<ld>(c,0))} {}

Matrix::Matrix(const Matrix &other) : r{other.r}, c{other.c}, m{other.m} {}

Matrix::Matrix(const Matrix &other, int col) : r{other.r-1}, c{other.c-1}, m{vector<vector<ld>>(other.r-1,vector<ld>(other.c-1,0))} {
	for (int i = 0; i < r; ++i) {
		bool passed = false;
		for (int j = 0; j <= c; ++j) {
			if (j == col) {
				passed = true;
				continue;
			}
			if (passed) {
				m[i][j-1] = other.m[i+1][j];
			} else {
				m[i][j] = other.m[i+1][j];
			}
		}
	}
}

Matrix::Matrix(Matrix &&other) : r{other.r}, c{other.c} {
	m.swap(other.m);
}

Matrix &Matrix::operator=(const Matrix &other) {
	r = other.r;
	c = other.c;
	m = other.m;
	return *this;
}

Matrix &Matrix::operator=(Matrix &&other) {
	r = other.r;
	c = other.c;
	m.swap(other.m);
	return *this;
}

int Matrix::getr() const { return r; }

int Matrix::getc() const { return c; }

vector<ld> Matrix::getRow(int row) const {
	return m[row];
}

vector<ld> Matrix::getCol(int col) const {
	vector<ld> result;
	for (int i = 0; i < r; ++i) {
		result.push_back(m[i][col]);
	}
	return result;
}

ld dot(vector<ld> a, vector<ld> b) {
	unsigned s = a.size();
	if (s != b.size()) throw Exception{"Sizes do not match for dot product!"};
	ld sum{0};
	for (unsigned i = 0; i < s; ++i) {
		sum += a[i]*b[i];
	}
	return sum;
}

Matrix Matrix::transpose() const {
	Matrix a{c,r};
	for (int x = 0; x < r; ++x) {
		for (int y = 0; y < c; ++y) {
			a.m[y][x] = m[x][y];
		}
	}
	return a;
}

Matrix operator*(Matrix lhs, Matrix rhs) {
	if (lhs.c != rhs.r) throw Exception{"Dimensions do not match!"};
	Matrix a{lhs.r,rhs.c};
	for (int r = 0; r < a.r; ++r) {
		for (int c = 0; c < a.c; ++c) {
			a.m[r][c] = dot(lhs.m[r],rhs.getCol(c));
		}
	}
	return a;
}

Matrix &Matrix::operator*=(ld cons) {
	for (int x = 0; x < r; ++x) {
		for (int y = 0; y < c; ++y) {
			m[x][y] *= cons;
		}
	}
	return *this;
}

bool Matrix::operator==(const Matrix &other) {
	if (r != other.r || c != other.c) return false;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			if (m[i][j] != other.m[i][j]) return false;
		}
	}
	return true;
}

bool Matrix::operator>=(const Matrix &other) {
	if (r != other.r || c != other.c) return false;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			if (m[i][j] < other.m[i][j]) return false;
		}
	}
	return true;
}

bool Matrix::operator>=(const int &other) {
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			if (m[i][j] < other) return false;
		}
	}
	return true;
}

bool Matrix::operator<=(const Matrix &other) {
	if (r != other.r || c != other.c) return false;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			if (m[i][j] > other.m[i][j]) return false;
		}
	}
	return true;
}

bool Matrix::operator<=(const int &other) {
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			if (m[i][j] > other) return false;
		}
	}
	return true;
}

Matrix Matrix::basis(vector<int> &b) const {
	unsigned s = b.size();
	Matrix a{r,s};
	for (int x = 0; x < r; ++x) {
		for (int y = 0; y < s; ++y) {
			a.m[x][y] = m[x][b[y]];
		}
	}
	return a;
}

ld Matrix::det() const {
	if (r != c) throw Exception{"Not a square matrix for determinant!"};
	if (r == 1) return m[0][0];
	ld sum{0};
	for (int i = 0; i < c; ++i) {
		Matrix a{*this, i};
		if (i % 2 == 0) sum += m[0][i] * a.det();
		else sum -= m[0][i] * a.det();
	}
	return sum;
}

Matrix operator+(Matrix lhs, Matrix rhs) {
	Matrix temp{lhs};
	int r = lhs.r;
	int c = lhs.c;
	if (r != rhs.r || c != rhs.c) throw "Dimensions do not match for simple operations!";
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			temp.m[i][j] += rhs.m[i][j];
		}
	}
	return temp;
}

Matrix operator-(Matrix lhs, Matrix rhs) {
	Matrix temp{lhs};
	int r = lhs.r;
	int c = lhs.c;
	if (r != rhs.r || c != rhs.c) throw Exception{"Dimensions do not match for simple operations!"};
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			temp.m[i][j] -= rhs.m[i][j];
		}
	}
	return temp;
}

void Matrix::rowOp(ld num1, ld denom1, int r1, ld num2, ld denom2, int r2, bool swap) {
	if (swap) {
		m[r1].swap(m[r2]);
	} else {
		for (int i = 0; i < c; ++i) {
			m[r1][i] = (m[r1][i] * num1) / denom1 + (m[r2][i] * num2) / denom2;
		}
	}
}

void Matrix::operation(ld num1, ld denom1, int r1, ld num2, ld denom2, vector<ld> row) {
	for (int i = 0; i < c; ++i) {
		m[r1][i] = (m[r1][i] * num1) / denom1 + (row[i] * num2) / denom2;
	}
}

Matrix Matrix::inverse() const {
	if (r != c) throw Exception{"Not a square matrix!"};
	Matrix a{*this};
	Matrix I{r,c};
	for (int i = 0; i < r; ++i) {
		I.m[i][i] = 1;
	}
	ld x;
	for (int j = 0; j < c; ++j) {
		if (a.m[j][j] == 0) {
			for (int i = j+1; i < r; ++i) {
				if (a.m[i][j] != 0) {
					a.rowOp(1,1,j,1,1,i,true);
					I.rowOp(1,1,j,1,1,i,true);
				}
			}
		}
		x = a.m[j][j];
		a.rowOp(1,x,j,0,1,j);
		I.rowOp(1,x,j,0,1,j);
		for (int i = 0; i < r; ++i) {
			if (i == j) continue;
			x = a.m[i][j];
			if (x == 0) continue;
			a.rowOp(1,1,i,-x,1,j);
			I.rowOp(1,1,i,-x,1,j);
		}
	}
	return I;
}

vector<RowOperation> Matrix::instructions(int row, int col) const {
	Matrix a{*this};
	vector<RowOperation> v;
	ld x;
	x = a.m[row][col];
	a.rowOp(1,x,row,0,1,row);
	v.emplace_back(1,x,row,0,1,row);
	for (int i = 0; i < r; ++i) {
		if (i == row) continue;
		x = a.m[i][row];
		if (x == 0) continue;
		a.rowOp(1,1,i,-x,1,row);
		v.emplace_back(1,1,i,-x,1,row);
	}
	return v;
}

void Matrix::readMatrix(std::istream &in) {
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			in >> m[i][j];
		}
	}
}

std::ostream &operator<<(std::ostream &out, const Matrix &A) {
	out << std::setprecision(3);
	for (int r = 0; r < A.r; ++r) {
		for (int c = 0; c < A.c; ++c) {
			out << std::setw(5) << A.m[r][c] << " ";
		}
		if (r != A.r - 1) out << std::endl;
	}
	return out;
}
