#ifndef QUAT_HPP
#define QUAT_HPP

#include "../inc/dual.hpp"

namespace Model {

	template<typename R = float>
	struct quat {
		R w, x, y, z;
		quat<R> operator=(quat<R> const& rhs) {
			w = rhs.w; x = rhs.x; y = rhs.y; z = rhs.z;
			return *this;
		}
		bool operator==(quat<R> const& rhs) const {
			return w == rhs.w && x == rhs.x 
				&& y == rhs.y && z == rhs.z;
		}
	
		quat<R> operator+(quat<R> const& rhs) {
			return {w+rhs.w, x+rhs.x, y+rhs.y, z+rhs.z};
		}
		quat<R> operator+=(quat<R> const& rhs) const {
			w += rhs.w; x += rhs.x; y += rhs.y; z += rhs.z;
			return *this;
		}

		quat<R> operator-(void) const {
			return {-w,-x,-y,-z};
		}

		quat<R> operator-(quat<R> const& rhs) const {
			return {w-rhs.w, x-rhs.x, y-rhs.y, z-rhs.z};
		}
		quat<R> operator-=(quat<R> const& rhs) {
			w -= rhs.w; x -= rhs.x; y -= rhs.y; z -= rhs.z;
			return *this;
		}

		quat<R> operator*(quat<R> const& rhs) const {
			R rw = rhs.w, rx = rhs.x, 
			  ry = rhs.y, rz = rhs.z;
			return {
				w*rw - x*rx - y*ry - z*rz,
				w*rx + x*rw + y*rz - z*ry,
				w*ry + y*rw + rz*x - z*rx,
				w*rz + z*rw + x*ry - y*rx};
		}

		quat<R> operator*=(quat<R> const& rhs) {
			return *this = *this * rhs;
		}
		quat<R> operator*(const R rhs) const {
			return {rhs*w, rhs*x, rhs*y, rhs*z};
		}
		quat<R> operator*=(const R rhs) {
			w *= rhs; x *= rhs;
			y *= rhs; z *= rhs;
			return *this;
		}

		quat<R> operator~(void) const {
			return {w,-x,-y,-z};
		}
		
		operator R(void) const {
			return sqrt((*this)());
		}
		R operator()(void) const {
			return w*w + x*x + y*y + z*z;
		}
		quat<R> operator()(quat<R> const& rhs) const {
			return *this * rhs * ~*this;
		}
		dual<R> operator()(dual<R> const& rhs) const {
			return *this * rhs * ~*this;
		}

		quat<R> operator!(void) const {
			return ~(*this)/((*this)());
		}

		quat<R> operator/(quat<R> const& rhs) const {
			return *this * !rhs;
		}
		quat<R> operator/=(quat<R> const& rhs) {
			return *this = *this / rhs;
		}

		quat<R> operator/(const R rhs) const {
			return {w/rhs, x/rhs, y/rhs, z/rhs};
		}

		quat<R> operator/=(const R rhs) {
			w /= rhs; x /= rhs;
		}

		operator dual<R>(void) const {
			return {*this, 0};
		}
		quat<R>(R w = R(0), R x = R(0), 
				R y = R(0), R z = R(0)):
			w(w), x(x), y(y), z(z) {}
	};
}

#endif
