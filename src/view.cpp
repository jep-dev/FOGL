
#include "view.hpp"
#include "view/shade.hpp"

#include "system.hpp"
#include "model/ply.hpp"

#include <iostream>
#include <sstream>
#include <math.h>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace View {

	void printErrors(const char *prefix = "OpenGL error(s): ") {
		GLenum err;
		bool once = false;
		std::ostringstream oss;
		
		while(!(err = glGetError())) {
			once = true;
			switch(err) {
			case GL_INVALID_ENUM:
				oss << "invalid enum; ";
				break;
			case GL_INVALID_VALUE:
				oss << "invalid value; ";
				break;
			case GL_INVALID_OPERATION:
				oss << "invalid operation; ";
				break;
			case GL_STACK_OVERFLOW:
				oss << "stack overflow; ";
				break;
			case GL_STACK_UNDERFLOW:
				oss << "stack underflow; ";
				break;
			case GL_OUT_OF_MEMORY:
				oss << "out of memory; ";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				oss << "invalid framebuffer operation; ";
				break;
			default:
				break;
			}
		}
		 
		if(once) {
			std::cout << prefix << oss.str() << std::endl;
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
		glUniformMatrix4fv(ids[proj_id], 1, GL_TRUE, mat_proj);

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

		glUniformMatrix4fv(ids[model_id], 1, GL_TRUE, mat_model);
		glUniformMatrix4fv(ids[view_id], 1, GL_FALSE, mat_view);
	}
	
	void view::redraw(int frame, int fps) {
		static constexpr const unsigned int
			offset = 3*sizeof(float),
			stride = 2*offset;

		setUniforms();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, ids[vbuf_id]);
		glVertexAttribPointer(0, 3, GL_FLOAT,
				GL_FALSE, stride, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT,
				GL_FALSE, stride, (void*) offset);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[ibuf_id]);
		glDrawElements(GL_TRIANGLES, nTriangles*3,
				GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glfwSwapBuffers(win);
	}
	
	void view::run(std::function<bool()> update,
			std::function<void()> quit) {
		if(valid && win) {
			glfwMakeContextCurrent(win);
			double t1 = glfwGetTime(), t2;
			int frame = 0, dFrame = 0, fps;
			while(true) {
				glfwPollEvents();
				if(glfwWindowShouldClose(win)) {
					quit();
				} else if(update()) {
					redraw(frame + dFrame, fps);
					t2 = glfwGetTime();
					if(t2-t1 >= .5) {
						fps = int(dFrame/(t2-t1));
						std::cout << fps << " fps\n";
						t1 = t2;
						frame += dFrame;
						dFrame = 0;
					}
					dFrame++;
					continue;
				}
				break;
			}
		}
	}

	view::view(const char *vert, const char *frag) {
		using namespace Model::Ply;

		// TODO - safe model and shader loading at runtime
		//static constexpr const char *mpath = "share/bunny.ply";

		//Header model(mpath);
		if(!glfwInit()) {
			std::cout << "Could not initialize GLFW." << std::endl;
			return;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		win = glfwCreateWindow(512, 512, "View", NULL, NULL);

		if(!win) {
			std::cout << "Could not create window." << std::endl;
			return;
		}

		glfwMakeContextCurrent(win);
		if(gl3wInit()) {
			std::cout << "Could not initialize gl3w." << std::endl;
			return;
		}
		glfwSetWindowSizeCallback(win, 
			[](GLFWwindow*, int w, int h){
				glViewport(0, 0, w, h);
			}
		);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glGenVertexArrays(1, &ids[va_id]);
		glBindVertexArray(ids[va_id]);

		/*if(model.status) {
			std::cout << model.statusContext << std::endl;
			return;
		}
		auto start = begin(model.elements), 
				 stop = end(model.elements);
		auto getVertices = [](Element const& el) -> bool {
			return el.name == "vertex" && !el.has_list
				&& el.properties.size() == 6;
		};
		auto getIndices = [](Element const& el) -> bool {
			int sz = el.properties.size();
			return el.name == "face"
				&& (el.has_list ? sz==1 : sz==3);
		};
		auto vertices = std::find_if(start, stop, getVertices),
				 indices = std::find_if(start, stop, getIndices);
		if(vertices == stop || indices == stop) {
			valid = false;
			std::cout << "The model is valid, but does not match "
				"the anticipated structure." << std::endl;
			return;
		}*/
		//Model::contour<GLfloat, Util::undef_t, GLclampf, GLclampf> contour;
		const int iMax = 10, jMax = iMax;
		GLfloat vertices[3*iMax*jMax];
		GLclampf u, v;
		for(int i = 0; i < iMax; i++) {
			u = i/(GLclampf) iMax;
			for(int j = 0; j < jMax; j++) {
				v = j/(GLclampf) jMax;
				/*contour((GLfloat*) (&vertices[3*(i*iMax+j)]),
						(GLclampf) u, (GLclampf) v);*/
			}
		}

		glGenBuffers(1, &ids[vbuf_id]);
		glBindBuffer(GL_ARRAY_BUFFER, ids[vbuf_id]);
		/*glBufferData(GL_ARRAY_BUFFER, vertices->data.size(),
				(void*)(&vertices->data[0]), GL_STATIC_DRAW);*/

		glGenBuffers(1, &ids[ibuf_id]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[ibuf_id]);
		/*glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->data.size(),
				(void*)(&indices->data[0]), GL_STATIC_DRAW);
		nTriangles = indices -> instances;*/
		
		ids[prog_id] = glCreateProgram();
		if(!link(vert, frag, ids[prog_id])) {
			std::cout << "Could not compile/link shader(s)." << std::endl;
			return;
		}
		ids[model_id] = glGetUniformLocation(ids[prog_id], "model");
		ids[view_id] = glGetUniformLocation(ids[prog_id], "view");
		ids[proj_id] = glGetUniformLocation(ids[prog_id], "proj");
		glUseProgram(ids[prog_id]);
		valid = true;
	}
	view::~view(void) {
		if(ids[prog_id]) {
			glDeleteProgram(ids[prog_id]);
		}
		glfwDestroyWindow(win);
	}
}
