#ifndef VIEW_HPP
#define VIEW_HPP

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

//#include <atomic>
//#include <iostream>
#include <iosfwd>
#include <functional>

#include <GL/gl3w.h>
#include <GL/glcorearb.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

namespace View {
	void printErrors(const char *prefix);
	struct view {
		bool valid = false;
		float near = 1, far = 10, fov = 25;
		GLuint progID, vaID, vbuf, ibuf,
					 modelID, viewID, projID;
		int nTriangles;
		GLFWwindow *win;
	
		void setUniforms(void);
		void redraw(void);
		void run(std::function<bool()>, std::function<void()>);
		view(const char *vert, const char *frag);
		virtual ~view(void);
	};
}

#endif
