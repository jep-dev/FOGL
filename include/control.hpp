#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <atomic>
#include <iostream>
#include <thread>

#include "util.hpp"
#include "model.hpp"
#include "view.hpp"

namespace Control {
	typedef enum : unsigned char {
		e_tid_view=0, ///< The view thread ID, necessarily 0 (master)
		e_tid_model,  ///< The model thread ID
		e_tid_total   ///< The total number of enumerated threads
	} e_tid; ///< The ID of a thread, where 0 is the master

	/// A data structure controlling both model and view
	struct control {
		Model::contour<Util::undef_t, GLfloat,
			GLclampf, GLclampf> model;
		void run(std::atomic_bool &alive,
				const char *vert, const char *frag);
	};

}


#endif
