#ifndef FRAC_H
#define FRAC_H
#include <iostream>

struct frac {
	int numer;
	int denom;
	frac();
	frac(int num, int denom = 1);
	frac(const frac &other);
	frac &operator=(const frac &other);
	frac &operator=(const int &other);
	void simplify();
	frac &operator+=(frac b);
	frac &operator-=(frac b);
	frac &operator*=(frac b);
	bool operator==(const frac &other);
	bool operator==(const int &other);
	bool operator!=(const frac &other);
	bool operator!=(const int &other);
	bool operator>=(const frac &other);
	bool operator>=(const int &other);
	bool operator<=(const frac &other);
	bool operator<=(const int &other);
	bool operator>(const frac &other);
	bool operator>(const int &other);
	bool operator<(const frac &other);
	bool operator<(const int &other);
};

frac operator+(frac a, frac b);
frac operator-(frac a, frac b);
frac operator*(frac a, frac b);
frac operator/(frac a, frac b);
frac operator+(frac a, int b);
frac operator-(frac a, int b);
frac operator*(frac a, int b);
frac operator/(frac a, int b);
frac operator+(int a, frac b);
frac operator-(int a, frac b);
frac operator*(int a, frac b);
frac operator/(int a, frac b);
frac operator-(frac other);
std::ostream &operator<<(std::ostream &out, const frac &a);
std::istream &operator>>(std::istream &in, frac &a);

#endif
