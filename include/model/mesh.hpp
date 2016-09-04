#ifndef MESH_HPP
#define MESH_HPP

namespace Model {
	struct mesh_t {
		std::vector<float> vertices;
		std::vector<int> faces;
		mesh_t(int w, int h, void (*fn)
			(float s, float t, std::vector<float> &));
	};
}

#endif
