#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>
#include <stdint.h>

namespace Model {
	struct model {
		std::vector<int> faces;
		std::vector<float> vertices, normals;
		std::vector<std::string> comments;
		virtual ~model(void);
	};
}
#include "model/obj.hpp"
#include "model/mesh.hpp"

#endif
