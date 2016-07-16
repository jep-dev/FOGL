
#include "view.hpp"
#include "view/shade.hpp"

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
		float projection[]{
				mag*h/w, 0, 0, 0, 0, mag, 0, 0,
				0, 0, (far+near)/(far-near), -1,
				0, 0, 2*far*near/(far-near), 0
		};
		glUniformMatrix4fv(projID, 1, GL_TRUE,  projection);

		/* TODO - static not intended, just persistence;
 		 * all of the data here should be passed or shared */
		static float theta = 0;
		theta += M_PI/180;
		float c = cos(theta), s = sin(theta);
		float modelData[]{
				c, 0,-s, 0,
				0, 1, 0, 0,
				s, 0, c, c,
				0, 0, 0, 1
		}, viewData[]{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 2, 1
		};

		glUniformMatrix4fv(modelID, 1, GL_TRUE,  modelData);
		glUniformMatrix4fv(viewID, 1, GL_FALSE, viewData);
	}
	
	void view::redraw(void) {
		setUniforms();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		static long int offset = 3*sizeof(float),
				stride = 2*offset;
		glBindBuffer(GL_ARRAY_BUFFER, vbuf);
		glVertexAttribPointer(0, 3, GL_FLOAT,
				GL_FALSE, stride, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT,
				GL_FALSE, stride, (void*) offset);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
		glDrawElements(GL_TRIANGLES, nTriangles*3,
				GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(win);

		static int nFrames = -1;
		nFrames++;
		static double t0 = glfwGetTime();
		double t1 = glfwGetTime(), dt = t1-t0;
		if(dt >= 1) {
			std::cout << (nFrames/dt) << "FPS" << std::endl;
			t0 = t1;
			nFrames = -1;
		}
	}

	void view::run(std::function<bool()> update,
			std::function<void()> quit) {
		if(valid && win) {
			glfwMakeContextCurrent(win);
			while(true) {
				glfwPollEvents();
				if(glfwWindowShouldClose(win)) {
					quit();
				} else if(update()) {
					redraw();
					continue;
				}
				break;
			}
		}
	}

	view::view(void) {
		using Header = Model::Ply::Header;
		using Element = Model::Ply::Element;

		// TODO - safe model and shader loading at runtime
		static constexpr const char 
			*vpath = "resources/shade.vert",
			*fpath = "resources/shade.frag",
			*mpath = "resources/bunny.ply";

		Header model(mpath);
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

		glGenVertexArrays(1, &vaID);
		glBindVertexArray(vaID);

		if(model.status) {
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
		}

		glGenBuffers(1, &vbuf);
		glBindBuffer(GL_ARRAY_BUFFER, vbuf);
		glBufferData(GL_ARRAY_BUFFER, vertices->data.size(),
				(void*)(&vertices->data[0]), GL_STATIC_DRAW);

		glGenBuffers(1, &ibuf);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->data.size(),
				(void*)(&indices->data[0]), GL_STATIC_DRAW);
		nTriangles = indices -> instances;
		
		progID = glCreateProgram();
		if(!link(vpath, fpath, progID)) {
			std::cout << "Could not compile/link shader(s)." << std::endl;
			return;
		}
		modelID = glGetUniformLocation(progID, "model");
		viewID = glGetUniformLocation(progID, "view");
		projID = glGetUniformLocation(progID, "proj");
		glUseProgram(progID);
		valid = true;
	}
	view::~view(void) {
		if(progID) {
			glDeleteProgram(progID);
		}
		glfwDestroyWindow(win);
	}
}
