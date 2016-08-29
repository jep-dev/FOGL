#include <iostream>
#include "main.hpp"

#include "omp.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

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
	glfwInit();
	std::atomic_bool alive(true);
	Control::control ctl(alive, OBJ_PATH, VERT_PATH, FRAG_PATH);
	task::init(alive, &ctl);
	task::run(alive, &ctl);
	glfwTerminate();

	return 0;
}
