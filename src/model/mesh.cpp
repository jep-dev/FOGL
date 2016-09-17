#include "model.hpp"
#include <iostream>
#include <string>

namespace Model {
	mesh_t::mesh_t(int w, int h, void (*fn)
		(float s, float t, std::vector<float> &vertices)) {
		for(int i = 0; i < w; i++) {
			for(int j = 0; j < h; j++) {
				auto index = j * w + i;
				fn(float(i)/(w-1), float(j)/(h-1), floats);
				if(i < w-1 && j < h-1) {
					ints.emplace_back(index);
					ints.emplace_back(index+1);
					ints.emplace_back(index+w+1);
					ints.emplace_back(index);
					ints.emplace_back(index+w+1);
					ints.emplace_back(index+w);
				}
			}
		}
	}
	std::ostream& operator<<(std::ostream& os, const mesh_t& mesh) {
		int i = 0;
		for(auto v : mesh.floats) {
			if(i == 0) os << "v ";
			os << v << ' ';
			i++;
			if(i == 3) {
				i = 0;
				os << '\n';
			}
		}
		for(auto f : mesh.ints) {
			if(i == 0) os << "f ";
			os << f+1 << ' ';
			i++;
			if(i == 3) {
				i = 0;
				os << '\n';
			}
		}
		return os;
	}
}
