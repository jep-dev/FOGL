#include "system.hpp"
#include "control.hpp"
#include "math.hpp"
#include "main.hpp"

#include <iostream>

#include "omp.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace System;
using PB = Printer_Base;

int main(int argc, const char **argv) {
	using namespace Math;
	auto __1 = quat<float>{1,0,0,0},
		 __2 = quat<float>{0,1,0,0},
		 __3 = quat<float>{0,0,1,0},
		 __4 = quat<float>{0,0,0,1};
	auto ident = quat<quat<float>>{__1,__2,__3,__4};

	glfwInit();
	std::atomic_bool alive(true);
	Control::control ctl(alive, "share/shade.vert", "share/shade.frag");
	ctl.run(alive);
	return 0;
}
