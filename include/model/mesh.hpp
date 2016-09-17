#ifndef MESH_HPP
#define MESH_HPP

namespace Model {
	struct mesh_t: model {
		friend std::ostream& operator<<(std::ostream& os, const mesh_t& mesh);
		mesh_t(int w, int h, void (*fn)
			(float s, float t, std::vector<float> &));
	};
}

#endif
