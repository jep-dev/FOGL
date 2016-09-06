#include "model.hpp"

namespace Model {
	mesh_t::mesh_t(int w, int h, void (*fn)
		(float s, float t, std::vector<float> &vertices)) {
		for(int i = 0; i < w; i++) {
			for(int j = 0; j < h; j++) {
				auto index = 3*(j * w + i), i2 = 3, j2 = w*3;
				fn(float(i)/w, float(j)/h, vertices);
				if(i < w-1 && j < h-1) {
					faces.emplace_back(index);
					faces.emplace_back(index+i2);
					faces.emplace_back(index+i2+j2);
					faces.emplace_back(index);
					faces.emplace_back(index+i2+j2);
					faces.emplace_back(index+j2);
				}
			}
		}
	}
}
