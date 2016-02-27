#ifndef VIEW_HPP
#define VIEW_HPP

#include <atomic>
#include <iostream>
#include <functional>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#include <GL/gl3w.h>
#include <GL/glcorearb.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

namespace View {
	struct view {
		bool valid = false;
		GLuint progID, vaID, vbuf, ibuf,
					 modelID, viewID, projID;
		int nTriangles;
		GLFWwindow *win;
	
		void setUniforms(void);
		void redraw(void);
		void run(std::function<bool()>, std::function<void()>);
		view();
		virtual ~view(void);

		//static void resize(GLFWwindow *win, int w, int h)

	};
}

#endif
