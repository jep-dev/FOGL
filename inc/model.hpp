#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <iostream>
#include <utility>
#include <cmath>

namespace Model {

	template<typename> struct quat;
	template<typename> struct dual;
	
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
			y /= rhs; z /= rhs;
			return *this;
		}
		operator dual<R>(void) const {
			return {*this, 0};
		}
		quat<R>(R w = R(0), R x = R(0), 
				R y = R(0), R z = R(0)):
			w(w), x(x), y(y), z(z) {}
	};

	template<typename R = float>
	struct dual {
		quat<R> u, v;
		dual<R> operator=(dual<R> const& rhs) {
			u = rhs.u; v = rhs.v;
			return *this;
		}
		bool operator==(dual<R> const& rhs) const {
			return u == rhs.u && v == rhs.v;
		}

		dual<R> operator+(dual<R> const& rhs) {
			return {u+rhs.u, v+rhs.v};
		}
		dual<R> operator+=(dual<R> const& rhs) {
			u += rhs.u; v += rhs.v;
			return *this;
		}
		
		dual<R> operator-(void) const {
			return {-u, -v};
		}

		dual<R> operator-(quat<R> const& rhs) const {
			return {u-rhs, v};
		}
		dual<R> operator-=(quat<R> const& rhs) {
			u -= rhs;
			return *this;
		}

		dual<R> operator-(dual<R> const& rhs) const {
			return {u-rhs.u, v-rhs.v};
		}
		dual<R> operator-=(dual<R> const& rhs) {
			u -= rhs.u; v -= rhs.v;
			return *this;
		}

		dual<R> operator*(const R rhs) const {
			return {u*rhs, v*rhs};
		}
		dual<R> operator*=(const R rhs) {
			u *= rhs; v *= rhs;
			return *this;
		}
		dual<R> operator*(quat<R> const& rhs) const {
			return {u*rhs, v*rhs};
		}
		dual<R> operator*=(quat<R> const& rhs) {
			return *this = *this * rhs;
		}
		dual<R> operator*(dual<R> const& rhs) const {
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
		dual<R> operator*=(dual<R> const& rhs) {
			return *this = *this * rhs;
		}
		
		operator R(void) const {
			return sqrt((*this)());
		}
		R operator()(void) const {
			return u()+v();
		}
		dual<R> operator()(quat<R> const& rhs) const {
			return *this * rhs * ~this;
		}
		dual<R> operator()(dual<R> const& rhs) const {
			return *this * rhs * ~*this;
		}

		dual<R> operator~(void) const {
			return {~u,{-v.w,v.x,v.y,v.z}};
		}
		dual<R> operator!(void) const {
			dual<R> conj = ~*this;
			return conj/conj();
		}

		dual<R> operator/(const R rhs) const {
			return {u/rhs, v/rhs};
		}
		dual<R> operator/=(const R rhs) {
			u /= rhs; v /= rhs;
			return *this;
		}
		dual<R> operator/(quat<R> const& rhs) const {
			auto inv = !rhs;
			return {u*inv, v*inv};
		}
		dual<R> operator/=(quat<R> const& rhs) {
			auto inv = !rhs;
			u *= inv; v *= inv;
			return *this;
		}
		dual<R> operator/(dual<R> const& rhs) const {
			return *this * !rhs;
		}
		dual<R> operator/=(dual<R> const& rhs) {
			return *this = *this * !rhs;
		}

		dual(quat<R> const& re, 
				quat<R> const& du = R(0)):
			u(re), v(du) {}
		dual(R uw = R(0), R ux = R(0), R uy = R(0), R uz = R(0),
			R vw = R(0), R vx = R(0), R vy = R(0), R vz = R(0)):
			u(uw, ux, uy, uz), v(vw, vx, vy, vz) {}
	};
	
	template<typename R = float>
	std::ostream& operator<<(std::ostream& lhs, 
			quat<R> const& rhs) {
		quat<R> singlet = rhs.w;
		if(rhs == singlet) {return lhs << rhs.w;}
		return lhs << "[" << rhs.w << ", " << rhs.x 
			<< ", " << rhs.y << ", " << rhs.z << "]";
	}

	template<typename R = float>
	std::ostream& operator<<(std::ostream& lhs, dual<R> const& rhs) {
		return lhs << "{" << rhs.u << ", " << rhs.v << "}";
	}

	int test(void);
}

#endif
