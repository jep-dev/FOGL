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
		float theta = 30 * M_PI/180, 
			  rw = float(cos(theta/2)), 
			  rz = float(sin(theta/2));
		dual<float> u = {1,{0,0,0.5,0}}, 
			T = {1,{0,0.5}}, 
			R = {{rw,0,0,rz},0};

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
			{"1/(T R)", 1/(T*R)}
		};
		std::cout << ds << std::endl;

		/*model<float> root, a, b;
		root.subs.emplace(a, T);
		root.subs.emplace(b, R);
		std::cout << root << std::endl;*/
	}
}
