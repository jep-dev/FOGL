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
	if(glfwInit() == 0) {
		std::cout << "Failed to initialize GLFW "
			<< glfwGetVersionString() << std::endl;
		return 1;
	}

	std::atomic_bool alive(true);
	const char *obj_fname, *vert_fname, *frag_fname;

	if(argc >= 2) obj_fname = argv[1];
	else obj_fname = OBJ_PATH;
	if(argc >= 3) vert_fname = argv[2];
	else vert_fname = VERT_PATH;
	if(argc >= 4) frag_fname = argv[3];
	else frag_fname = FRAG_PATH;

	Control::control ctl(alive, obj_fname, vert_fname, frag_fname);
	if(alive) {
		task::init(alive, &ctl);
	} else {
		std::cout << "Initialization of control failed." << std::endl;
		return 1;
	}
	if(alive) {
		task::run(alive, &ctl);
	} else {
		std::cout << "Control failed while running." << std::endl;
	}


	glfwTerminate();

	return 0;
}
