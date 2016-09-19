#define GLFW_INCLUDE_NONE
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "view.hpp"
#include <iostream>

int main(int argc, const char **argv) {
	using namespace View;
	view v("share/shade.vert", "share/shade.frag");
	if(!v.init()) {
		std::cout << "Failed to initialize view." << std::endl;
		for(const auto &e : v.errors) {
			std::cout << e << std::endl;
		}
		return 1;
	}
	if(!v.run()) {
		for(const auto &e : v.errors) {
			std::cout << e << std::endl;
		}
	}
}
