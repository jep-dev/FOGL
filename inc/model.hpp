#ifndef MODEL_HPP
#define MODEL_HPP

#include <iostream>
#include <vector>
#include <sstream>

#include "quat.hpp"
#include "dual.hpp"
#include "system.hpp"

namespace Model {
	using std::vector;
	using std::string;

	struct model {
		vector<float> vertices, normals;
		vector<unsigned int> indices;
	};
	enum PlyFormat {ASCII=0, LENDIAN, BENDIAN, UNDEF};

	bool ply(const char *fname, model &out); 
}

#endif
