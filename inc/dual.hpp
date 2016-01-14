#ifndef DUAL_HPP
#define DUAL_HPP

#include "../inc/rijk.hpp"
#include <iostream>

template<typename R = float>
struct dual {
	R u, v;
	dual(R re = (R)0, R du = (R)0): u(re), v(du) {}
	dual(std::array<R,2> uv): u(uv[0]), v(uv[1]) {}
};

template<typename R = float>
dual<R> operator-(const dual<R>& d) {
	return {-d.u, -d.v};
}
template<typename R = float>
dual<R> operator-(const dual<R>& lhs, const dual<R>& rhs) {
	return {lhs.u-rhs.u, lhs.v-rhs.v};
}

template<typename R = float>
dual<R> operator~(const dual<R>& d) {
	return {~(d.u), -(~d.v)};
}

template<typename R = float>
dual<R> operator*(const dual<R>& lhs, 
		const dual<R>& rhs) {
	R u0 = lhs.u, u1 = rhs.u, 
	  v0 = lhs.v, v1 = rhs.v;
	return {u0*u1, u0*v1 + u1*v0};
}

template<typename R = float>
dual<R> operator/(const dual<R>& lhs, 
		const dual<R>& rhs) {	
	R u0 = lhs.u, u1 = rhs.u, 
	  v0 = lhs.v, v1 = rhs.v;
	return {u0/u1, (v0 - u0*v1/u1)/u1};
}

template<typename R = float>
bool operator==(const dual<R>& lhs, const dual<R>& rhs) {
	return lhs.u == rhs.u && lhs.v == rhs.v;
}
template<typename R = float>
bool operator!=(const dual<R>& lhs, const dual<R>& rhs) {
	return !(lhs == rhs);
}

template<typename R = float>
dual<R> operator+(const dual<R>& lhs, const dual<R>& rhs) {
	return {lhs.u+rhs.u, lhs.v+rhs.v};
}

template<typename R = float>
std::ostream& operator<<(std::ostream& lhs, const dual<R>& rhs) {
	const char *suffix = "\u03B5";
	// R zero = (R) 0;
	R u = rhs.u, v = rhs.v;
	/*if(v == zero)
		return lhs << u;
	if(u == zero)
		return lhs << v << suffix;*/
	return lhs << "(" << u << " + " 
		<< v << suffix << ")";
}


template<typename R = float>
R abs2(const dual<rijk<R>>& dquat) {
	return abs2(dquat.u)+abs2(dquat.v);
}

template<typename R = float>
R abs(const dual<rijk<R>>& dquat) {
	return sqrt(abs2(dquat));
}

template<typename R = float>
dual<rijk<R>> operator*(const dual<rijk<R>>& lhs, const R& rhs) {
	return {lhs.u*rhs, lhs.v*rhs};
}

template<typename R = float>
dual<rijk<R>> operator/(const dual<rijk<R>>& lhs, const R& rhs) {
	return {lhs.u/rhs, lhs.v/rhs};
}

template<typename R = float>
dual<rijk<R>> lerp(const dual<rijk<R>>& from, 
		const dual<rijk<R>>& to, R weight) {
	return from*(1-weight)+to*weight;
}

template<typename R = float>
dual<rijk<R>> nlerp(const dual<rijk<R>>& from,
		const dual<rijk<R>>& to, R weight) {
	auto out = lerp(from, to, weight);
	return out / abs(out);
}

namespace Dual {
	void test(void);
}

#endif
