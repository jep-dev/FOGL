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
				mag, 0, 0, 0, 0, mag, 0, 0,
				0, 0, (far+near)/(far-near), -1,
				0, 0, 2*far*near/(far-near), 0
		};
		glUniformMatrix4fv(ids[e_id_proj], 1, GL_TRUE, mat_proj);

		float ct = cos(-theta*1.5), st = sin(-theta*1.5),
			  cp = cos(-phi), sp = sin(-phi);
		float mat_model[]{
				    ct,  0,    -st, 0,
				-sp*st, cp, -sp*ct, 0,
				 cp*st, sp,  cp*ct, 0,
				     0,  0,      0, 1
		}, mat_view[]{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 2, 1
		};

		glUniformMatrix4fv(ids[e_id_model], 1, GL_TRUE, mat_model);
		glUniformMatrix4fv(ids[e_id_view], 1, GL_FALSE, mat_view);
	}
	
	void view::redraw() {
		static constexpr const unsigned int
			offset = 3*sizeof(float),
			//stride = offset,
			stride = 2*offset,
			bits = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		setUniforms();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindBuffer(GL_ARRAY_BUFFER, ids[e_id_vbuf]);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT,
				GL_FALSE, stride, nullptr);
		
		///*
 		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT,
				GL_FALSE, stride, (void*) offset);
		//*/

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[e_id_ibuf]);
		glDrawElements(GL_TRIANGLES, nTriangles*3,
				GL_UNSIGNED_INT, nullptr);
		///*
		glDisableVertexAttribArray(1);
		//*/
		glDisableVertexAttribArray(0);
		glfwSwapBuffers(win);
	}

	void view::poll(std::atomic_bool &alive) {
		if(alive && win) {
			glfwMakeContextCurrent(win);
			glfwPollEvents();
		}
		if(glfwWindowShouldClose(win)) {
			alive = false;
		}
	}
	void view::init(std::atomic_bool &alive) {
		glfwSetWindowSizeCallback(win, 
			[](GLFWwindow*, int w, int h){
				glViewport(0, 0, w, h);
			}
		);
		glfwSetErrorCallback(
			[] (int, const char *szErr) {
				std::cout << szErr << std::endl;
			});
	}
	
	void view::run(std::atomic_bool &alive) {
		if(alive && win && !glfwWindowShouldClose(win)) {
			glfwMakeContextCurrent(win);
			redraw();
		}
	}
	view::view(std::atomic_bool &alive, const char *vert, const char *frag) {
		if(!alive) {
			return;
		}
		if(glfwInit() == 0) {
			alive = false;
			return;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		if(!(win = glfwCreateWindow(680, 680, "View", NULL, NULL))) {
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
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH, GL_NICEST);

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
		glUseProgram(ids[e_id_prog]);
	}
	view::~view(void) {
		if(ids[e_id_prog]) {
			glDeleteProgram(ids[e_id_prog]);
		}
		glfwDestroyWindow(win);
	}
}
