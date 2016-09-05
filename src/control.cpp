#include "control.hpp"
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
	bool control::init(std::atomic_bool &alive) {
		//using namespace Util;
		// Task 1: view before model (splash)
		using namespace View;
		glfwSetInputMode(viewer.win, GLFW_STICKY_KEYS, 1);
		glfwMakeContextCurrent(viewer.win);

		using namespace Model;
		mesh_t mesh(50, 50, [](float s, float t, std::vector<float> &vertices) {
			vertices.push_back(2*s-1);
			vertices.emplace_back(2*t-1);
			vertices.emplace_back(s*s+t*t);
		});

		glGenBuffers(1, &viewer.ids[view::e_id_vbuf]);
		glBindBuffer(GL_ARRAY_BUFFER, viewer.ids[view::e_id_vbuf]);
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size(),
			(void*) &mesh.vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &viewer.ids[view::e_id_fbuf]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, viewer.ids[view::e_id_fbuf]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size(),
			(void*) &mesh.faces[0], viewer.ids[view::e_id_fbuf]);
		viewer.nTriangles = mesh.faces.size()/3;
		return alive;
		/*
		// Task 2: wavefront obj model loading
		using namespace Model;
		obj_t object;
		auto status = obj_t::load(this -> mpath, object);
		if(status != obj_t::e_ok) {
			errors.push_back("The model failed to load.");
			return alive = false;
		}

		glGenBuffers(view::e_id_model-view::e_id_vbuf,
				&viewer.ids[view::e_id_vbuf]);
		// TODO Use more than first range of each obj type
		if(object.v_beg.size() == 0) {
			errors.push_back("The loaded model does not contain vertices.");
			return alive = false;
		} else {
			auto v0 = object.v_beg[0], v1 = object.v_end[0];
			glBindBuffer(GL_ARRAY_BUFFER, viewer.ids[view::e_id_vbuf]);
			glBufferData(GL_ARRAY_BUFFER, v1-v0,
					(void*)(&object.floats[v0]), GL_STATIC_DRAW);
		}
		if(object.vn_beg.size() != 0) {
			auto vn0 = object.vn_beg[0], vn1 = object.vn_end[0];
			glBindBuffer(GL_ARRAY_BUFFER, viewer.ids[view::e_id_vnbuf]);
			glBufferData(GL_ARRAY_BUFFER, vn1-vn0,
					(void*)(&object.floats[vn0]), GL_STATIC_DRAW);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
				viewer.ids[view::e_id_fbuf]);
		int f0, f1;
		if(object.f0_beg.size() != 0) {
			f0 = object.f0_beg[0];
			f1 = object.f0_end[0];
		} else if(object.f1_beg.size() != 0) {
			f0 = object.f1_beg[0];
			f1 = object.f1_end[0];
		} else if(object.f2_beg.size() != 0) {
			f0 = object.f2_beg[0];
			f1 = object.f2_end[0];
		} else if(object.f3_beg.size() != 0) {
			f0 = object.f3_beg[0];
			f1 = object.f3_end[0];
		} else {
			errors.push_back("The loaded model does not contain faces.");
			return alive = false;
		}
		viewer.nTriangles = (f1-f0)/3;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, viewer.nTriangles*3,
				(void*)(&object.ints[f0]), GL_STATIC_DRAW);*/

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
				bool pressed = buttons[i] == GLFW_PRESS;
				if(i==8 && pressed) 
					alive = false;
			}
			int nAxes;
			const GLfloat *axes =
				glfwGetJoystickAxes(GLFW_JOYSTICK_1, &nAxes);
			for(int i = 0; i < nAxes; i++) {
				if(i == 0) {
					viewer.theta = axes[i]*2 + 1.5;
				} else if(i == 1) {
					viewer.phi = axes[i];
				}
			}
		}
		return alive;
	}

	bool control::run(std::atomic_bool &alive) {
		using namespace Model::Ply;
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
