#ifndef MATH_HPP
#define MATH_HPP

#include <math.h>
#include <iostream>

namespace Model {
	template<typename T = double, int E = -6>
	bool near(T u, T v) {
		return std::abs(u-v) <= pow(2.0, E);
	};
}

#include "util.hpp"
#include "model.hpp"

namespace Model {
	template<typename T>
	struct var {};
	template<typename T, T V>
	struct val {};

	
	/*---------------------------------------------------------------------*/

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
