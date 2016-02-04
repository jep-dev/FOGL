#ifndef DUAL_HPP
#define DUAL_HPP

#include "../inc/quat.hpp"

namespace Model {

	template<typename> struct quat;

	template<typename R = float>
	struct dual {
		quat<R> u, v;

		/* Unary operators */

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


		/* Constant binary methods */

		/** Distributes equality test */
		bool operator==(dual<R> const& rhs) const;
		/** Apply (lhs * rhs * ~lhs) */
		dual<R> operator()(quat<R> const& rhs) const;
		/** Apply (lhs * rhs * ~lhs) */
		dual<R> operator()(dual<R> const& rhs) const;
		dual<R> operator+(dual<R> const& rhs) const;
		dual<R> operator-(quat<R> const& rhs) const;
		dual<R> operator-(dual<R> const& rhs) const;
		dual<R> operator*(R const& rhs) const;
		dual<R> operator*(quat<R> const& rhs) const;
		dual<R> operator*(dual<R> const& rhs) const;
		dual<R> operator/(R const& rhs) const;
		dual<R> operator/(quat<R> const& rhs) const;
		dual<R> operator/(dual<R> const& rhs) const;

		/* Binary assignment methods */
		
		/*dual<R> operator=(dual<R> const& rhs);*/
		dual<R> operator+=(dual<R> const& rhs);
		dual<R> operator-=(quat<R> const& rhs);
		dual<R> operator-=(dual<R> const& rhs);
		dual<R> operator*=(R const& rhs);
		dual<R> operator*=(quat<R> const& rhs);
		dual<R> operator*=(dual<R> const& rhs);
		dual<R> operator/=(R const& rhs);
		dual<R> operator/=(quat<R> const& rhs);
		dual<R> operator/=(dual<R> const& rhs);

		dual(void) = default;
		dual(dual<R> const&) = default;
		dual(dual<R> &&) = default;

		~dual(void) = default;
		dual<R>& operator=(dual<R> const&) = default;
		dual<R>& operator=(dual<R> &&) = default;
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
		return {~u,{-v.w,v.x,v.y,v.z}};
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
	dual<R> dual<R>::operator+(dual<R> const& rhs) const {
		return {u+rhs.u, v+rhs.v};
	}
	template<typename R>
	dual<R> dual<R>::operator-(quat<R> const& rhs) const {
		return {u-rhs, v};
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
		R auw = u.w, aux = u.x, auy = u.y, auz = u.z,
		  avw = v.w, avx = v.x, avy = v.y, avz = v.z,
		  buw = p.w, bux = p.x, buy = p.y, buz = p.z,
		  bvw = q.w, bvx = q.x, bvy = q.y, bvz = q.z;
		return {{
			auw*buw - aux*bux - auy*buy - auz*buz,
			auw*bux + aux*buw + auy*buz - auz*buy,
			auw*buy + auy*buw + auz*buy - auy*buz,
			auw*buz + auz*buw + aux*buy - auy*bux
		}, {
			avw*buw + avx*bux + avy*buy + avz*buz
			+ auw*bvw - aux*bvx - auy*bvy - auz*bvz,
			auw*bvx + aux*bvw - auy*bvz + auz*bvy
			- avw*bux + avx*buw - avy*buz + avz*buy,
			auw*bvy + aux*bvz + auy*bvw - auz*bvx
			- avw*buy + avx*buz + avy*buw - avz*bux,
			auw*bvz - aux*bvy + auy*bvx + auz*bvw
			- avw*buz - avx*buy + avy*bux + avz*buw
		}};
	}
	template<typename R>
	dual<R> dual<R>::operator()(quat<R> const& rhs) const {
		return *this * rhs * ~*this;
	}
	template<typename R>
	dual<R> dual<R>::operator()(dual<R> const& rhs) const {
		return *this * rhs * ~*this;
	}
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
	dual<R> dual<R>::operator/(dual<R> const& rhs) const {
		return *this * !rhs;
	}

	/*template<typename R>
	dual<R> dual<R>::operator=(dual<R> const& rhs) {
		u = rhs.u; v = rhs.v;
		return *this;
	}*/
	template<typename R>
	dual<R> dual<R>::operator+=(dual<R> const& rhs) {
		u += rhs.u; v += rhs.v;
		return *this;
	}
	template<typename R>
	dual<R> dual<R>::operator-=(quat<R> const& rhs) {
		u -= rhs;
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
	}
	template<typename R>
	dual<R> dual<R>::operator/=(dual<R> const& rhs) {
		return *this = *this * !rhs;
	}

	/*template<typename R>
	dual<R>::dual(quat<R> const& re, quat<R> const& du):
		u(re), v(du) {}*/

	/*template<typename R>
	dual<R>::dual(R uw, R ux, R uy, R uz,
			R vw, R vx, R vy, R vz):
		u(uw, ux, uy, uz), v(vw, vx, vy, vz) {}*/
}

#endif
