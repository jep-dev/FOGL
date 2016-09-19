#include "control.hpp"
#include "math.hpp"
#include "model.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <SOIL/SOIL.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>

#include "omp.h"

namespace Control {

	void control::deadzone(float &x, float &y) {
		float r = sqrt(x*x + y*y), theta = atan2(y, x);
		if(r < r_dead) {
			x = y = 0;
		} else {
			x = (x/r - r_dead) / (1-r_dead);
			y = (y/r - r_dead) / (1-r_dead);
		}
	}

	bool control::init(void) {
		if(!alive) return false;
		//using namespace Util;
		// Task 1: view before model (splash)
		using namespace View;
		glfwSetInputMode(viewer.win, GLFW_STICKY_KEYS, 1);
		glfwMakeContextCurrent(viewer.win);

		glBindBuffer(GL_ARRAY_BUFFER, viewer.ids[view::e_id_vbuf]);
		glBufferData(GL_ARRAY_BUFFER, model.floats.size() * sizeof(float),
			(void*) &model.floats[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			viewer.ids[view::e_id_fbuf]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.ints.size() * sizeof(int),
			(void*) &model.ints[0], GL_STATIC_DRAW);
		viewer.nTriangles = model.ints.size()/3;
		return true;
	}

	bool control::poll(void) {
		if(!alive) return false;
		std::ostringstream oss;
		if(viewer.poll()) {
			if(glfwGetKey(viewer.win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				return false;
			int nButtons;
			const uint8_t *buttons =
				glfwGetJoystickButtons(GLFW_JOYSTICK_1, &nButtons);
			for(int i = 0; i < nButtons; i++) {
				if(buttons[i] == GLFW_PRESS) {
					if(i == 8) return false;
					else if(i == 7)
						viewer.x = viewer.y = viewer.z = 0;
					else if(i == 4)
						viewer.y -= .1;
					else if(i == 5)
						viewer.y += .1;
				}
			}
			int nAxes;
			const GLfloat *axes =
				glfwGetJoystickAxes(GLFW_JOYSTICK_1, &nAxes);
			if(nAxes >= 2) {
				float x = axes[0], y = axes[1];
				deadzone(x, y);
				viewer.x -= x/20;
				viewer.z -= y/20;
			}
			if(nAxes >= 5) {
				float theta = axes[3], phi = axes[4];
				viewer.theta = theta*M_PI;
				viewer.phi = phi*M_PI;
			}
		}
		return true;
	}

	bool control::run(void) {
		if(!alive) return false;
		auto delay = 150;

		int frame = 0, dFrames = 0, fps = 0;
		double t1 = glfwGetTime(), t2;
		while(true) {
			if(!poll()) {
				errors.emplace_back("Safely shutting down");
				break;
			} else if(!viewer.run()) {
				errors.emplace_back("Failed to run view");
				break;
			}
			if(delay > 1) {
				std::this_thread::sleep_for(
						std::chrono::milliseconds(delay));
			} else {
				delay = 1;
			}
			t2 = glfwGetTime() - t1;
			frame++;
			dFrames++;
			if(t2 > 1.0) {
				delay *= dFrames / t2 / 60;
				std::cout << "FPS: " << int(dFrames/t2) << std::endl;
				t1 += t2;
				dFrames = 0;
			}
		}
		return false;
	}

	control::control(const Model::model &model, View::view &view):
		task(), model(model), viewer(view) {
		alive = true;
	}
}
