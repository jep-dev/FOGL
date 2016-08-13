#include "view.hpp"
#include "system.hpp"
#include "model/ply.hpp"
#include "model/obj.hpp"

#include <iostream>
#include <sstream>
#include <math.h>

#include <GL/gl3w.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

namespace View {
	void printErrors(void) {
		GLenum err;
		while(!(err = glGetError())) {
			std::cout << gluErrorString(err) << "\n";
		}
	}

	void view::setUniforms(void) {
		int w, h;
		glfwGetFramebufferSize(win, &w, &h);
		float mag = float(1/tan(fov*M_PI/180));
		float mat_proj[]{
				mag*h/w, 0, 0, 0, 0, mag, 0, 0,
				0, 0, (far+near)/(far-near), -1,
				0, 0, 2*far*near/(far-near), 0
		};
		glUniformMatrix4fv(ids[e_id_proj], 1, GL_TRUE, mat_proj);

		/* TODO - static not intended, just persistence;
 		 * all of the data here should be passed or shared */
		static float theta = 0;
		theta += M_PI/180;
		float c = cos(theta), s = sin(theta);
		float mat_model[]{
				c, 0,-s, 0,
				0, 1, 0, 0,
				s, 0, c, c,
				0, 0, 0, 1
		}, mat_view[]{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 2, 1
		};

		glUniformMatrix4fv(ids[e_id_model], 1, GL_TRUE, mat_model);
		glUniformMatrix4fv(ids[e_id_view], 1, GL_FALSE, mat_view);
	}
	
	void view::redraw(int frame, int fps) {
		static constexpr const unsigned int
			offset = 3*sizeof(float),
			stride = 2*offset,
			bits = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		setUniforms();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindBuffer(GL_ARRAY_BUFFER, ids[e_id_vbuf]);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT,
				GL_FALSE, stride, nullptr);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT,
				GL_FALSE, stride, (void*) offset);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[e_id_ibuf]);
		glDrawElements(GL_TRIANGLES, nTriangles*3,
				GL_UNSIGNED_INT, nullptr);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glfwSwapBuffers(win);
	}

	// TODO move to control along with View::Inputs
	void view::poll(std::atomic_bool &alive) {
		if(alive && win) {
			glfwMakeContextCurrent(win);
			glfwPollEvents();
		}
		if(glfwWindowShouldClose(win)) {
			alive = false;
		}
	}
	
	void view::run(std::atomic_bool& alive, int frame, int fps) {
		if(alive && win && !glfwWindowShouldClose(win)) {
			glfwMakeContextCurrent(win);
			redraw(frame, fps);
		}
	}
	view::view(std::atomic_bool &alive, const char *vert, const char *frag) {
		if(!alive) {
			return;
		}
		if(glfwInit() == 0) {
			alive = false;
			//std::cout << "glfwInit() returned 0!" << std::endl;
			return;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		if(!(win = glfwCreateWindow(512, 512, "View", NULL, NULL))) {
			std::cout << "Could not create window." << std::endl;
			alive = false;
			return;
		}

		glfwMakeContextCurrent(win);
		if(gl3wInit()) {
			std::cout << "Could not initialize gl3w." << std::endl;
			alive = false;
			return;
		}
		glfwSetWindowSizeCallback(win, 
			[](GLFWwindow*, int w, int h){
				glViewport(0, 0, w, h);
			}
		);
		glfwSetErrorCallback(
			[] (int, const char *szErr) {
				std::cout << szErr << std::endl;
			});
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glGenVertexArrays(1, &ids[e_id_va]);
		glBindVertexArray(ids[e_id_va]);
		
		ids[e_id_prog] = glCreateProgram();
		if(!link(vert, frag, ids[e_id_prog])) {
			std::cout << "Could not compile/link shader(s)." << std::endl;
			alive = false;
			return;
		}
		ids[e_id_model] = glGetUniformLocation(ids[e_id_prog], "model");
		ids[e_id_view] = glGetUniformLocation(ids[e_id_prog], "view");
		ids[e_id_proj] = glGetUniformLocation(ids[e_id_prog], "proj");
		
		/*if(model.status) {
			std::cout << model.statusContext << std::endl;
			return;
		}
		auto start = begin(model.elements), 
				 stop = end(model.elements);
		auto getVertices = [](Element const& el) -> bool {
			return el.name == "vertex" && !el.has_list
				&& (el.properties.size() == 6 || el.properties.size() == 3);
		};
		auto getIndices = [](Element const& el) -> bool {
			int sz = el.properties.size();
			return el.name == "face"
				&& (el.has_list ? sz==1 : sz==3);
		};
		auto vertices = std::find_if(start, stop, getVertices),
				 indices = std::find_if(start, stop, getIndices);
		if(vertices == stop || indices == stop) {
			std::cout << "The model is valid, but does not match "
				"the anticipated structure." << std::endl;
			return;
		}

		glGenBuffers(1, &ids[e_id_vbuf]);
		glBindBuffer(GL_ARRAY_BUFFER, ids[e_id_vbuf]);
		glBufferData(GL_ARRAY_BUFFER, vertices->data.size(),
				(void*)(&vertices->data[0]), GL_STATIC_DRAW);

		glGenBuffers(1, &ids[e_id_ibuf]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[e_id_ibuf]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->data.size(),
				(void*)(&indices->data[0]), GL_STATIC_DRAW);
		nTriangles = indices -> instances;
		glUseProgram(ids[e_id_prog]);*/
	}
	view::~view(void) {
		if(ids[e_id_prog]) {
			glDeleteProgram(ids[e_id_prog]);
		}
		glfwDestroyWindow(win);
	}
}
