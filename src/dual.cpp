#include "../inc/dual.hpp"
#include <iostream>

namespace Model {
	template<typename R>
	std::ostream& operator<<(std::ostream& lhs,
			dual<R> const& rhs) {
		return lhs << "{{" << rhs.u << "} + E{"
			<< rhs.v << "}}";
	}
}
