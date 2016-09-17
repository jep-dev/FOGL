#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>
#include <stdint.h>

namespace Model {
	struct model {
		std::vector<bool> bools;
		std::vector<int> ints;
		std::vector<float> floats;
		std::vector<std::string> strings;
		virtual ~model(void);
	};
}
#include "model/obj.hpp"
#include "model/mtl.hpp"
#include "model/mesh.hpp"

#endif
