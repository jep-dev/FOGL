#include <iostream>
#include "main.hpp"

#ifndef OBJ_PATH
#define OBJ_PATH "share/icosahedron.obj"
#endif
#ifndef VERT_PATH
#define VERT_PATH "share/fallback.vert"
#endif
#ifndef FRAG_PATH
#define FRAG_PATH "share/shade.frag"
#endif

int main(int argc, const char **argv) {
	using namespace Util;
	std::atomic_bool alive(true);
	if(glfwInit() == 0) {
		std::cout << "Failed to initialize GLFW "
			<< glfwGetVersionString() << std::endl;
		return 1;
	}
	Control::control ctl(alive, OBJ_PATH, VERT_PATH, FRAG_PATH);
	task::init(alive, &ctl);
	task::run(alive, &ctl);
	glfwTerminate();

	return 0;
}
