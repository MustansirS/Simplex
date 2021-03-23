#include "tab.h"
#include "matrix.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main(int argc, char *argv[]) {
	try {
		int nv, nc;
		int z = 0;
		char ans;
		std::cout << "Enter Number of Variables: ";
		std::cin >> nv;
		std::cout << "Enter Number of Constraints: ";
		std::cin >> nc;
		std::cout << "Enter +z objective value (from max c^T x + z). Enter 0 if z = 0. ";
		std::cin >> z;
		std::cout << "Do you want to see solving steps? y/n ";
		std::cin >> ans;
		bool show = (ans == 'y');
		std::cout << "Set initial basis? y/n ";
		std::cin >> ans;
		std::vector<int> bas;
		if (ans == 'y') {
			int x;
			int count = nc;
			std::cout << "Enter the basis please: ";
			while (count > 0) {
				std::cin >> x;
				bas.push_back(x-1);
				count--;
			}
		}
		std::ifstream inf1{"A.txt"};
		std::ifstream inf2{"b.txt"};
		std::ifstream inf3{"c.txt"};
		Matrix A{nc,nv};
		Matrix b{nc,1};
		Matrix c{nv,1};
		A.readMatrix(inf1);
		b.readMatrix(inf2);
		c.readMatrix(inf3);
		Simplex solver{A,b,c,show,z};
		if (ans == 'y') {
			solver.setBasis(bas);
		}
		solver.printState();
		solver.checkFeasible();
		solver.solve();
		solver.printState();
		std::cout << "Reached Optimality!" << std::endl;
	} catch (Exception &e) {
		std::cerr << e.msg << std::endl;
	}
}
