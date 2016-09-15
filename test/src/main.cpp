#include "system/printer.hpp"
#include "math.hpp"

#include <iostream>
#include <sstream>

template<typename T>
std::string pad(const T &t) {
	using namespace System;
	return Printer_Base::align(t, 14, Printer_Base::CENTER);
}
template<typename T>
std::string paren(const T &t) {
	return "(" + pad(t) + ")";
}
template<typename T>
std::string transforms(const T &outer, const T &inner) {
	return paren(outer) + "*" + paren(inner) + "*" + paren(~outer)
		+ " = " + pad(outer(inner));
}

int main(int argc, const char **argv) {
	using namespace System;
	using namespace Math;

	quat<float> r0 = {0,0,0,0},
		r1 = {1,0,0,0}, ri = {0,1,0,0},
		rj = {0,0,1,0}, rk = {0,0,0,1};
	dual<float> d10 = {r1, r0}, di0 = {ri, r0},
		dj0 = {rj, r0}, dk0 = {rk, r0},
		d01 = {r0, r1}, d0i = {r0, ri},
		d0j = {r0, rj}, d0k = {r0, rk},
		duals[]{d10, di0, dj0, dk0, d01, d0i, d0j, d0k};

	Printer<10> printer;
	std::string rows[10], cols[8];
	dual<float> cells[64];
	rows[0] = rows[9] = "";

	int row = 0;
	for(auto lhs : duals) {
		std::ostringstream oss;
		oss << lhs;
		rows[row+1] = cols[row] = oss.str();
		int col = 0;
		for(auto rhs : duals) {
			cells[row*8+col] = lhs * rhs;
			col++;
		}
		row++;
	}
	printer.push(&rows[0], &rows[0]+10)
		.push<dual<float>, 8, 8>(cells, &cols[0], &cols[8]).level();
	std::cout << printer << std::endl;

	std::cout << transforms(d10+d0i, d10+d0j) << std::endl;
	std::cout << transforms(dj0, d10+d0i) << std::endl;
	std::cout << transforms(d10+d0j, d10+d0i) << std::endl;

	float src[]{1, 2, 0, 0}, dest[4];
	transpose<2,2>(src, dest);
	printer.clear();
	std::string cols2[]{"",""};
	printer.push<float, 2, 2>(src, &cols2[0], &cols2[0]+2)
		.level().insert(0, "  ").level()
		.push<float, 2, 2>(dest, &cols2[0], &cols2[0]+2);
	std::cout << printer << std::endl;
}
