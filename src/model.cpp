#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "../inc/model.hpp"
#include "../inc/dual.hpp"
#include "../inc/quat.hpp"

namespace Model {
	template<typename R>
	unsigned int model<R>::nextID = 0;

	template<class C> std::ostream& operator<<(std::ostream& lhs,
			std::vector<std::pair<const char*,C>> const& rhs) {
		for(auto p : rhs) {
			if(p.first) {
				std::string fstr(p.first);
				if(fstr.size() > 0) {
					lhs << "\r\n" << fstr << "\r\n";
				}
			}
			lhs << "\t" << p.second << "\r\n";
		}
		return lhs << std::endl;
	}

	int test(void) {
		std::cout << "A" << std::endl;
		dual<float> a(1);
		std::cout << "\r\nB" << std::endl;
		dual<float> b = {1};
		std::cout << "\r\nC" << std::endl;
		dual<float> c = std::move(a);
		std::cout << "\r\n" << std::endl;

		dual<float> extra(c.u, c.v);
		extra.u += extra.u;

		std::vector<dual<float>*> lst = {
			&a, &b, &c, &extra};
		std::cout << "\r\nDump:" << std::endl;
		for(auto d : lst) {std::cout << *d << std::endl;}

		/*float theta = 30 * M_PI/180, 
			  rw = float(cos(theta/2)), 
			  rz = float(sin(theta/2));
		dual<float> u = 1, 
			T(1,0,0,0,0,0.5,0,0), 
			R(rw,0,0,rz,0,0,0,0);

		std::vector<std::pair<const char*,dual<float>>> ds{
			{"u (origin)", u},

			{"T (translates by <1,0,0>)", T},
			{"T u ~T", T(u)},

			{"R (rotates by 30 around <0,0,1>)", R},
			{"R u ~R", R(u)},

			{"R T", R*T},
			{"R (T u ~T) ~R = (R T) u ~(R T)", 
				R(T(u))}, {"", (R*T)(u)},

			{"T R", T*R},
			{"T (R u ~R) ~T = (T R) u ~(R T)", 
				T(R(u))}, {"", (T*R)(u)},
			{"1/(T R)", dual<float>(1)/(T*R)}
		};
		std::cout << ds << std::endl;*/
	}
}
