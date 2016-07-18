#ifndef VIEW_SHADE_HPP
#define VIEW_SHADE_HPP

#include "view.hpp"

/*#include <iostream>
#include <GL/gl3w.h>
#include <GL/glcorearb.h>
#include <GLFW/glfw3.h>
#include "GL/gl.h"*/

namespace View {
	bool compile(const char*, GLuint&);
	bool link(const char*, const char*, GLuint&);
}

#endif
