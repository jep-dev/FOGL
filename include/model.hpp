#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>
#include <stdint.h>

namespace Model {
	
	/** A parametrized surface
	 * @tparam S The implementation type
	 * @tparam T The type of each output dimension
	 * @tparam U The type of the first input dimension
	 * @tparam V The type of the second input dimension
	 */
	/*template<typename S, typename T = GLfloat,
		typename U = GLclampf, typename V = U>
	struct contour {
		**
 		 * A surface with parameters 0 <= u,v < 1
	 	 * @param dest The destination for the output value
	 	 * @param u The value of the first parameter
	 	 * @param v The value of the second parameter
	 	 *
		void operator()(S *dest, U u, V v);
	};*/

 	/// A vector/stack of raw bytes (needs replacement)
	struct Buffer {
		std::vector<uint8_t> data;
		template<typename T>
		void push_back(T const& t) {
			static constexpr int N = sizeof(T);
			auto buf = (uint8_t*) &t;
			data.insert(data.end(), buf, buf+N);
		}
		template<typename S, typename... T>
		void push_back(S const& s, T... t) {
			push_back(s);
			push_back(t...);
		}
	};

}
#include "model/ply.hpp"
#include "model/obj.hpp"
#include "model/mtl.hpp"
#include "model/mesh.hpp"

#endif
