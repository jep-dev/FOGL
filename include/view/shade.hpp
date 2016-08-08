#pragma once
#ifndef VIEW_SHADE_HPP
#define VIEW_SHADE_HPP

#include "view.hpp"
//#include <GL/gl.h>

namespace View {
	bool compile(const char*, GLuint&);
	bool link(const char*, const char*, GLuint&);
}

#endif
