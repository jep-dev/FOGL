#pragma once
#ifndef MODEL_HPP
#define MODEL_HPP

#include <GL/gl3w.h>

namespace Model {
	
	/** A parametrized surface
	 * @tparam S The implementation type
	 * @tparam T The type of each output dimension
	 * @tparam U The type of the first input dimension
	 * @tparam V The type of the second input dimension
	 */
	template<typename S, typename T = GLfloat,
		typename U = GLclampf, typename V = U>
	struct contour {
		/**
 		 * A surface with parameters 0 <= u,v < 1
	 	 * @param dest The destination for the output value
	 	 * @param u The value of the first parameter
	 	 * @param v The value of the second parameter
	 	 */
		void operator()(S *dest, U u, V v);
	};
}

#endif
