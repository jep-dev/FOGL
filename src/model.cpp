#include "../inc/rijk.hpp"
#include "../inc/dual.hpp"
#include <cmath>

#include <iostream>

namespace Model {
	using dq = dual<rijk<float>>;
	
	void test(void) {

		const char *br = "\r\n", *brt = "\r\n\t";

		//const dq x = {{1,0,0,0}, {0,0,0,0}};
		float theta = M_PI/16, 
			  ctheta = cos(theta/2), 
			  stheta = sin(theta/2);

		std::cout << br << "t = " << theta << ";" << brt <<
 		   "cos(t/2) = " << ctheta << ";" << brt <<
		   "sin(t/2) = " << stheta << br << std::endl;
		
		rijk<float> zero = 0, one = 1;
		/*dq transforms[] = {
			{one, zero},
			{{ctheta, stheta,0,0}, zero},
			{{ctheta, 0,stheta,0}, zero},
			{{ctheta, 0,0,stheta}, zero}
		}, values[] = {
			{zero, {1,1,0,0}}
		};
		int jMax = sizeof(values) / sizeof(values[0]),
			iMax = sizeof(transforms) / sizeof(transforms[0]);
		for(int j = 0; j < jMax; ++j) {
			dq x = values[j];
			std::cout << "x[" << j << "] = " << x << std::endl;
			for(int i = 0; i < iMax; ++i) {
				dq lhs = transforms[i], rhs = ~lhs;
				std::cout << lhs << brt 
					<< (dq(one,zero)/lhs) 
					<< br << std::endl;
				std::cout << ((lhs * x) / lhs) << br << std::endl;
			}
			endl(std::cout);
		}
		
		dual<dual<float>> ddf = {{1,2},{3,4}};
		rijk<dual<float>> rdf = {{1,2},{3,4},{5,6},{7,8}};
		std::cout << "Recursively dual? " << ddf << std::endl;
		std::cout << "Inverted-dual? " << rdf << std::endl;*/
		dq x = {zero, {0, 1, 0, 0}}, t = {{ctheta, 0, 0, stheta}, zero};
		std::cout << "Original X: " << x << br
			<< "Original T: " << t << br << std::endl;
		for(int i = 0; i < 10; ++i) {
			x = t*x*t;
			//x = x/abs(x);
			std::cout << x << std::endl;
		}
		endl(std::cout);

		rijk<float> x2 = {0, 1, 0, 0},
			t2 = {ctheta, 0, 0, stheta};
		std::cout << x2 << " rotated by " << t2 << " is " << brt
			<< (t2*x2*~t2) << br << std::endl;
		std::cout << "Via duals:" << brt
			<< (t*x*~t) << brt << (t*x/t) << std::endl;


	}
}
