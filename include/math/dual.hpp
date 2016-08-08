#pragma once
#ifndef MATH_DUAL_HPP
#define MATH_DUAL_HPP

#ifndef _E_of
#define _E_of(X) X "\u0190"
#endif

#include "math.hpp"

#include <boost/operators.hpp>

namespace Math {
	template<typename>
		struct quat;

	template<typename R = float>
	struct dual: public boost::operators<dual<R>> {
		quat<R> u, v;

		/** Additive inverse */
		dual<R> operator-(void) const;
		/** Multiplicative inverse */
		dual<R> operator!(void) const;
		/** Distributes conjugation to members */
		dual<R> operator~(void) const;
		/** Cast operator, as Euclidean norm */
		explicit operator R(void) const;
		/** Squared Euclidean norm */
		R operator()(void) const;
		/** Distributes equality test */
		bool operator==(dual<R> const &rhs) const;
		/** Apply (lhs * rhs * ~lhs) */
		dual<R> operator()(quat<R> const &rhs) const;
		dual<R> operator()(dual<R> const &rhs) const;

		dual<R>& operator=(quat<R> const &rhs) {
			u = rhs; v = 0;
			return *this;
		}
		dual<R>& operator=(dual<R> const &rhs) = default;
		dual<R>& operator+=(R const &rhs) {
			u.w += rhs;
			return *this;
		}
		dual<R>& operator+=(dual<R> const &rhs) {
			u += rhs.u;
			v += rhs.v;
			return *this;
		}
		dual<R>& operator-=(dual<R> const &rhs) {
			u -= rhs.u;
			v -= rhs.v;
			return *this;
		}
		dual<R>& operator*=(dual<R> const &rhs) {
			auto const& p = rhs.u, q = rhs.v;
 			auto const r = u, s = v;
			return *this = {
				r.w*p.w - r.x*p.x - r.y*p.y - r.z*p.z,
				r.w*p.x + r.x*p.w + r.y*p.z - r.z*p.y,
				r.w*p.y - r.x*p.z + r.y*p.w + r.z*p.x,
				r.w*p.z + r.x*p.y - r.y*p.x + r.z*p.w,
				r.w*q.w - r.x*q.x - r.y*q.y - r.z*q.z
					+ s.w*p.w - s.x*p.x - s.y*p.y - s.z*p.z,
				r.w*q.x + r.x*q.w + r.y*q.z - r.z*q.y
					+ s.w*p.x + s.x*p.w + s.y*p.z - s.z*p.y,
				r.w*q.y - r.x*q.z + r.y*q.w + r.z*q.x
					+ s.w*p.y - s.x*p.z + s.y*p.w + s.z*p.x,
				r.w*q.z + r.x*q.y - r.y*q.x + r.z*q.w
					+ s.w*p.z + s.x*p.y - s.y*p.x + s.z*p.w
			};
		}
		dual<R>& operator*=(quat<R> const &rhs) {
			u *= rhs;
			v *= rhs;
			return *this;
		}
		dual<R>& operator*=(R const &rhs) {
			u *= rhs;
			v *= rhs;
			return *this;
		}
		dual<R>& operator/=(R const &rhs) {
			u /= rhs;
			v /= rhs;
			return *this;
		}

		dual(void) = default;
		dual(dual<R> const&) = default;
		dual(dual<R> &&) = default;
		dual(R uw, R ux = 0, R uy = 0, R uz = 0,
				R vw = 0, R vx = 0, R vy = 0, R vz = 0):
			u(uw, ux, uy, uz), v(vw, vx, vy, vz) {}
		dual(const quat<R> &u, const quat<R> v = {0}):
			u(u), v(v) {}
	};
	
	template<typename R>
	dual<R> dual<R>::operator-(void) const {
		return {-u, -v};
	}
	template<typename R>
	dual<R> dual<R>::operator!(void) const {
		dual<R> conj = ~*this;
		return conj/conj();
	}
	template<typename R>
	dual<R> dual<R>::operator~(void) const {
		return {{u.w,-u.x,-u.y,-u.z},
			{-v.w, v.x, v.y, v.z}};
	}
	template<typename R>
	dual<R>::operator R(void) const {
		return sqrt((*this)());
	}
	template<typename R>
	R dual<R>::operator()(void) const {
		return u()+v();
	}

	template<typename R>
	bool dual<R>::operator==(dual<R> const& rhs) const {
		return u == rhs.u && v == rhs.v;
	}
	/*
	template<typename R>
	dual<R> dual<R>::operator+(dual<R> const& rhs) const {
		return {u+rhs.u, v+rhs.v};
	}
	template<typename R>
	dual<R> dual<R>::operator-(dual<R> const& rhs) const {
		return {u-rhs.u, v-rhs.v};
	}
	template<typename R>
	dual<R> dual<R>::operator*(R const& rhs) const {
		return {u*rhs, v*rhs};
	}
	template<typename R>
	dual<R> dual<R>::operator*(quat<R> const& rhs) const {
		return {u*rhs, v*rhs};
	}
	template<typename R>
	dual<R> dual<R>::operator*(dual<R> const& rhs) const {
		auto const& p = rhs.u, q = rhs.v;
		return {{
			u.w*p.w - u.x*p.x - u.y*p.y - u.z*p.z,
			u.w*p.x + u.x*p.w + u.y*p.z - u.z*p.y,
			u.w*p.y - u.x*p.z + u.y*p.w + u.z*p.x,
			u.w*p.z + u.x*p.y - u.y*p.x + u.z*p.w
		}, {
			u.w*q.w - u.x*q.x - u.y*q.y - u.z*q.z
			+ v.w*p.w - v.x*p.x - v.y*p.y - v.z*p.z,
			u.w*q.x + u.x*q.w + u.y*q.z - u.z*q.y
			+ v.w*p.x + v.x*p.w + v.y*p.z - v.z*p.y,
			u.w*q.y - u.x*q.z + u.y*q.w + u.z*q.x
			+ v.w*p.y - v.x*p.z + v.y*p.w + v.z*p.x,
			u.w*q.z + u.x*q.y - u.y*q.x + u.z*q.w
			+ v.w*p.z + v.x*p.y - v.y*p.x + v.z*p.w
		}};
	}*/
	template<typename R>
	dual<R> dual<R>::operator()(quat<R> const& rhs) const {
		return *this * rhs * ~*this;
	}
	template<typename R>
	dual<R> dual<R>::operator()(dual<R> const& rhs) const {
		return *this * rhs * ~*this;
	}
	/*
	template<typename R>
	dual<R> dual<R>::operator/(R const& rhs) const {
		return {u/rhs, v/rhs};
	}
	template<typename R>
	dual<R> dual<R>::operator/(quat<R> const& rhs) const {
		auto inv = !rhs;
		return {u*inv, v*inv};
	}
	
	template<typename R>
	dual<R> dual<R>::operator+=(dual<R> const& rhs) {
		u += rhs.u; v += rhs.v;
		return *this;
	}
	template<typename R>
	dual<R> dual<R>::operator-=(dual<R> const& rhs) {
		u -= rhs.u; v -= rhs.v;
		return *this;
	}
	template<typename R>
	dual<R> dual<R>::operator*=(R const& rhs) {
		u *= rhs; v *= rhs;
		return *this;
	}
	template<typename R>
	dual<R> dual<R>::operator*=(quat<R> const& rhs) {
		return *this = *this * rhs;
	}
	template<typename R>
	dual<R> dual<R>::operator*=(dual<R> const& rhs) {
		return *this = *this * rhs;
	}
	template<typename R>
	dual<R> dual<R>::operator/=(R const& rhs) {
		u /= rhs; v /= rhs;
		return *this;
	}
	template<typename R>
	dual<R> dual<R>::operator/=(quat<R> const& rhs) {
		auto inv = !rhs;
		u *= inv; v *= inv;
		return *this;
	}*/
}

#endif
