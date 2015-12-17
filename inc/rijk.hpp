#ifndef RIJK_HPP
#define RIJK_HPP

#include <array>
#include <ostream>
#include <ctgmath>

template<std::size_t N, typename R>
struct indexable {
	static const std::size_t size = N;
	R const& operator[](const unsigned int i) const;
	R& operator[](const unsigned int i);
};

template<typename R = float>
struct dual : public indexable<2,R> {
	R u, v;
	dual(R re, R du): 
		u(re), v(du) {}
	dual(std::array<R,2> data):
		u(data[0]), v(data[1]) {}
};

template<typename R = float>
dual<R> operator-(const dual<R>& d) {
	return {-d[0], -d[1]};
}

template<typename R = float>
dual<R> operator*(const dual<R>& lhs, const dual<R>& rhs) {
	R u0 = lhs[0], u1 = rhs[0], 
	  v0 = lhs[1], v1 = rhs[1];
	return {u0*u1, u0*v1 + u1*v0};
}

template<typename R = float>
bool operator==(const dual<R>& lhs, const dual<R>& rhs) {
	return lhs[0] == rhs[0] && lhs[1] == rhs[1];
}
template<typename R = float>
bool operator!=(const dual<R>& lhs, const dual<R>& rhs) {
	return !(lhs == rhs);
}

template<typename R = float>
dual<R> operator+(const dual<R>& lhs, const dual<R>& rhs) {
	return {lhs[0]+rhs[0], lhs[1]+rhs[1]};
}

template<typename R = float>
std::ostream& operator<<(std::ostream& lhs, const dual<R>& rhs) {
	bool lz = rhs[0] == -rhs[0], 
		 dz = rhs[1] == -rhs[1];

	return lhs << "(" << rhs[1] << " + " 
		<< rhs[1] << "\u03B5)";
}

template<typename R = float>
struct rijk {
	R w, x, y, z;
	R const& operator[](const unsigned int i) const {
		switch(i){
			case 1: return x;
			case 2: return y;
			case 3: return z;
			default: return w;
		}
	}
	R& operator[](const unsigned int i) {
		switch(i) {
			case 1: return x;
			case 2: return y;
			case 3: return z;
			default: return w;
		}
	}
	rijk<R>& operator=(rijk<R> rhs) {
		w = rhs.w;
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	rijk<R>& operator=(rijk<R> const& rhs) const {
		w = rhs.w;
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	/*rijk<R>& operator+=(rijk<R> rhs) {
		w += rhs.w;
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}*/
	
	rijk<R>& operator+=(rijk<R> const& rhs) const {
		w += rhs.w;
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	template<typename S = int>
	rijk<R>& operator*=(const S& s_rhs) {
		R const& rhs = static_cast<R>(s_rhs);
		w *= rhs;
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	rijk(R r, R i, R j, R k):
		w(r), x(i), y(j), z(k) {}
	rijk(std::array<R,4> data):
		rijk(data[0], data[1],
		data[2], data[3]) {}
	rijk(void) : rijk(0, 0, 0, 0) {}
};

/*template<typename R = int>
struct zero {
	operator rijk<R>() const {
		return {0, 0, 0, 0};
	}
	operator dual<R>() const {
		return {0, 0};
	}
};*/

template<typename R = float>
R abs2(const rijk<R>& quat) {
	return quat[0]*quat[0] + quat[1]*quat[1]
		+quat[2]*quat[2]+quat[3]*quat[3];
}

template<typename R = float>
R abs(const rijk<R>& quat) {
	return sqrt(abs2(quat));
}

template<typename R = float, typename S = int>
rijk<R> operator*(const rijk<R>& lhs, const S& s_rhs) {
	R const& rhs = static_cast<R>(s_rhs);
	return rijk<R>(lhs[0]*rhs, lhs[1]*rhs, 
			lhs[2]*rhs, lhs[3]*rhs);
}

template<typename R = int>
rijk<R> operator/(const rijk<R>& lhs, const R& rhs) {
	return rijk<R>(lhs[0]/rhs, lhs[1]/rhs, 
			lhs[2]/rhs, lhs[3]/rhs);
}

template<typename R = int>
rijk<R> operator+(const rijk<R>& lhs,
		const rijk<R>& rhs) {
	return {lhs.w+rhs.w, lhs.x+rhs.x,
		lhs.y+rhs.y, lhs.z+rhs.z};
}

template<typename R = int>
rijk<R> operator*(const rijk<R>& lhs,
		const rijk<R>& rhs) {
	return {lhs.w*rhs.w - lhs.x*rhs.x
			- lhs.y*rhs.y - lhs.z*rhs.z,
		lhs.w*rhs.x + lhs.x * rhs.w
			+ lhs.y*rhs.z - lhs.z*rhs.y,
		lhs.w*rhs.y + lhs.y*rhs.w
			+ lhs.z*rhs.x - lhs.x*rhs.w,
		lhs.w*rhs.z + lhs.z*rhs.w
			+ lhs.x*rhs.y - lhs.y*rhs.x};
}

template<typename R = int>
rijk<R> operator/(const rijk<R>& lhs, const rijk<R>& rhs) {
	return (lhs*~rhs)/abs2(rhs);
}

template<typename R = int>
rijk<R> operator-(const rijk<R>& q) {
	return {-q[0], -q[1], -q[2], -q[3]};
}

template<typename R = int>
rijk<R> operator~(const rijk<R>& q) {
	return {q[0], -q[1], -q[2], -q[3]};
}

template<typename R = int>
std::ostream& operator<<(std::ostream& lhs, 
		const rijk<R>& rhs) {
	return lhs << "{" << rhs[0] << " + "
		<< rhs[1] << "i + " << rhs[2] << "j + "
		<< rhs[3] << "k}";
}

#endif
