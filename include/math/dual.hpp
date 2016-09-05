#ifndef MATH_DUAL_HPP
#define MATH_DUAL_HPP

#ifndef _E_of
#define _E_of(X) X "E"
//#define _E_of(X) X "\u0190"
#endif

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
				s.w*p.w - s.x*p.x - s.y*p.y - s.z*p.z
					+ r.w*q.w - r.x*q.x - r.y*q.y - r.z*q.z,
				s.x*p.w + s.w*p.x - s.z*p.y + s.y*p.z
					+ r.x*q.w + r.w*q.x - r.z*q.y + r.y*q.z,
				s.y*p.w + s.z*p.x + s.w*p.y - s.x*p.z
					+ r.y*q.w + r.z*q.x + r.w*q.y - r.x*q.z,
				s.z*p.w - s.y*p.x + s.x*p.y + s.w*p.z
					+ r.z*q.w - r.y*q.x + r.x*q.y + r.w*q.z
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
	template<typename R>
	dual<R> dual<R>::operator()(quat<R> const& rhs) const {
		return *this * rhs * ~*this;
	}
	template<typename R>
	dual<R> dual<R>::operator()(dual<R> const& rhs) const {
		return *this * rhs * ~*this;
	}
}

#endif
