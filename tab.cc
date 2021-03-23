#include "tab.h"
#include <string>
#include <sstream>
#include <climits>

static vector<int> nextbasis(vector<int> &b) {
	int s = (int) b.size();
	int max = b[s-1];
	vector<int> temp = b;
	if (s == max + 1) {
		temp[max] = s;
		return temp;
	}
	temp.erase(temp.begin()+s-1);
	vector<int> &ref = temp;
	temp = nextbasis(ref);
	temp.push_back(max);
	return temp;
}

Simplex::Simplex(Matrix &A, Matrix &b, Matrix &c, bool showSteps, ld val) : A{A}, b{b}, c{c}, bfs{c}, objective{val}, showSteps{showSteps} {
	for (int i = 0; i < A.r; ++i) {
		basis.push_back(i);
	}
}

void Simplex::canonical(vector<int> &bas) {
	basis = bas;
	Matrix Ab = A.basis(basis);
	if (Ab.det() == 0) return;
	Matrix AbInv = Ab.inverse();
	Matrix cB = c.transpose().basis(bas).transpose();
	Matrix y = AbInv.transpose() * cB;
	Matrix yT = y.transpose();
	c = c - A.transpose() * y;
	Matrix yTb = yT * b;
	objective += yTb.m[0][0];
	A = AbInv * A;
	b = AbInv * b;
	Matrix s{c.r,1};
	int count = 0;
	for (auto i : bas) {
		s.m[i][0] = b.m[count][0];
		count++;
	}
	bfs = s;
	feasible = (bfs >= 0);
	cneg = (c <= 0);
}

void Simplex::printState(bool val) {
	std::cout << std::endl;
	Matrix ct = c.transpose();
	std::cout << "max (" << ct << ")^T x + " << objective << std::endl;
	std::cout << "s.t Ax=b with" << std::endl;
	std::cout << "A:" << std::endl << A << std::endl << "b:" << std::endl << b << std::endl;
	std::cout << "x >= 0" << std::endl;
	if (val) {
		std::cout << "Objective value reached = " << objective << std::endl;
		std::cout << "With basic feasible solution:" << std::endl << bfs << std::endl;
		std::cout << "Basis: B = {";
		for (unsigned i = 0; i < basis.size(); ++i) {
			std::cout << basis[i]+1;
			if (i != basis.size()-1) std::cout << ",";
		}
		std::cout << "}" << std::endl;
	}
}

void Simplex::operation() {
	int entering = -1;
	for (int i = 0; i < c.r; ++i) {
		if (c.m[i][0] > 0) {
			entering = i;
			break;
		}
	}
	if (entering == -1) {
		if (feasible == false) {
			for (int i = 0; i < A.r; ++i) {
				if (b.m[i][0] < 0) {
					b.m[i][0] *= -1;
					A.rowOp(-1,1,i,0,1,i);
				}
			}
			canonical(basis);
		}
		throw Exception{"Error!"};
	}
	int pos = -1;
	ld minpos{0};
	bool set = false;
	for (int i = 0; i < A.r; ++i) {
		if (A.m[i][entering] != 0) {
			ld cmp = b.m[i][0] / A.m[i][entering];
			if (set == false && cmp >= 0) {
				minpos = cmp;
				pos = i;
				set = true;
			}
			if (cmp < minpos && cmp >= 0) {
				pos = i;
				minpos = cmp;
			}
		}
	}
	if (pos == -1) throw Exception{"LP is Unbounded!"};
	basis[pos] = entering;
	ld a = A.m[pos][entering];
	A.rowOp(1,a,pos,0,1,pos);
	b.rowOp(1,a,pos,0,1,pos);
	for (int i = 0; i < A.r; ++i) {
		if (i == pos) continue;
		ld x = -A.m[i][entering];
		A.rowOp(1,1,i,x,1,pos);
		b.rowOp(1,1,i,x,1,pos);
	}
	Matrix ct = c.transpose();
	auto arow = A.getRow(pos);
	a = -ct.m[0][entering];
	ct.operation(1,1,0,a,1,arow);
	c = ct.transpose();
	objective -= a * b.m[pos][0];
	Matrix s{c.r,1};
	int count = 0;
	for (auto i : basis) {
		s.m[i][0] = b.m[count][0];
		count++;
	}
	bfs = s;
	feasible = (bfs >= 0);
	cneg = (c <= 0);
}

void Simplex::setBasis(vector<int> basis) {
	this->basis = basis;
}

void Simplex::checkFeasible() {
	Matrix Aaux{A};
	Matrix baux{b};
	Matrix caux{A.c+A.r,1};
	Aaux.c = A.c + A.r;
	vector<int> bas;
	for (int i = A.c; i < A.c + A.r; ++i) {
		caux.m[i][0] = -1;
		for (int j = 0; j < A.r; ++j) {
			if (i - A.c == j) Aaux.m[j].push_back(1);
			else Aaux.m[j].push_back(0);
		}
		bas.push_back(i);
	}
	for (int i = 0; i < A.r; ++i) {
		if (baux.m[i][0] < 0) {
			baux.m[i][0] *= -1;
			for (int j = 0; j < A.c; ++j) {
				Aaux.m[i][j] *= -1;
			}
		}
	}
	Simplex auxilary{Aaux,baux,caux};
	auxilary.printState();
	auxilary.canonical(bas);
	while (auxilary.cneg == false || auxilary.feasible == false) {
		auxilary.operation();
	}
	auxilary.printState();
	if (auxilary.objective != 0) throw Exception{"LP is infeasible!"};
}

void Simplex::solve() {
	if (A.r > A.c) {
		throw Exception{"More constraints than variables!"};
	} else {
		canonical(basis);
		if (showSteps) printState();
		while (cneg == false || feasible == false) {
			operation();
			if (showSteps) printState();
		}
	}
}

// void Simplex::dualsolve() {
// 	Matrix At = A.transpose();
// 	int ac = At.c;
// 	At.c *= 2;
// 	Matrix bd = b;
// 	bd.r *= 2;
// 	for (int i = ac; i < At.c; ++i) {
// 		bd.m.push_back(vector<ld>(1,bd.m[i-ac][0]));
// 		bd.m[i-ac][0] *= -1;
// 		for (int j = 0; j < At.r; ++j) {
// 			At.m[j].push_back(-At.m[j][i-ac]);
// 		}
// 	}
// 	for (int i = At.c; i < At.c + At.r; ++i) {
// 		for (int j = 0; j < At.r; ++i) {
// 			bd.m[j].push_back(0);
// 			if (j == i - At.c) At.m[j].push_back(-1);
// 			else At.m[j].push_back(0);
// 		}
// 	}
// 	At.c += At.r;
// 	bd.r = At.c;
// 	Simplex dual{At,c,bd};
// 	dual.solve();
// 	dual.printState();
// }
