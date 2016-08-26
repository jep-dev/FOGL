#ifndef VIEW_HPP
#define VIEW_HPP

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <atomic>
#include <functional>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "util.hpp"
#include "view/shade.hpp"
#include "view/pane.hpp"

namespace View {
	/// Loops over the queued GL errors and prints each (using GLU).
	void printErrors(const char *prefix);

	/// A view structure (state and implementation)
	struct view : public virtual Util::task {
		typedef enum {
			e_id_prog=0, e_id_va, 
			e_id_vbuf,   e_id_ibuf, 
			e_id_model,  e_id_view,
			e_id_proj,
			e_id_total
		} e_id_index; 
		GLuint ids[e_id_total];
		float near = 1, far = 10, fov = 25;
		int nTriangles;
		GLFWwindow *win;
		int frame = 0, fps = 0;
		float phi, theta;
	
		/// Sets stable shader values (uniforms)
		void setUniforms(void);

		/** Polls for updates; move to control?
 		 * @param alive Shared state; false signals shutdown
 		 */
		void poll(std::atomic_bool &alive);

		/*! Calculates and displays the next frame, potentially incorporating
 		 * the frame index (discrete time) and FPS
		 */
		void redraw();
		
		/** Begins the game loop with the specified callbacks
		 * @param alive Shared status flag; false signals shutdown
		 */
		void run(std::atomic_bool &alive) override;

		/** Initializes any resources not initialized in the constructor
 		 * @param alive Shared status flag; false signals shutdown
 		 */
		void init(std::atomic_bool &alive) override;

		/** Constructor for a view object
 		 * @param alive Shared status flag; false signals shutdown
		 * @param vert Path to a GLSL vertex shader
		 * @param frag Path to a GLSL fragment shader
		 */
		view(std::atomic_bool &alive, const char *vert, const char *frag);

		/// Destructor for a view object
		virtual ~view(void);
	};
}

#endif
