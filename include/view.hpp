#ifndef VIEW_HPP
#define VIEW_HPP

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <vector>
#include <string>

#include <GL/gl3w.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "util.hpp"
#include "view/shade.hpp"

namespace View {
	/// A view structure (state and implementation)
	struct view : public virtual Util::task {
		typedef enum {
			e_id_prog=0, e_id_va, 
			e_id_vbuf,   e_id_vnbuf, e_id_fbuf,
			e_id_model,  e_id_view,  e_id_proj,
			e_id_total
		} e_id_index; 
		GLuint ids[e_id_total];
		const char *vert_fname, *frag_fname;
		float near = 1, far = 20, fov = 30;
		int nTriangles;
		GLFWwindow *win;
		int frame = 0, fps = 0;
		float theta, phi, x, y, z;
		std::vector<std::string> errors;
	
		/// Sets stable shader values (uniforms)
		void setUniforms(void);

		/** Polls for updates; move to control?
 		 * @param alive Shared state; false signals shutdown
 		 */
		bool poll(std::atomic_bool &alive) override;

		/*! Calculates and displays the next frame, potentially incorporating
 		 * the frame index (discrete time) and FPS
		 */
		void redraw();
		
		/** Begins the game loop with the specified callbacks
		 * @param alive Shared status flag; false signals shutdown
		 */
		bool run(std::atomic_bool &alive) override;

		/** Initializes any resources not initialized in the constructor
 		 * @param alive Shared status flag; false signals shutdown
 		 */
		bool init(std::atomic_bool &alive) override;

		/** Constructor for a view object
 		 * @param alive Shared status flag; false signals shutdown
		 * @param vert_fname Path to a GLSL vertex shader
		 * @param frag_fname Path to a GLSL fragment shader
		 */
		view(std::atomic_bool &alive, const char *vname, const char *fname);

		/// Destructor for a view object
		virtual ~view(void);
	};
}

#endif
