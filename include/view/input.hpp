#ifndef VIEW_INPUT_HPP
#define VIEW_INPUT_HPP

#include "view.hpp"

namespace View {
	namespace Input {
		/// Keyboard event callback type
		typedef void (*ecb_kbd)(GLFWwindow*, int, int, int, int);

		/// Character input event callback type
		typedef void (*ecb_char)(GLFWwindow*, unsigned int);

		/// Mouse button press event callback type
		typedef void (*ecb_mpress)(GLFWwindow*, unsigned int);

		/// Drag-and-drop event callback type
		typedef void (*ecb_mdrop)(GLFWwindow*, int, const char**);

		/// Mouse movement event callback type
		typedef void (*ecb_mmove)(GLFWwindow*, double, double);

		/// Mouse crossing (enter/exit) callback type
		typedef void (*ecb_mcross)(GLFWwindow*, int);

		/// Scroll event callback type
		typedef void (*ecb_mscroll)(GLFWwindow*, double, double);

		/// Joystick movement event callback type
		typedef void (*ecb_jmove)(GLFWwindow*, int, int);
	}
}

#endif
