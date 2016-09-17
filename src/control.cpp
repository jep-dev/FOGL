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

	bool control::init(std::atomic_bool &alive) {
		//using namespace Util;
		// Task 1: view before model (splash)
		using namespace View;
		glfwSetInputMode(viewer.win, GLFW_STICKY_KEYS, 1);
		glfwMakeContextCurrent(viewer.win);

		using namespace Model;
		model model;
		if(mpath) {
			obj_t obj;
			auto status = obj_t::load(this -> mpath, obj);
			if(status != obj_t::e_ok) {
				errors.push_back("The model failed to load.");
				return alive = false;
			}
			model = obj;
		} else {
			model = mesh_t(150, 150,
			[](float s, float t, std::vector<float> &vertices) {
				using namespace Math;
				auto theta = s*M_PI*2, phi = t*M_PI;
				vertices.emplace_back(cos(theta)*sin(phi)); // X
				vertices.emplace_back(sin(theta)*sin(phi)); // Y
				vertices.emplace_back(cos(phi));            // Z
			});
		}

		glGenBuffers(1, &viewer.ids[view::e_id_vbuf]);
		glBindBuffer(GL_ARRAY_BUFFER, viewer.ids[view::e_id_vbuf]);
		glBufferData(GL_ARRAY_BUFFER, model.floats.size() * sizeof(float),
			(void*) &model.floats[0], GL_STATIC_DRAW);

		glGenBuffers(1, &viewer.ids[view::e_id_fbuf]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			viewer.ids[view::e_id_fbuf]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.ints.size() * sizeof(int),
			(void*) &model.ints[0], GL_STATIC_DRAW);
		viewer.nTriangles = model.ints.size()/3;
		return alive;
		//return viewer.init(alive);
	}

	bool control::poll(std::atomic_bool &alive) {
		std::ostringstream oss;
		if(alive)
			viewer.poll(alive);
		if(alive) {
			if(glfwGetKey(viewer.win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				alive = false;
			int nButtons;
			const uint8_t *buttons =
				glfwGetJoystickButtons(GLFW_JOYSTICK_1, &nButtons);
			for(int i = 0; i < nButtons; i++) {
				if(buttons[i] == GLFW_PRESS) {
					if(i == 8) alive = false;
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
		return alive;
	}

	bool control::run(std::atomic_bool &alive) {
		auto delay = std::chrono::milliseconds(150);

		int frame = 0, dFrames = 0, fps = 0;
		auto kill = [&alive] {alive = false;};
		double t1 = glfwGetTime(), t2;
		while(true) {
			if(poll(alive)) viewer.run(alive);
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
		return alive;
	}

	control::control(std::atomic_bool &alive, const char *mpath):
		task(), mpath(mpath), viewer(alive) {}
}
