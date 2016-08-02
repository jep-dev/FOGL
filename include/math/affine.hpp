#ifndef MATH_AFFINE_HPP
#define MATH_AFFINE_HPP

#include "math.hpp"
#include "math/quat.hpp"
#include "math/dual.hpp"
#include <cmath>
#include <boost/operators.hpp>

namespace Math {

	template<typename R>
	struct point: boost::operators<point<R>> {
		R x, y, z;
		operator dual<R>(void) const {
			return {{1},{0,x,y,z}};
		}
		point& operator+=(const point<R> &rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		point& operator-=(const point<R> &rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		point& operator*=(const R &rhs) {
			return *this = {x*rhs, y*rhs, z*rhs};
		}
		point& operator*=(R && rhs) {
			return *this = {x*rhs, y*rhs, z*rhs};
		}
		point operator*(R rhs) const {
			return Point(x*rhs, y*rhs, z*rhs);
		}
		point(R x = 0, R y = 0, R z = 0):
			x(x), y(y), z(z) {}
		point(const point &p):
			x(p.x), y(p.y), z(p.z) {}
	};

	template<typename S, typename R>
	point<R> operator*(const S &lhs, const point<R> &rhs) {
		point<R> out(rhs);
		return out *= static_cast<R>(lhs);
	}
	template<typename S, typename R>
	point<R> operator*(S && lhs, const point<R> &rhs) {
		return rhs * static_cast<R>(lhs);
	}

	template<typename R>
	struct unit {
		R x, y, z;
		operator dual<R>(void) const {
			return {1,0,0,0,0, x/2, y/2, z/2};
		}
		unit(R x, R y, R z): x(x), y(y), z(z) {}
		unit(unit<R> const& u): x(u.x), y(u.y), z(u.z) {}
	};

	template<typename R>
	struct ray: virtual dual<R> {
		R r;
		unit<R> n;
		ray(R r, R x, R y, R z):
			r(r), n(x, y, z),
			dual<R>(1, 0, 0, 0, 0,
				r*x/2, r*y/2, r*z/2) {}
		ray(R r, const unit<R> &dir):
			r(r), n(dir),
			dual<R>(1, 0, 0, 0, 0,
				r*n.x/2, r*n.y/2, r*n.z/2) {}
		ray(unit<R> const& dir):
			ray(1, dir) {}
		ray(ray<R> const& rhs):
			ray(rhs.r, rhs.n) {}
	};
	
	template<typename R>
	struct rotor: quat<R> {
		R theta;
		unit<R> n;
		operator dual<R>(void) const {
			return {*this, 0};
		}
		rotor(R theta, R nx, R ny, R nz):
			rotor(theta, unit<R>(nx, ny, nz)) {}
		rotor(R theta, const unit<R>& n):
			theta(theta), n(n),
			quat<R>(cos(theta/2), sin(theta/2)*n.x,
				sin(theta/2)*n.y, sin(theta/2)*n.z) {}
	};

	template<typename R>
	struct pivot: dual<R> {
		ray<R> translation;
		rotor<R> rotation;
		pivot(const ray<R> &offset, const rotor<R> &rotor):
			translation(offset), rotation(rotor),
			dual<R>((~dual<R>(offset))(rotor)) {}
		pivot(ray<R> && offset, rotor<R> && rotor):
			translation(offset), rotation(rotor),
			dual<R> ((~dual<R>(offset))(rotor)) {}
	};
}

#endif
