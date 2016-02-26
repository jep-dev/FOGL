#ifndef SHADE_HPP
#define SHADE_HPP

#include "view.hpp"
#include "system.hpp"
#include <iostream>

namespace View {
	bool compile(const char*, GLuint&);
	bool link(const char*, const char*, GLuint&);
}

#endif
