#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include <type_traits>

#include "../inc/model.hpp"
#include "../inc/dual.hpp"
#include "../inc/quat.hpp"

namespace Model {
	template<typename R>
	unsigned int model<R>::nextID = 0;
}
