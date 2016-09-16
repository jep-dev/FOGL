#include "view.hpp"

namespace View {
	// TODO Screen/compositor type?

	bool pane::init(std::atomic_bool &alive) {
		glGenFramebuffers(1, &ids[e_fbo]);
		glBindFramebuffer(GL_FRAMEBUFFER, ids[e_fbo]);

		auto gltex2D = GL_TEXTURE_2D;
		glGenTextures(1, &ids[e_tex]);
		glBindTexture(gltex2D, ids[e_tex]);
		glTexImage2D(gltex2D, 0, GL_RGB, ids[e_dx], ids[e_dy], 0,
				GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(gltex2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(gltex2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		auto glfb = GL_FRAMEBUFFER;
		auto glrb = GL_RENDERBUFFER;
		GLenum att0 [] {GL_COLOR_ATTACHMENT0};
		glGenRenderbuffers(1, &ids[e_zbo]);
		glBindRenderbuffer(glrb, ids[e_zbo]);
		glFramebufferRenderbuffer(glfb, GL_DEPTH_ATTACHMENT, glrb, ids[e_zbo]);
		glFramebufferTexture(glfb, att0[0], ids[e_tex], 0);
		glDrawBuffers(1, att0);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER)
				!= GL_FRAMEBUFFER_COMPLETE) {
			errors.emplace_back("Framebuffer status != complete");
			return alive = false;
		}

		// TODO Generate a quad, plus depth, stencil, z-buf?
		glGenVertexArrays(1, &ids[e_q_va]);
		glBindVertexArray(ids[e_q_va]);
		static constexpr const GLfloat points[] = {
			-1, 1, 0, 1, 1, 0, -1, -1, 0, // Up/left triangle
			-1, -1, 0, 1, 1, 0, 1, -1, 0  // Down/right triangle
		};
		glGenBuffers(1, &ids[e_q_vb]);
		glBindBuffer(GL_ARRAY_BUFFER, ids[e_q_vb]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points),
 				points, GL_STATIC_DRAW);
		static constexpr const GLint indices[] = {
			0, 1, 2, 3, 4, 5
		};
		glGenBuffers(1, &ids[e_fbo]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[e_fbo]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
				indices, GL_STATIC_DRAW);

		return alive;
	}
	bool pane::setProg(std::atomic_bool &alive,
			const char *vname, const char *fname) {
		ids[e_q_prog] = glCreateProgram();
		// TODO Shader paths as members by ctor/init
		if(!link(vname, fname, ids[e_q_prog], errors)) {
			return alive = false;
		}
		ids[e_q_tex] = glGetUniformLocation(ids[e_q_prog], "tex");
		glUseProgram(ids[e_q_prog]);
		return alive;
	}
	bool pane::poll(std::atomic_bool &alive) {
		return alive;
	}

	void pane::resize(int dx, int dy, int x0, int y0) {
		ids[e_dx] = dx;
		ids[e_dy] = dy;
		ids[e_x0] = x0;
		ids[e_y0] = y0;
		glViewport(x0, y0, x0 + dx, y0 + dy);
	}

	bool pane::run(std::atomic_bool &alive) {
		// TODO Render the quad to screen/compositor type
		return alive;
	}

	pane::pane(std::atomic_bool &alive):
		Util::task(), win(glfwCreateWindow(640, 480, "", NULL, NULL)) {}
}
