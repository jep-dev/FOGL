#include "model.hpp"

namespace Model {
	void mesh_t::vertices(int w, int h, float *points) {
		for(int index = 0, max_index = w*h; index < max_index; index++) {
			int y = index % w, x = index / w;
			surf(float(x)/w, float(y)/h, &points[index*3]);
		}
	}
	void mesh_t::faces(int w, int h, int *faces) {
		int index = 0, vindex = 0;
		for(int i = 0; i < w; i++) {
			for(int j = 0; j < h; j++) {
				auto vIndex = j*w+i;
				faces[index++] = vIndex;
				faces[index++] = vIndex + 1;
				faces[index++] = vIndex + w + 1;
				faces[index++] = vIndex;
				faces[index++] = vIndex + w + 1;
				faces[index++] = vIndex + w;
			}
		}
	}
	mesh_t::mesh_t(void (*fn)(float s, float t, float *point)):
		surf(fn) {}
}
