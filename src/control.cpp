#include "control.hpp"
#include "model/ply.hpp"

#include "omp.h"

#include <GLFW/glfw3.h>
#include <chrono>

namespace Control {

	void control::run(std::atomic_bool &alive,
			const char *vert, const char *frag) {
		using namespace Model::Ply;
		auto delay = std::chrono::milliseconds(150);

		int frame = 0, dFrames = 0, fps = 0;
		View::view viewer(alive, vert, frag);
		auto kill = [&alive] {alive = false;};
		double t1 = glfwGetTime(), t2;
		while(true) {
			if(alive) viewer.poll(alive);
			if(alive) viewer.run(alive, frame, fps);
			t2 = glfwGetTime() - t1;
			frame++;
			dFrames++;
			if(t2 > 1.0) {
				std::cout << "FPS: " << int(dFrames/t2) << std::endl;
				t1 += t2;
				dFrames = 0;
			}
			if(!alive) break;
		}
	}
}
