#include "control.hpp"
#include "model/ply.hpp"
#include <sstream>

#include "omp.h"
#include <chrono>

namespace Control {
	void control::init(std::atomic_bool &alive) {
		glfwSetInputMode(viewer.win, GLFW_STICKY_KEYS, 1);
	}

	void control::poll(std::atomic_bool &alive) {
		std::ostringstream oss;
		if(alive)
			viewer.poll(alive);
		if(alive) {
			if(glfwGetKey(viewer.win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				alive = false;
			int nButtons;
			const uint8_t *buttons =
				glfwGetJoystickButtons(GLFW_JOYSTICK_1, &nButtons);
			bool once = false;
			for(int i = 0; i < nButtons; i++) {
				bool pressed = buttons[i] == GLFW_PRESS;
				if(once && pressed) oss << ", " << i;
				if(!once && pressed) {
					once = true;
					oss << i;
				}
			}
			if(once)
				std::cout << "\nPressed: " << oss.str() << std::endl;

			once = false;
			int nAxes;
			const GLfloat *axes =
				glfwGetJoystickAxes(GLFW_JOYSTICK_1, &nAxes);
			for(int i = 0; i < nAxes; i++) {
				std::cout << "Axis " << i << " = " << axes[i] << std::endl;
			}
			endl(std::cout);
		}
	}

	void control::run(std::atomic_bool &alive) {
		using namespace Model::Ply;
		auto delay = std::chrono::milliseconds(150);

		int frame = 0, dFrames = 0, fps = 0;
		auto kill = [&alive] {alive = false;};
		double t1 = glfwGetTime(), t2;
		while(true) {
			poll(alive);
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
