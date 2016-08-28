#include "control.hpp"
#include "model.hpp"

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
	void control::init(std::atomic_bool &alive) {
		//using namespace Util;
		// Task 1: view before model (splash)
		using namespace View;
		glfwSetInputMode(viewer.win, GLFW_STICKY_KEYS, 1);
		glfwMakeContextCurrent(viewer.win);

		// Task 2: wavefront obj model loading
		using namespace Model;
		obj_t object;
		auto status = obj_t::load(this -> mpath, object);
		if(status != obj_t::e_ok) {
			std::cout << "The model failed to load." << std::endl;
			return;
		}

		glGenBuffers(1, &viewer.ids[view::e_id_vbuf]);
		glBindBuffer(GL_ARRAY_BUFFER, viewer.ids[view::e_id_vbuf]);
		glBufferData(GL_ARRAY_BUFFER, object.floats.size(),
				(void*)(&object.floats[0]), GL_STATIC_DRAW);

		glGenBuffers(1, &viewer.ids[view::e_id_ibuf]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
				viewer.ids[view::e_id_ibuf]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.ints.size(),
				(void*)(&object.ints[0]), GL_STATIC_DRAW);
		viewer.nTriangles = object.ints.size()/3;
		glUseProgram(viewer.ids[view::e_id_prog]);

		/*
		// Task 2: model loading
		using namespace Model::Ply;
 		Header model(this -> mpath);
 		if(model.status) {
			std::cout << model.statusContext << std::endl;
			return;
		}
		auto start = begin(model.elements), 
				 stop = end(model.elements);
		auto vertices = std::find_if(start, stop,
			[](Element const& el) -> bool {
				// TODO Change behavior based on the number of elements
				return el.name == "vertex" && !el.has_list;
		}), indices = std::find_if(start, stop,
			[](Element const& el) -> bool {
				int sz = el.properties.size();
				return el.name == "face"
					&& (el.has_list ? sz==1 : sz==3);
		});
		if(vertices == stop || indices == stop) {
			std::cout << "The model is valid, but does not match "
				"the anticipated structure." << std::endl;
			return;
		Task 3: view after model
		glGenBuffers(1, &viewer.ids[view::e_id_vbuf]);
		glBindBuffer(GL_ARRAY_BUFFER, viewer.ids[view::e_id_vbuf]);
		glBufferData(GL_ARRAY_BUFFER, vertices->data.size(),
				(void*)(&vertices->data[0]), GL_STATIC_DRAW);

		glGenBuffers(1, &viewer.ids[view::e_id_ibuf]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
				viewer.ids[view::e_id_ibuf]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->data.size(),
				(void*)(&indices->data[0]), GL_STATIC_DRAW);
		viewer.nTriangles = indices -> instances;
		glUseProgram(viewer.ids[view::e_id_prog]);*/
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
				if(i==8 && pressed) 
					alive = false;
			}
			if(once)
				std::cout << "\nPressed: " << oss.str() << std::endl;

			once = false;
			int nAxes;
			const GLfloat *axes =
				glfwGetJoystickAxes(GLFW_JOYSTICK_1, &nAxes);
			for(int i = 0; i < nAxes; i++) {
				if(i == 0) {
					viewer.theta = axes[i]*2 + 1.5;
				} else if(i == 1) {
					viewer.phi = axes[i];
				}
				//std::cout << "Axis " << i << " = " << axes[i] << std::endl;
			}
			//endl(std::cout);
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

	control::control(std::atomic_bool &alive, const char *mpath,
			const char *vert, const char *frag):
		supertask(), mpath(mpath), viewer(alive, vert, frag) {}
}
