#ifndef VIEW_HPP
#define VIEW_HPP

#include <atomic>
#include <iostream>

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
		std::atomic_bool &alive;
		GLuint progID, vaID, vbuf, ibuf, 
			   transformID, projXYID, projZWID;
		GLFWwindow *win;
		
		static void resize(GLFWwindow *win, int w, int h);

		void project(int w, int h);
		void redraw(void);
		void run(void (*)(void));
		view(GLFWwindow *win,
				std::atomic_bool &alive);
		virtual ~view(void);
	};
}

#endif
