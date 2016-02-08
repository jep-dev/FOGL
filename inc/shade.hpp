#ifndef SHADE_HPP
#define SHADE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>

namespace View {
	void read(const char *fname, std::string &lines);
	bool compile(const char *fname, GLuint &shader);
	bool link(const char *vertName, 
			const char *fragName, GLuint &program);
}

#endif
