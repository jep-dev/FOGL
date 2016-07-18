#ifndef MATH_AFFINE_HPP
#define MATH_AFFINE_HPP

#include "math.hpp"
#include "math/quat.hpp"
#include "math/dual.hpp"
#include <cmath>
#include <boost/operators.hpp>

namespace Math {

	template<typename R>
	struct Point:
	boost::additive<Point<R>> {
		R x, y, z;
		operator dual<R>(void) const {
			return {{1},{0,x,y,z}};
		}
		Point& operator+=(const Point<R> &rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		Point& operator-=(const Point<R> &rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		Point& operator*=(const R &rhs) {
			return *this = {x*rhs, y*rhs, z*rhs};
		}
		Point& operator*=(R && rhs) {
			return *this = {x*rhs, y*rhs, z*rhs};
		}
		Point operator*(R rhs) const {
			return Point(x*rhs, y*rhs, z*rhs);
		}
		Point(R x = 0, R y = 0, R z = 0):
			x(x), y(y), z(z) {}
		Point(const Point &p):
			x(p.x), y(p.y), z(p.z) {}
	};

	template<typename S, typename R>
	Point<R> operator*(const S &lhs, const Point<R> &rhs) {
		Point<R> out(rhs);
		return out *= static_cast<R>(lhs);
	}
	template<typename S, typename R>
	Point<R> operator*(S && lhs, const Point<R> &rhs) {
		return rhs * static_cast<R>(lhs);
	}

	template<typename R>
	struct Unit {
		R x, y, z;
		operator dual<R>(void) const {
			return {1,0,0,0,0, x/2, y/2, z/2};
		}
		Unit(R x, R y, R z): x(x), y(y), z(z) {}
		Unit(Unit<R> const& u): x(u.x), y(u.y), z(u.z) {}
	};

	template<typename R>
	struct Ray: public virtual dual<R> {
		R r;
		Unit<R> n;
		Ray(R r, R x, R y, R z):
			r(r), n(x, y, z),
			dual<R>(1, 0, 0, 0, 0,
				r*x/2, r*y/2, r*z/2) {}
		Ray(R r, const Unit<R> &dir):
			r(r), n(dir),
			dual<R>(1, 0, 0, 0, 0,
				r*n.x/2, r*n.y/2, r*n.z/2) {}
		Ray(Unit<R> const& dir):
			Ray(1, dir) {}
		Ray(Ray<R> const& rhs):
			Ray(rhs.r, rhs.n) {}
	};
	
	template<typename R>
	struct Rotor: public quat<R> {
		R theta;
		Unit<R> n;
		operator dual<R>(void) const {
			return {*this, 0};
		}
		Rotor(R theta, R nx, R ny, R nz):
			Rotor(theta, Unit<R>(nx, ny, nz)) {}
		Rotor(R theta, const Unit<R>& n):
			theta(theta), n(n),
			quat<R>(cos(theta/2), sin(theta/2)*n.x,
				sin(theta/2)*n.y, sin(theta/2)*n.z) {}
	};

	template<typename R>
	struct Pivot: public dual<R> {
		Ray<R> translation;
		Rotor<R> rotation;
		Pivot(const Ray<R> &offset, const Rotor<R> &rotor):
			translation(offset), rotation(rotor),
			dual<R>((~dual<R>(offset))(rotor)) {}
		Pivot(Ray<R> && offset, Rotor<R> && rotor):
			translation(offset), rotation(rotor),
			dual<R> ((~dual<R>(offset))(rotor)) {}
	};
}

#endif
