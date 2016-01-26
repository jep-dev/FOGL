#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <iostream>
#include <utility>
#include <cmath>

#include "../inc/quat.hpp"
#include "../inc/dual.hpp"

namespace Model {
	
	template<typename R = float>
	std::ostream& operator<<(std::ostream& lhs, 
			quat<R> const& rhs) {
		quat<R> singlet = rhs.w;
		if(rhs == singlet) {return lhs << rhs.w;}
		return lhs << "[" << rhs.w << ", " << rhs.x 
			<< ", " << rhs.y << ", " << rhs.z << "]";
	}

	template<typename R = float>
	std::ostream& operator<<(std::ostream& lhs, dual<R> const& rhs) {
		return lhs << "{" << rhs.u << ", " << rhs.v << "}";
	}

	int test(void);
}

#endif
