#include "../inc/dual.hpp"

void Dual::test(void) {
	const char *br = "\r\n", *brt = "\r\n\t";
	dual<float> a{1,1}, b{0,1};
	endl(std::cout 
		<< "a = " << a << br
		<< "b = " << b << brt
		<< "a+b = " << (a+b) << brt
		<< "a*b = " << (a*b) << brt
		<< "a/b = " << (a/b) << brt
		<< "b/a = " << (b/a));
}
