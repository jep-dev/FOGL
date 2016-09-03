#ifndef MESH_HPP
#define MESH_HPP

namespace Model {
	struct mesh_t {
		void (*surf)(float, float, float*);
		void vertices(int w, int h, float *points);
		void faces(int w, int h, int *faces);
		mesh_t(void (*fn)(float s, float t, float *point));
	};
}

#endif
