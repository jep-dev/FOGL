#include "../inc/quat.hpp"
#include <iostream>

namespace Model {
	template<typename R>
	std::ostream& operator<<(std::ostream& lhs,
			quat<R> const& rhs) {
		if(rhs == quat<R>(rhs.w)) {
			return lhs << rhs.w;
		}
		return lhs << rhs.w << ", " << rhs.x << ", " 
			<< rhs.y << ", " << rhs.z;
	}
}
