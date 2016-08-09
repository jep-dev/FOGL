#include "main.hpp"

//#include "util.hpp"
#include "system.hpp"
//#include "math.hpp"
#include "model.hpp"
#include "view.hpp"

#include <chrono>
#include <iostream>
#include <thread>

#include "omp.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace System;
using PB = Printer_Base;

int main(int argc, const char **argv) {
	std::atomic_bool alive(true);
	auto delay = std::chrono::milliseconds(150);

	int tid;
 	#pragma omp parallel num_threads(e_tid_total) \
	shared(alive, delay) private(tid)
	{
		switch(omp_get_thread_num()) {
			case e_tid_view: {
				auto quit = [&alive]() {
					alive = false;
				};
				auto updateView = []() {
					return true;
				};
				View::view display("share/shade.vert", "share/shade.frag");
				if(display.valid) {
					display.run(updateView, quit);
				}
			} break;
			case e_tid_model: {
				int t = 0;
				auto updateModel = [&alive, &delay]{
					static int t = 0;
					while(alive) {
						std::cout << "Model update #" << t++ << std::endl;
						std::this_thread::sleep_for(delay);
					}
				};
				
				while(alive) {
					std::cout << "Model update #" << t++ << std::endl;
					std::this_thread::sleep_for(delay);
				}
			} break;
			default: break;
		}
	}

	glfwTerminate();

	return 0;
}
