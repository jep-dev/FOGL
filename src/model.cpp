#include "model.hpp"

namespace Model {
	model::~model(void) {}
	void model::fan(std::vector<int> face) {
		int v0 = face[0];
		for(int i = 1; i < face.size(); i++) {
			faces.emplace_back(v0);
			faces.emplace_back(face[i-1]);
			faces.emplace_back(face[i]);
		}
	}
}
