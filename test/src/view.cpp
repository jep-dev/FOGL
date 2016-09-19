#define GLFW_INCLUDE_NONE
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "view.hpp"
#include <iostream>

int main(int argc, const char **argv) {
	using namespace View;
	std::atomic_bool alive(true);
	view v(alive, "share/shade.vert", "share/shade.frag");
	if(alive) {
		glGenBuffers(1, &v.ids[view::e_id_vbuf]);
		glBindBuffer(GL_ARRAY_BUFFER, v.ids[view::e_id_vbuf]);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
		glGenBuffers(1, &v.ids[view::e_id_fbuf]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v.ids[view::e_id_fbuf]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
		v.init(alive);
	} else {
		std::cout << "Failed to initialize view." << std::endl;
		return 1;
	}
	if(alive) v.run(alive);
}
