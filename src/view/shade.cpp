#include "system.hpp"
#include "system/printer.hpp"

#include "view.hpp"
#include "view/shade.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>

namespace View {
	bool compile(const char *fname, GLuint &shader,
			std::vector<std::string> &errors) {
		int len;
		GLint status = GL_FALSE;
		
		std::ifstream file(fname);
		std::string lines((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
		file.close();

		const GLchar *cs = lines.c_str();
		glShaderSource(shader, 1, &cs, NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		if(len > 1) {
			char msg[len+1];
			msg[len] = '\0';
			glGetShaderInfoLog(shader, len, NULL, msg);
			errors.emplace_back((const char *)msg);
			return false;
		}
		return true;
	}

	bool link(const char *vertName, const char *fragName,
			GLuint &program, std::vector<std::string> &errors) {
		bool success = false;
		GLint len, status;
		GLuint vert = glCreateShader(GL_VERTEX_SHADER),
			frag = glCreateShader(GL_FRAGMENT_SHADER);
		if((success = compile(vertName, vert, errors))) {
			if((success = compile(fragName, frag, errors))) {
				glAttachShader(program, vert);
				glAttachShader(program, frag);
				glLinkProgram(program);
				glGetProgramiv(program, 
						GL_LINK_STATUS, &status);
				success = status == GL_TRUE;
				glGetProgramiv(program, 
						GL_INFO_LOG_LENGTH, &len);
				if(len > 1) {
					char msg[len+1];
					msg[len] = '\0';
					glGetProgramInfoLog(program, 
							len, NULL, msg);
					errors.emplace_back((const char *)msg);
					return false;
				}
				glDeleteShader(frag);
			}
			glDeleteShader(vert);
		}
		return success;
	}

}
