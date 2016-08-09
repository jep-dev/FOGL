#pragma once
#ifndef VIEW_HPP
#define VIEW_HPP

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <functional>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "view/input.hpp"
#include "view/shade.hpp"

//template<typename T> struct std::function;
namespace View {
	void printErrors(const char *prefix);
	/// A view structure (state and implementation)
	struct view {
		typedef enum {
			prog_id=0, va_id, 
			vbuf_id, ibuf_id, 
			model_id, view_id,
			proj_id,
			_max
		} id_index; 
		GLuint ids[id_index::_max];

		bool valid = false;
		float near = 1, far = 10, fov = 25;
		int nTriangles;
		GLFWwindow *win;
	
		/// Sets stable shader values (uniforms)
		void setUniforms(void);
		/*! Calculates and displays the next frame, potentially incorporating
 		 * the frame index (discrete time) and FPS
		 * @param frame The discrete time (frames since startup)
		 * @param fps The most-recently calculated frames per second
		 */
		void redraw(int frame = 0, int fps = 0);
		
		/** Begins the game loop with the specified callbacks
		 * @param update Updates the model; false signals shutdown
		 * @param quit Destroys the model and managed resources
		 */
		void run(std::function<bool()> update,
			std::function<void()> quit);
		
		/** Constructor for a view object
		 * @param vert Path to a GLSL vertex shader
		 * @param frag Path to a GLSL fragment shader
		 */
		view(const char *vert, const char *frag);

		/// Destructor for a view object
		virtual ~view(void);
	};
}

#endif
