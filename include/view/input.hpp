#ifndef VIEW_INPUT_HPP
#define VIEW_INPUT_HPP

#include "view.hpp"
#include <GLFW/glfw3.h>

namespace View {
	namespace Input {
		enum e_cb_type : uint16_t{
			e_cb_char=0, e_cb_jmove, e_cb_kbd, e_cb_mcross,
			e_cb_mdrop, e_cb_mmove, e_cb_mpress, e_cb_mscroll
		};

		/// Generic callback type
		typedef void (*cb_t)();

		/// Keyboard event callback type
		typedef void (*cb_kbd)(GLFWwindow*, int, int, int, int);

		/// Character input event callback type
		typedef void (*cb_char)(GLFWwindow*, unsigned int);

		/// Mouse button press event callback type
		typedef void (*cb_mpress)(GLFWwindow*, unsigned int);

		/// Drag-and-drop event callback type
		typedef void (*cb_mdrop)(GLFWwindow*, int, const char**);

		/// Mouse movement event callback type
		typedef void (*cb_mmove)(GLFWwindow*, double, double);

		/// Mouse crossing (enter/exit) callback type
		typedef void (*cb_mcross)(GLFWwindow*, int);

		/// Scroll event callback type
		typedef void (*cb_mscroll)(GLFWwindow*, double, double);

		/// Joystick movement event callback type
		typedef void (*cb_jmove)(GLFWwindow*, int, int);

	}
}

#endif
