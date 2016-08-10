#include "control.hpp"
#include "omp.h"

#include <chrono>

namespace Control {
	void control::run(std::atomic_bool &alive,
			const char *vert, const char *frag) {
		auto delay = std::chrono::milliseconds(150);

		int tid;
 		#pragma omp parallel num_threads(e_tid_total) \
		shared(alive, delay, vert, frag) private(tid)
		{
			switch(omp_get_thread_num()) {
				case e_tid_view: {
				View::view viewer("share/shade.vert",
						"share/shade.frag");
					auto kill = [&alive] {alive = false;};
					if(viewer.valid) {
						viewer.run([](void){return true;}, alive);
					} else {
						alive = false;
					}
				} break;
				case e_tid_model: {
					int t = 0;
					while(alive) {
						std::cout << "Model update #" << t++ << std::endl;
						std::this_thread::sleep_for(delay);
					}
				} break;
				default: break;
			}
		}
		glfwTerminate();
	}
}
