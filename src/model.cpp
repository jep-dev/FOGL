#include "model.hpp"

namespace Model {
	template<typename S, typename T, typename U, typename V>
	void Model::contour<S,T,U,V>::operator()(S *dest, U u, V v) {
		dest[0] = u;
		dest[1] = v;
		dest[2] = 0;
	}
}
