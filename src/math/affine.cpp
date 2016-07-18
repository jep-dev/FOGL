#include "math.hpp"
#include "math/affine.hpp"
#include <iostream>

namespace Math {
	template<typename R> std::ostream&
	operator<<(std::ostream &lhs, const Unit<R> &rhs) {
		return lhs << "<" << rhs.x << ", " << rhs.y
			<< ", " << rhs.z << ">";
	}
	template<typename R> std::ostream&
	operator<<(std::ostream &lhs, const Ray<R> &rhs) {
		return lhs << rhs.r << " * " << rhs.n;
	}
	template<typename R> std::ostream&
	operator<<(std::ostream& lhs, const Rotor<R> &rhs) {
		return lhs << (rhs.theta/M_PI) 
			<< "pi; " << rhs.n;
	}
	template<typename R> std::ostream&
	operator<<(std::ostream& lhs, const Point<R> &rhs) {
		return lhs << "{" << rhs.x << ", " << rhs.y
			<< ", " << rhs.z << "}";
	}
	template<typename R> std::ostream&
	operator<<(std::ostream& lhs, const Pivot<R> &rhs) {
		return lhs << "Pivot: {" << rhs.rotation << "} "
			"from center <" << rhs.translation << ">";
	}
}
