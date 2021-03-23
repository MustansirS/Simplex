#ifndef TAB_H
#define TAB_H
#include "matrix.h"

class Simplex {
	Matrix A;
	Matrix b;
	Matrix c;
	ld objective;
	vector<int> basis;
	Matrix bfs;
	bool feasible = false;
	bool cneg = false;
	void canonical(vector<int> &bas);
	void operation();
	bool showSteps = false;
public:
	Simplex(Matrix &, Matrix &b, Matrix &c, bool showSteps = false, ld val = 0);
	void printState(bool val = true);
	void setBasis(vector<int> basis);
	void checkFeasible();
	void solve();
	// void dualsolve();
};

#endif
