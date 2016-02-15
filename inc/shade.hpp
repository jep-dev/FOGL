#ifndef SHADE_HPP
#define SHADE_HPP

#include "system.hpp"
#include <iostream>
#include <GL/glew.h>

namespace View {
	bool compile(const char *fname, GLuint &shader);
	bool link(const char *vertName, 
			const char *fragName, GLuint &program);
}

#endif
