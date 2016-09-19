#include "view.hpp"
#include "system.hpp"

#include <iostream>
#include <sstream>
#include <math.h>

#define GLFW_INCLUDE_NONE
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

namespace View {

	void view::setUniforms(void) {
		int w, h;
		glfwGetFramebufferSize(win, &w, &h);
		glViewport(0, 0, w, h);
		float mag = 1.0f/tan(fov*M_PI/180),
			ct = cos(theta/2), st = sin(theta/2),
			cp = cos(phi/2), sp = sin(phi/2),
		mat_proj[]{ // Projection matrix
			mag*h/w, 0, 0, 0,
			0, mag, 0, 0,
			0, 0, (far+near)/(far-near), -1,
			0, 0, 2*far*near/(far-near), 0
		}, mat_view[]{ // View matrix
			ct*cp, -ct*sp, -st, 0,
			sp,        cp,   0, 0,
			st*cp, -st*sp,  ct, 0,
			0,          0,   0, 1
		}, mat_model[]{ // Model matrix
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z-2.5f, 1
		};

		glUniformMatrix4fv(ids[e_id_proj], 1, GL_FALSE, mat_proj);
		glUniformMatrix4fv(ids[e_id_model], 1, GL_FALSE, mat_model);
		glUniformMatrix4fv(ids[e_id_view], 1, GL_FALSE, mat_view);
	}
	
	void view::redraw() {
		static constexpr const unsigned int
			offset = 3*sizeof(float),
			stride = offset,
			//stride = 2*offset,
			bits = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		setUniforms();
		glClearColor(0.25,0.25,0.25,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindBuffer(GL_ARRAY_BUFFER, ids[e_id_vbuf]);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT,
				GL_FALSE, stride, nullptr);
		
 		/*glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT,
				GL_FALSE, stride, (void*) offset);*/

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[e_id_fbuf]);
		glDrawElements(GL_TRIANGLES, nTriangles*3,
				GL_UNSIGNED_INT, nullptr);

		//glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glfwSwapBuffers(win);
	}

	bool view::poll(std::atomic_bool &alive) {
		if(alive && win) {
			glfwMakeContextCurrent(win);
			glfwPollEvents();
		} else {
			return alive = false;
		}
		if(glfwWindowShouldClose(win)) {
			return alive = false;
		}
		return alive;
	}
	bool view::init(std::atomic_bool &alive) {
		if(!alive) {
			return false;
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

		if(ids[e_id_prog]) glDeleteProgram(ids[e_id_prog]);
		ids[e_id_prog] = glCreateProgram();
		if(!link(vert_fname, frag_fname, ids[e_id_prog], errors)) {
			errors.emplace_back("Could not compile/link shader(s).");
			return alive = false;
		}
		auto mat_model = glGetUniformLocation(ids[e_id_prog], "model"),
			 mat_view = glGetUniformLocation(ids[e_id_prog], "view"),
			 mat_proj = glGetUniformLocation(ids[e_id_prog], "proj");
		if(mat_model == -1) {
			errors.emplace_back("Could not find uniform 'model'");
			return alive = false;
		}
		ids[e_id_model] = mat_model;
		if(mat_view == -1) {
			errors.emplace_back("Could not find uniform 'view'");
			return alive = false;
		}
		ids[e_id_view] = mat_view;
		if(mat_proj == -1) {
			errors.emplace_back("Could not find uniform 'view'");
			return alive = false;
		}
		ids[e_id_proj] = mat_proj;
		glUseProgram(ids[e_id_prog]);
		if(!ids[e_id_vbuf]) glGenBuffers(1, &ids[e_id_vbuf]);
		if(!ids[e_id_fbuf]) glGenBuffers(1, &ids[e_id_fbuf]);
		return alive;
	}
	
	bool view::run(std::atomic_bool &alive) {
		if(alive && win && !glfwWindowShouldClose(win)) {
			glfwMakeContextCurrent(win);
			redraw();
		}
		return alive;
	}
	view::view(std::atomic_bool &alive,
			const char *vert_fname, const char *frag_fname):
		vert_fname(vert_fname), frag_fname(frag_fname),
			theta(0), phi(0), x(0), y(0), z(0)
	{
		if(!alive) {
			return;
		}
		if(glfwInit() == 0) {
			errors.emplace_back("Could not initialize GLFW.");
			alive = false;
			return;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_SAMPLES, 16);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		if(!(win = glfwCreateWindow(680, 680, "View", NULL, NULL))) {
			errors.emplace_back("Could not create window.");
			alive = false;
			return;
		}

		glfwMakeContextCurrent(win);
		if(gl3wInit()) {
			errors.emplace_back("Could not initialize gl3w.");
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
	}
	view::~view(void) {
		if(ids[e_id_prog]) {
			glDeleteProgram(ids[e_id_prog]);
		}
		glfwDestroyWindow(win);
		glfwTerminate();
	}
}
