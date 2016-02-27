#include "view.hpp"
#include "shade.hpp"

#include "model.hpp"

namespace View {

	void printErrors(const char *prefix = "") {
		bool once = false;
		GLenum err;
		while((err = glGetError()) != GL_NO_ERROR) {
			if(!once) {
				std::cout << prefix;
				once = true;
			}
			switch(err) {
			case GL_INVALID_ENUM:
				std::cout << "invalid enum; ";
				break;
			case GL_INVALID_VALUE:
				std::cout << "invalid value; ";
				break;
			case GL_INVALID_OPERATION:
				std::cout << "invalid operation; ";
				break;
			default:
				break;
			}
		}
		 
		if(once) {
			endl(std::cout);
		}
	}

	void view::setUniforms(void) {
		int w, h;
		glfwGetFramebufferSize(win, &w, &h);

		float aspect = h/float(w),
			fov = float(1/tan(25 * M_PI / 180)),
			near = 1, far = 10,
			x = fov*aspect, y = fov, 
			z1 = (far+near)/(far-near),
			z2 = 2*far*near/(far-near),
			viewData[] = {
				 1,  0,  0,  0,
				 0,  1,  0,  0,
				 0,  0,  1,  0,
				 0,  0, -3,  1
			},
			projData[] = {
				 x,  0,  0,  0,
				 0,  y,  0,  0,
				 0,  0, z1, -1,
				 0,  0, z2,  0
			};
		
		static float theta = 0;
		theta += M_PI/180;
		float c = cos(theta), 
					s = sin(theta);
		float modelData[]{
				 c, 0,-s, 0,
				 0, 1, 0, 0,
				 s, 0, c, 0,
				 0, 0, 0, 1
		};

		glUniformMatrix4fv(modelID, 1, GL_TRUE,  modelData);
		glUniformMatrix4fv(viewID, 1, GL_FALSE, viewData);
		glUniformMatrix4fv(projID, 1, GL_FALSE, projData);
	}
	
	void view::redraw(void) {
		setUniforms();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		static long int offset = 3*sizeof(float), stride = 2*offset;
		glBindBuffer(GL_ARRAY_BUFFER, vbuf);
		glVertexAttribPointer(0, 3, GL_FLOAT, 
				GL_FALSE, stride, (void*) 0);
		glVertexAttribPointer(1, 3, GL_FLOAT,
				GL_FALSE, stride, (void*) offset);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
		glDrawElements(GL_TRIANGLES, nTriangles*3,
				GL_UNSIGNED_INT, (void*) 0);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(win);
	}

	void view::run(std::function<bool()> update,
			std::function<void()> quit) {
		if(valid && win) {
			glfwMakeContextCurrent(win);
			while(valid) {
				glfwPollEvents();
				if(!glfwWindowShouldClose(win)
						&& update()) {
					redraw();
					continue;
				}
				quit();
				break;
			}
		}
	}

	view::view(void) {
		using Header = Model::Ply::Header;
		using Element = Model::Ply::Element;

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
				[](GLFWwindow* win, int w, int h){
					glfwMakeContextCurrent(win);
					glViewport(0, 0, w, h);
				}
		);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glGenVertexArrays(1, &vaID);
		glBindVertexArray(vaID);

		Header model("./resources/bunny.ply");
		if(!model.status) {
			std::cout << model.status << std::endl;
			return;
		}
		auto start = begin(model.elements), 
				 stop = end(model.elements);
		auto vertices = std::find_if(start, stop,
			[](Element const& el) -> bool {
				return el.name == "vertex" 
					&& el.properties.size() == 6;
			}), indices = std::find_if(start, stop,
			[](Element const& el) -> bool {
				return el.name == "face"
					&& el.properties.size() == 1
					&& el.has_list == true;
			});
		if(vertices == stop || indices == stop) {
			valid = false;
			std::cout << "The model is valid, but does not match "
				"the anticipated structure." << std::endl;
			return;
		}

		int nVertices = vertices->instances,
				vSize = nVertices*6*sizeof(float),
				nIndices = indices->instances,
				iSize = nIndices*3*sizeof(int);
		nTriangles = nIndices;
		
		glGenBuffers(1, &vbuf);
		glBindBuffer(GL_ARRAY_BUFFER, vbuf);
		glBufferData(GL_ARRAY_BUFFER, vertices->data.size(),
				(void*)(&vertices->data[0]), GL_STATIC_DRAW);
		glGenBuffers(1, &ibuf);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->data.size(),
				(void*)(&indices->data[0]), GL_STATIC_DRAW);
		
		progID = glCreateProgram();
		const char *vName = "resources/shade.vert",
				*fName = "resources/shade.frag";
		if(!link(vName, fName, progID)) {
			std::cout << "Could not compile/link shader(s)." << std::endl;
			return;
		}
		modelID = glGetUniformLocation(progID, "model");
		viewID = glGetUniformLocation(progID, "view");
		projID = glGetUniformLocation(progID, "proj");
		glUseProgram(progID);
		/*std::cout << progID << ", " << modelID << ", " << viewID 
			<< ", " << projID << "." << std::endl;*/
		valid = true;
	}
	view::~view(void) {
		if(progID) {
			glDeleteProgram(progID);
		}
		glfwDestroyWindow(win);
	}
}
