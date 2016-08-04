#ifndef MODEL_HPP
#define MODEL_HPP

#include <GL/gl3w.h>
//#include <GL/gl.h>

namespace Model {
	template<typename T, typename R = GLfloat>
	struct contour_t {
		void operator()(GLfloat *dest, GLclampf u, GLclampf v) {
			dest[0] = T::x(u,v);
			dest[1] = T::y(u,v);
			dest[2] = T::z(u,v);
		}
	};
}

#endif
