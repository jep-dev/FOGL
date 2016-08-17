#include "control.hpp"
#include "model/ply.hpp"

#include "omp.h"
#include <chrono>

namespace Control {
	void control::init(std::atomic_bool &alive) {
		glfwSetWindowSizeCallback(viewer.win,
			[] (GLFWwindow *win, int wx, int wy) {
				std::cout << "Resize: " << wx << ", " << wy << std::endl;
			});
		glfwSetCursorPosCallback(viewer.win,
			[] (GLFWwindow *win, double x, double y) {
				std::cout << "Mouse: " << x << ", " << y << std::endl;
			});
		glfwSetKeyCallback(viewer.win,
			[] (GLFWwindow *win, int key, int scancode, int action, int mods) {
				if(action == GLFW_PRESS) {
					switch(key) {
					case GLFW_KEY_ESCAPE: {
						std::cout << "Escape press" << std::endl;
					} break;
					default: break;
					}
				}
			});
	}

	void control::run(std::atomic_bool &alive) {
		using namespace Model::Ply;
		auto delay = std::chrono::milliseconds(150);

		int frame = 0, dFrames = 0, fps = 0;
		auto kill = [&alive] {alive = false;};
		double t1 = glfwGetTime(), t2;
		while(true) {
			if(alive) viewer.poll(alive);
			if(alive) viewer.run(alive);
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
	control::control(std::atomic_bool &alive,
			const char *vert, const char *frag):
		task(), viewer(alive, vert, frag) {
			init(alive);
		}
}
