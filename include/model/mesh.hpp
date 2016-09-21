#ifndef MESH_HPP
#define MESH_HPP

namespace Model {
	struct mesh_t: model {
		friend std::ostream& operator<<(std::ostream& os, const mesh_t& mesh);
		virtual mesh_t& generate(int w, int h,
				void (*fn)(float s, float t, std::vector<float>&));
		mesh_t(int w, int h, void (*fn)
			(float s, float t, std::vector<float> &));
		mesh_t(void);
	};
	struct trimesh_t : mesh_t {
		trimesh_t& generate(int w, int h,
				void (*fn)(float s, float t, std::vector<float>&)) override;
		trimesh_t(int w, int h,
				void (*fn) (float s, float t, std::vector<float> &));
	};
	struct hexmesh_t : mesh_t {
		hexmesh_t& generate(int w, int h,
				void (*fn)(float s, float t, std::vector<float>&)) override;
		hexmesh_t(int w, int h,
				void (*fn) (float s, float t, std::vector<float> &));
	};
}

#endif
