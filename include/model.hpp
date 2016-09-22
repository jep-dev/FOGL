#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>
#include <stdint.h>

namespace Model {
	struct model {
		void fan(std::vector<int> face);
		std::vector<int> faces;
		std::vector<float> vertices, normals, vertex_normals;
		std::vector<std::string> comments;
		virtual ~model(void);
	};
}
#include "model/obj.hpp"
#include "model/mesh.hpp"
#include "model/node.hpp"

#endif
