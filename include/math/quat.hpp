#ifndef MATH_QUAT_HPP
#define MATH_QUAT_HPP

#include "math.hpp"

namespace Math {

	template<typename R = float>
	struct quat {
		R w, x, y, z;
		
		/* Unary operators */

		/** Multiplicative inverse */
		quat<R> operator!(void) const;
		/** Additive inverse */
		quat<R> operator-(void) const;
		/** Conjugate */
		quat<R> operator~(void) const;
		/** Cast operator, as Euclidean norm */
		explicit operator R(void) const;
		/** Squared Euclidean norm */
		R operator()(void) const;
		/** Simple promotion */
		explicit operator dual<R>(void) const;

		/* Constant binary methods (where lhs = *this) */

		/** Distributes equality test */
		bool operator==(quat<R> const& rhs) const;
		/** Apply (lhs * rhs * ~lhs) */
		quat<R> operator()(quat<R> const& rhs) const;
		/** Apply (lhs * rhs * ~lhs) */
		dual<R> operator()(dual<R> const& rhs) const;
		
		quat<R> operator+(quat<R> const& rhs) const;
		quat<R> operator-(quat<R> const& rhs) const;
		quat<R> operator*(quat<R> const& rhs) const;
		quat<R> operator/(quat<R> const& rhs) const;

		quat<R> operator*(R const& rhs) const;
		quat<R> operator/(R const& rhs) const;

		/* Binary operators */

 		//quat<R> operator=(quat<R> const& rhs);
		quat<R>& operator+=(quat<R> const& rhs);
		quat<R>& operator-=(quat<R> const& rhs);
		quat<R>& operator*=(R const& rhs);
		quat<R>& operator*=(quat<R> const& rhs);
		quat<R>& operator/=(R const& rhs);
		quat<R>& operator/=(quat<R> const& rhs);

		quat(void) = default;
		quat(const R w, const R x = 0, const R y = 0, const R z = 0):
			w(w), x(x), y(y), z(z) {}
	};
	template<typename R>
	quat<R> quat<R>::operator-(void) const {
		return {-w,-x,-y,-z};
	}
	template<typename R>
	quat<R> quat<R>::operator!(void) const {
		return ~(*this)/((*this)());
	}
	template<typename R>
	quat<R> quat<R>::operator~(void) const {
		return {w,-x,-y,-z};
	}
	template<typename R>
	quat<R>::operator R(void) const {
		return sqrt((*this)());
	}
	template<typename R>
	R quat<R>::operator()(void) const {
		return w*w + x*x + y*y + z*z;
	}
	template<typename R>
	quat<R>::operator dual<R>(void) const {
		return {*this, 0};
	}

	template<typename R>
	bool quat<R>::operator==(quat<R> const& rhs) const {
		return w == rhs.w && x == rhs.x 
			&& y == rhs.y && z == rhs.z;
	}
	template<typename R>
	quat<R> quat<R>::operator()(quat<R> const& rhs) const {
		return *this * rhs * ~*this;
	}
	template<typename R>
	quat<R> quat<R>::operator+(quat<R> const& rhs) const {
		return {w+rhs.w, x+rhs.x, y+rhs.y, z+rhs.z};
	}
	template<typename R>
	quat<R> quat<R>::operator-(quat<R> const& rhs) const {
		return {w-rhs.w, x-rhs.x, y-rhs.y, z-rhs.z};
	}
	template<typename R>
	quat<R> quat<R>::operator*(R const& rhs) const {
		return {rhs*w, rhs*x, rhs*y, rhs*z};
	}
	template<typename R>
	quat<R> quat<R>::operator*(quat<R> const& rhs) const {
		R rw = rhs.w, rx = rhs.x, 
			ry = rhs.y, rz = rhs.z;
		return {
			w*rw - x*rx - y*ry - z*rz,
			w*rx + x*rw + y*rz - z*ry,
			w*ry + y*rw + rz*x - z*rx,
			w*rz + z*rw + x*ry - y*rx};
	}
	template<typename R>
	quat<R> quat<R>::operator/(R const& rhs) const {
		return {w/rhs, x/rhs, y/rhs, z/rhs};
	}
	template<typename R>
	quat<R> quat<R>::operator/(quat<R> const& rhs) const {
		return *this * !rhs;
	}

	template<typename R>
	quat<R>& quat<R>::operator+=(quat<R> const& rhs) {
		w += rhs.w; x += rhs.x; y += rhs.y; z += rhs.z;
		return *this;
	}
	template<typename R>
	quat<R>& quat<R>::operator-=(quat<R> const& rhs) {
		w -= rhs.w; x -= rhs.x; y -= rhs.y; z -= rhs.z;
		return *this;
	}
	template<typename R>
	quat<R>& quat<R>::operator*=(R const& rhs) {
		w *= rhs; x *= rhs;
		y *= rhs; z *= rhs;
		return *this;
	}
	template<typename R>
	quat<R>& quat<R>::operator*=(quat<R> const& rhs) {
		return *this = *this * rhs;
	}
	template<typename R>
	quat<R>& quat<R>::operator/=(R const& rhs) {
		w /= rhs; x /= rhs;
		y /= rhs; z /= rhs;
		return *this;
	}
	template<typename R>
	quat<R>& quat<R>::operator/=(quat<R> const& rhs) {
		return *this = *this / rhs;
	}
}

#endif
