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

	void view::resize(GLFWwindow *win, int w, int h) {
		glViewport(0, 0, w, h);
	}

	void view::project(int w, int h) {

		const float aspect = h/float(w),
			fov = float(1/tan(45 * M_PI / 180)),
			near = 1.0, far = 5.0,
			x = fov*aspect, y = fov, 
			z1 = (far+near)/(far-near),
			z2 = 2*far*near/(far-near),
			t[3]{0,-0.1,-2},
			transformData[] = {
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				t[0], t[1], t[2], 1.0
			}, 
			xyData[] = { x,	 0.0, 0.0,	 y},
			zwData[] = {z1, -1.0,	 z2, 0.0};

		glUniformMatrix4fv(transformID, 1,
				GL_FALSE, transformData);
		glUniformMatrix2fv(projXYID, 1,
				GL_FALSE, xyData);
		glUniformMatrix2fv(projZWID, 1,
				GL_FALSE, zwData);
	}
	
	void view::redraw(void) {
		int w, h;
		glfwGetFramebufferSize(win, &w, &h);
		project(w, h);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbuf);
		glVertexAttribPointer(0, 3, GL_FLOAT, 
				GL_FALSE, 0, (void*) 0);

		glBindBuffer(GL_ARRAY_BUFFER, nbuf);
		glVertexAttribPointer(1, 3, GL_FLOAT,
				GL_FALSE, 0, (void*) 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
		glDrawElements(GL_TRIANGLES, nTriangles*3,
				GL_UNSIGNED_INT, (void*) 0);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(win);
	}

	void view::run(void (*update)(void)) {
		if(!valid) {
			return;
		}

		glfwMakeContextCurrent(win);

		while(alive) {
			glfwPollEvents();
			if(glfwWindowShouldClose(win)) {
				alive = false;
				break;
			}
			if(alive) {
				update();
				redraw();
			}
		}
	}

	view::view(GLFWwindow *win,
			std::atomic_bool &alive):
		win(win), alive(alive) {

		glfwSetWindowSizeCallback(win, resize);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glGenVertexArrays(1, &vaID);
		glBindVertexArray(vaID);

		Model::Ply::Header model("../models/bunny3.ply");
		if(!model.status) {
			alive = false;
			valid = false;
			std::cout << model.status << std::endl;
			return;
		}

		int f3 = sizeof(float)*3, i3 = sizeof(int)*3;
		auto &vertices = model.elements[0], 
				 &indices = model.elements[1];
		int nVertices = vertices.instances, 
				verticesSize = nVertices * f3,
				nIndices = indices.instances,
				indicesSize = nIndices * i3;
		nTriangles = nIndices;

		float vbufData[nVertices*3],
					nbufData[nVertices*3];
		for(int i = 0, j = 0; i < nVertices; ++i, j+=3) {
			int pos = vertices.properties[0].indices[i];
			std::memcpy(&vbufData[j], &vertices.data[pos], f3);
			std::memcpy(&nbufData[j], &vertices.data[pos+f3], f3);
		}
		int ibufData[nIndices*3];
		std::memcpy(ibufData, &indices.data[0], i3*nIndices);
		
		glGenBuffers(1, &vbuf);
		glBindBuffer(GL_ARRAY_BUFFER, vbuf);
		glBufferData(GL_ARRAY_BUFFER, verticesSize, 
				vbufData, GL_STATIC_DRAW);

		glGenBuffers(1, &nbuf);
		glBindBuffer(GL_ARRAY_BUFFER, nbuf);
		glBufferData(GL_ARRAY_BUFFER, verticesSize,
				nbufData, GL_STATIC_DRAW);

		glGenBuffers(1, &ibuf);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, 
				ibufData, GL_STATIC_DRAW);

		progID = glCreateProgram();
		const char *vName = "resources/shade.vert",
				*fName = "resources/shade.frag";
		if(!link(vName, fName, progID)) {
			std::cout << "Could not compile/link shader(s)." << std::endl;
			return;
		}
		transformID = glGetUniformLocation(progID, "transform");
		projXYID = glGetUniformLocation(progID, "projXY");
		projZWID = glGetUniformLocation(progID, "projZW");
		glUseProgram(progID);
		valid = true;
	}
	view::~view(void) {
		if(progID != 0) {
			glDeleteProgram(progID);
		}
		glfwDestroyWindow(win);
	}
}
