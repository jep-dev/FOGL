#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <cstdlib>
#include <iosfwd>
#include <sstream>

#include <ratio>

/* TODO 
 * Improve generics; at least fixed- vs. floating-point
 * Replace quat/dual with abstract CT logic applied to RT primitives
 * Integrate util (graph -> expression tree, etc.) */
namespace Math {
	template<typename T, int E>
	constexpr bool near(T A, T B, int _adt1, int _adt2 = 0) {
		return std::ratio_less_equal<std::ratio<B,A>,
			   std::ratio<2,-(E+1)>>::value;
	}
	/*! True if and only if u and v are at most 1/2^e apart
	 * \tparam T The type of each input dimension
	 * \tparam E The threshold, a binary exponent 
	 * \param u The first value to compare
	 * \param v The second value to compare
	 * \param e The runtime precision binary exponent
	 * \return True if and only if u and v are less than the threshold
	 */
	template<typename T = double, int E = -6>
	bool near(T u, T v, int e = E) {
		return (((u-v)*(u-v)) <= pow(2.0,e+1));
	};

	// TODO constexpr near using std::ratio

	/*! A quaternion (real, i, j, k)
	 * @tparam R The type of each dimension */
	template<typename R> struct quat; // --> math/quat

	/*! A dual quaternion (real, dual)
	 * @tparam R The type of each dimension */
	template<typename R> struct dual; // --> math/dual
	
	/*! A Cartesian triplet (i, j, k)
	 * @tparam R The type of each dimension */
	template<typename R> struct point; // --> math/affine

	/*! A Cartesian triplet (i, j, k) normalized to radius 1
	 * @tparam R The type of each dimension */
	template<typename R> struct unit; // --> math/affine

	/*! A Cartesian triplet (i, j, k) representing a directed vector
	 * @tparam R The type of each dimension */
	template<typename R> struct ray; // --> math/affine

	/*! Rotation about an arbitrary axis
	 * @tparam R The type of each dimension */
	template<typename R> struct rotor; // --> math/affine

	/*! Rotation about an offset
	 * @tparam R The type of each dimension */
	template<typename R> struct pivot; // --> math/affine
}

#include "math/quat.hpp"
#include "math/dual.hpp"
#include "math/affine.hpp"

namespace Math {

	template<typename R> std::ostream&
	operator<<(std::ostream& lhs, quat<R> const& rhs) {
		if(rhs == quat<R>(rhs.w)) {
			return lhs << rhs.w;
		}
		return lhs << rhs.w << ", " << rhs.x << ", " 
			<< rhs.y << ", " << rhs.z;
	}

	template<typename R> std::ostream&
	operator<<(std::ostream &lhs, dual<R> const& rhs) {
		static constexpr const char *labels[]{
			"1", "i", "j", "k",
			_E_of(), _E_of("i"),
			_E_of("j"), _E_of("k")
		};
		const quat<R> &u = rhs.u, v = rhs.v;
		const R flat[] {
			u.w, u.x, u.y, u.z, 
			v.w, v.x, v.y, v.z
		};

		bool any = false;
		std::ostringstream oss;
		for(int i = 0; i < 8; ++i) {
			R val(flat[i]), vabs(std::abs(val));
			if(!near<R>(vabs,0)) {
				bool pos = val > 0,
					 unit = near<R>(vabs, 1);
				oss << (any ? pos ? " + " : " - "
					: pos ? "" : "-");
				if(!unit) {
					oss << vabs;
				}
				if(i || unit) {
					oss << labels[i];
				}
				any = true;
			}
		}
		return lhs << (any ? oss.str() : "0");
	}
	
	/*template<typename U, typename V = U>
	struct State {
		U u; V v;
		template<typename R> State<U,V>
		operator*(R r) const {return {u*r, v*r};}
	};

	template<typename U, typename V>
	struct Derivative:
		public State<U,V> {};

	template<typename S, typename T, typename U, typename V>
	struct CompositeState:
		public State<State<S,T>,
			Derivative<U,V>> {};
	
	template<typename T, typename U = T, typename V = T>
	struct Impulse {
		const std::function<T (const State<U,V> &, T)> fn;
	};

	template<typename R, typename S = R,
		typename T = S, typename... IN>
	State<S,T> apply(
			const Impulse<IN...> impulse,
			const State<S,T> &x, const State<S,T> &dx,
			R dt, R t) {
		State<S,T> x2 {x.u + dx.u * dt, x.v + dx.v * dt};
		return {S(x2.v), T(impulse.fn(x2, t+dt))};
	}

	template<typename R, typename S = R,
		typename T = S, typename... IN>
	State<S,T> integrate(
			const Impulse<IN...> &impulse,
			const State<S,T> x, R &t, R dt) {
		State<S,T> base,
			a = apply(impulse, x, base, t, R(0.0)),
			b = apply(impulse, x, a, t, R(dt/2.0)),
			c = apply(impulse, x, b, t, R(dt/2.0)),
			d = apply(impulse, x, c, t, R(dt));
		t += dt;
		S x_t((a.u + (b.u + c.u)*2.0 + d.u)*1.0/6);
		T x_tt((a.v + (b.v + c.v)*2.0 + d.v)*1.0/6);
		return {S(x.u + x_t * dt), T(x.v + x_tt * dt)};
	}
	

	template<typename R, typename S = R,
		typename T = S, typename... IN,
		typename ADD_S = std::function<S (const S&, const S&)>,
		typename ADD_T = std::function<T (const T&, const T&)>,
		typename INC_R = std::function<R& (R&, const R&)>>
	State<S,T> integrate(
			const Impulse<IN...> &impulse, const State<S,T> x,
			const ADD_S &add_s, const ADD_T &add_t,
			const INC_R &inc_r, R &t, R dt) {
		State<S,T> base,
			a = apply(impulse, x, base, t, 0.0),
			b = apply(impulse, x, a, t, dt/2.0),
			c = apply(impulse, x, b, t, dt/2.0),
			d = apply(impulse, x, c, t, dt);
		S x_t(add_s(add_s(b.u, c.u)*(1/3.0),
				add_s(a.u, d.u)*(1/6.0)));
		T x_tt(add_t(add_t(a.v, d.v)*(1/6.0),
				add_t(b.v, c.v)*(1/3.0)));
		inc(t, dt);
		return {add_s(x.u, x_t * dt),
				add_t(x.v, x_tt * dt)};
	}
	template<typename R, typename S = R,
		typename T = S, typename... IN>
	State<S,T> integrate(
			const Impulse<IN...> &impulse, const State<S,T> x,
			const std::function<S(const S&,const S&)> &add_s,
			const std::function<T(const T&,const T&)> &add_t,
			const std::function<R& (R&, const R&)> &inc,
			R &t, R dt) {
		State<S,T> base,
			a = apply(impulse, x, base, t, 0.0),
			b = apply(impulse, x, a, t, dt/2.0),
			c = apply(impulse, x, b, t, dt/2.0),
			d = apply(impulse, x, c, t, dt);
		S x_t(add_s(add_s(b.u, c.u)*(1/3.0),
				add_s(a.u, d.u)*(1/6.0)));
		T x_tt(add_t(add_t(a.v, d.v)*(1/6.0),
				add_t(b.v, c.v)*(1/3.0)));
		inc(t, dt);
		return {add_s(x.u, x_t * dt),
				add_t(x.v, x_tt * dt)};
	}*/

}

#endif
