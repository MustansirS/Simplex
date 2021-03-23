#include "frac.h"
#include "exception.h"

static int gcd(int b, int a) {
	if (a < 0) return gcd(b,-a);
	if (b < 0) return gcd(-b,a);
	if (a == 0) return b;
	if (a > b) return gcd(a,b);
	return gcd(a,b%a);
}

frac::frac() : numer{0}, denom{1} {}

frac::frac(int num, int denom) : numer{num}, denom{denom} {}

frac::frac(const frac &other) : numer{other.numer}, denom{other.denom} {}

frac &frac::operator=(const frac &other) {
	numer = other.numer;
	denom = other.denom;
	return *this;
}

frac &frac::operator=(const int &other) {
	numer = other;
	denom = 1;
	return *this;
}

void frac::simplify() {
	int g = gcd(numer,denom);
	numer /= g;
	denom /= g;
	if (denom == 0) throw Exception{"Divide by 0 error!"};
	if (denom < 0) {
		denom = -denom;
		numer = -numer;
	}
}

frac &frac::operator+=(frac b) {
	frac temp = *this + b;
	numer = temp.numer;
	denom = temp.denom;
	return *this;
}

frac &frac::operator-=(frac b) {
	frac temp = *this - b;
	numer = temp.numer;
	denom = temp.denom;
	return *this;
}

frac &frac::operator*=(frac b) {
	frac temp{*this};
	temp = temp * b;
	numer = temp.numer;
	denom = temp.denom;
	return *this;
}

bool frac::operator==(const frac &other) {
	return (numer == other.numer && denom == other.denom);
}

bool frac::operator==(const int &other) {
	frac f{other};
	return (*this == f);
}

bool frac::operator!=(const frac &other) {
	return !(*this == other);
}

bool frac::operator!=(const int &other) {
	frac f{other};
	return !(*this == f);
}

bool frac::operator>=(const frac &other) {
	int g = gcd(denom,other.denom);
	int tm = other.denom / g;
	int om = denom / g;
	int nt = tm * numer;
	int no = om * other.numer;
	return (nt >= no);
}

bool frac::operator>=(const int &other) {
	int no = denom * other;
	return (numer >= no);
}

bool frac::operator<=(const frac &other) {
	int g = gcd(denom,other.denom);
	int tm = other.denom / g;
	int om = denom / g;
	int nt = tm * numer;
	int no = om * other.numer;
	return (nt <= no);
}

bool frac::operator<=(const int &other) {
	int no = denom * other;
	return (numer <= no);
}

bool frac::operator>(const frac &other) {
	int g = gcd(denom,other.denom);
	int tm = other.denom / g;
	int om = denom / g;
	int nt = tm * numer;
	int no = om * other.numer;
	return (nt > no);
}

bool frac::operator>(const int &other) {
	int no = denom * other;
	return (numer > no);
}

bool frac::operator<(const frac &other) {
	int g = gcd(denom,other.denom);
	int tm = other.denom / g;
	int om = denom / g;
	int nt = tm * numer;
	int no = om * other.numer;
	return (nt < no);
}

bool frac::operator<(const int &other) {
	int no = denom * other;
	return (numer < no);
}

frac operator+(frac a, frac b) {
	frac f{a.numer*b.denom+b.numer*a.denom,a.denom*b.denom};
	f.simplify();
	return f;
}

frac operator-(frac a, frac b) {
	frac f{a.numer*b.denom-b.numer*a.denom,a.denom*b.denom};
	f.simplify();
	return f;
}

frac operator*(frac a, frac b) {
	frac f{a.numer*b.numer,a.denom*b.denom};
	f.simplify();
	return f;
}

frac operator/(frac a, frac b) {
	frac f{a.numer*b.denom,a.denom*b.numer};
	f.simplify();
	return f;
}

frac operator+(frac a, int b) {
	frac temp{b};
	return a + temp;
}

frac operator-(frac a, int b) {
	frac temp{b};
	return a - temp;
}

frac operator*(frac a, int b) {
	frac temp{b};
	return a * temp;
}

frac operator/(frac a, int b) {
	frac temp{b};
	return a / temp;
}

frac operator+(int a, frac b) {
	return b + a;
}

frac operator-(int a, frac b) {
	frac temp{a};
	return temp - b;
}

frac operator*(int a, frac b) {
	return b * a;
}

frac operator/(int a, frac b) {
	frac temp{b.denom,b.numer};
	return a * temp;
}

frac operator-(frac other) {
	return 0 - other;
}

std::ostream &operator<<(std::ostream &out, const frac &a) {
	out << a.numer;
	if (a.denom != 1) out << '/' << a.denom;
	return out;
}

std::istream &operator>>(std::istream &in, frac &a) {
	in >> a.numer;
	return in;
}
