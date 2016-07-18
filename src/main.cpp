//#include "main.hpp"

#include "util.hpp"      // TMP, functional, algorithms
#include "util/types.hpp"
#include "util/functional.hpp"

#include "system.hpp" // Files, I/O, term adaptor; needs term cap
#include "system/net.hpp"  // Net adaptor & services for I/O

#include "math.hpp"  // Quaternions, duals, transforms
#include "math/quat.hpp"
#include "math/dual.hpp"
#include "math/affine.hpp"

#include "model.hpp" // Deserializes .ply model data
#include "model/ply.hpp"

#include "view.hpp" // Windowed model viewer (GLFW)
#include "view/shade.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <thread>

#include "omp.h"
#include <boost/asio.hpp>
#include <boost/asio/yield.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

//using namespace Model;
//using namespace Detail;

/*using std::cout;
using std::ostream;
using std::endl;*/

using namespace System;
using PB = Printer_Base;

int main(int argc, const char **argv) {
	const short port = 5000;
	const int workers = 3;

	// OpenMP stub;
	// TODO migrate from std thread over closures to OMP
	/* int id;
 	#pragma omp parallel num_threads(3) \
	shared(port, workers) private(id)
	{
		//switch(omp_get_thread_num()) {}
	}*/

	std::atomic_bool alive(true);
	auto delay = std::chrono::milliseconds(150);
	
	auto quit = [&alive]() {
		alive = false;
	};
	auto updateView = []() {
		return true;
	};

	// Update model based on state & delay
	auto updateModel = [&alive, &delay]{
		static int t = 0;
		while(alive) {
			std::cout << "Model update #" << t++ << std::endl;
			std::this_thread::sleep_for(delay);
		}
	};

	View::view display;
	if(display.valid) {
		std::thread modelThread(updateModel);
		display.run(updateView, quit);
		modelThread.join();
	}

	glfwTerminate();

	return 0;
}
