#ifndef CONTROL_HPP
#define CONTROL_HPP

#include "model.hpp"
#include "view.hpp"

namespace Control {
	typedef enum : unsigned char {
		e_tid_view=0, ///< The view task ID, necessarily 0 (master)
		e_tid_model,  ///< The model task ID
		e_tid_total   ///< The total number of enumerated tasks
	} e_tid; ///< The ID of a task

	/// A data structure controlling both model and view
	struct control : public Util::task {
		std::vector<std::string> errors;
		const char *mpath;
		View::view viewer;
		//Model::contour<Util::undef_t, GLfloat, GLclampf, GLclampf> model;
		
		void init(std::atomic_bool &alive) override;
		void poll(std::atomic_bool &alive) override;
		void run(std::atomic_bool &alive) override;

		/** Constructor; initializes and applies shaders
 		 * @param alive Shared state; false signals shutdown
 		 * @param ply The path to a ply model
 		 * @param vert Path to a GLSL vertex shader
 		 * @param frag Path to a GLSL fragment shader
 		 */
		control(std::atomic_bool &alive, const char *ply,
 			const char *vert, const char *frag);
	};
}


#endif
