#pragma once
#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <atomic>
#include "model.hpp"
#include "view.hpp"

namespace Control {
	/// A data structure controlling both model and view
	struct control {
		std::atomic_bool alive;
		View::view viewer;
		Model::contour<Util::undef_t, GLfloat,
			GLclampf, GLclampf> model;
	};
}


#endif
