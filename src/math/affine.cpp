#include "math.hpp"
#include "math/affine.hpp"
#include <iostream>

namespace Math {
	template<typename R> std::ostream&
	operator<<(std::ostream &lhs, const unit<R> &rhs) {
		return lhs << "<" << rhs.x << ", " << rhs.y
			<< ", " << rhs.z << ">";
	}
	template<typename R> std::ostream&
	operator<<(std::ostream &lhs, const ray<R> &rhs) {
		return lhs << rhs.r << " * " << rhs.n;
	}
	template<typename R> std::ostream&
	operator<<(std::ostream& lhs, const rotor<R> &rhs) {
		return lhs << (rhs.theta/M_PI) 
			<< "pi; " << rhs.n;
	}
	template<typename R> std::ostream&
	operator<<(std::ostream& lhs, const point<R> &rhs) {
		return lhs << "{" << rhs.x << ", " << rhs.y
			<< ", " << rhs.z << "}";
	}
	template<typename R> std::ostream&
	operator<<(std::ostream& lhs, const pivot<R> &rhs) {
		return lhs << "Pivot: {" << rhs.rotation << "} "
			"from center <" << rhs.translation << ">";
	}
}
