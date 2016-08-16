#ifndef VIEW_INPUT_HPP
#define VIEW_INPUT_HPP

#include "view.hpp"

namespace View {
	namespace Input {
		enum e_cb_type : uint16_t{
			e_cb_char=0, e_cb_jmove, e_cb_kbd, e_cb_mcross,
			e_cb_mdrop, e_cb_mmove, e_cb_mpress, e_cb_mscroll
		};
	}
}

#endif
