#ifndef RIJK_HPP
#define RIJK_HPP

#include <array>
#include <ostream>
#include <ctgmath>

template<typename R = float>
struct rijk {
	R w, x, y, z;
	/*rijk<R>& operator=(rijk<R> rhs) {
		w = rhs.w;
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}*/
	rijk<R>& operator=(rijk<R> const& rhs) {
		w = rhs.w;
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	rijk<R>& operator+=(rijk<R> const& rhs) {
		w += rhs.w;
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	/* TODO - R>S, R<S, R<S>, S<R> decisions */
	template<typename S = int>
	rijk<R>& operator*=(const S& rhs) {
		w *= rhs;
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	rijk(R r = (R)0, R i = (R)0, R j = (R)0, R k = (R)0):
		w(r), x(i), y(j), z(k) {}
	rijk(std::array<R,4> data):
		rijk(data[0], data[1],
		data[2], data[3]) {}
	//rijk(void) : rijk(0, 0, 0, 0) {}
};

template<typename R = float>
R abs2(const rijk<R>& quat) {
	return quat.w*quat.w + quat.x*quat.x
		+ quat.y*quat.y + quat.z*quat.z;
}

template<typename R = float>
R abs(const rijk<R>& quat) {
	return sqrt(abs2(quat));
}

template<typename R = float, typename S = int>
rijk<R> operator*(const rijk<R>& lhs, const S& s_rhs) {
	R const& rhs = static_cast<R>(s_rhs);
	return rijk<R>(lhs.w*rhs, lhs.x*rhs, 
			lhs.y*rhs, lhs.z*rhs);
}

/*template<typename R = float>
rijk<R> operator/(const rijk<R>& lhs, const R& rhs) {
	return rijk<R>(lhs.w/rhs, lhs.x/rhs, 
			lhs.y/rhs, lhs.z/rhs);
}*/

template<typename R = float>
rijk<R> operator+(const rijk<R>& lhs,
		const rijk<R>& rhs) {
	return {lhs.w+rhs.w, lhs.x+rhs.x,
		lhs.y+rhs.y, lhs.z+rhs.z};
}

template<typename R = float>
rijk<R> operator*(const rijk<R>& lhs,
		const rijk<R>& rhs) {
	return rijk<R>(
		lhs.w*rhs.w - lhs.x*rhs.x
			- lhs.y*rhs.y - lhs.z*rhs.z,
		lhs.w*rhs.x + lhs.x * rhs.w
			+ lhs.y*rhs.z - lhs.z*rhs.y,
		lhs.w*rhs.y + lhs.y*rhs.w
			+ lhs.z*rhs.x - lhs.x*rhs.w,
		lhs.w*rhs.z + lhs.z*rhs.w
			+ lhs.x*rhs.y - lhs.y*rhs.x);
}


template<typename R = float>
rijk<R> operator-(const rijk<R>& q) {
	return {-q.w, -q.x, -q.y, -q.z};
}
template<typename R = float>
rijk<R> operator-(const rijk<R>& lhs, const rijk<R>& rhs) {
	return {lhs.w-rhs.w, lhs.x-rhs.x,
		lhs.y-rhs.y, lhs.z-rhs.z};
}

template<typename R = float>
rijk<R> operator~(const rijk<R>& q) {
	return rijk<R>(q.w, -q.x, -q.y, -q.z);
}

template<typename R = float>
rijk<R> operator/(const rijk<R>& lhs, const rijk<R>& rhs) {
	return (lhs*~rhs)/abs2(rhs);
}
template<typename R = float>
rijk<R> operator/(const rijk<R>& lhs, const R& rhs) {
	return {lhs.w/rhs, lhs.x/rhs, lhs.y/rhs, lhs.z/rhs};
}

template<typename R = float>
std::ostream& operator<<(std::ostream& lhs, 
		const rijk<R>& rhs) {
	R zero = (R) 0,
		w = rhs.w + zero, 
		x = rhs.x + zero,
		y = rhs.y + zero,
		z = rhs.z + zero;
	bool w0 = w==zero, x0 = x==zero, 
		 y0 = y==zero, z0 = z==zero;
	
	if(x0 && y0 && z0)
		return lhs << w;

	return lhs << "{" << w << " + " << x 
		<< "i + " << y << "j + " << z << "k}";
}



#endif
