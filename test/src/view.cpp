#include "view.hpp"
#include <iostream>
#include <GLFW/glfw3.h>

#ifndef GLFW_TRUE
#define GLFW_TRUE 1
#endif
#ifndef GLFW_FALSE
#define GLFW_FALSE 0
#endif

int main(int argc, const char **argv) {
	using namespace View;
	if(glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialize GLFW "
			<< glfwGetVersionString() << std::endl;
		return 1;
	}
	std::atomic_bool alive(true);
	view v(alive, "share/shade.vert", "share/shade.frag");
	v.init(alive);
	v.run(alive);
	glfwTerminate();

	// TODO Determine why GLFW can init but cannot terminate
}
