#include "model.hpp"
#include <iostream>
#include <string>
#include <cmath>

namespace Model {
	mesh_t::mesh_t(void) {}
	mesh_t::mesh_t(int w, int h,
			void (*fn) (float s, float t, std::vector<float> &vertices)) {
		generate(w, h, fn);
	}
	mesh_t& mesh_t::generate(int w, int h,
			void (*fn) (float s, float t, std::vector<float> &vertices)) {
		auto offset = floats.size()/3;
		for(int i = 0; i < w; i++) {
			for(int j = 0; j < h; j++) {
				auto index = j * w + i + offset;
				fn(float(i)/(w-1)*2-1, float(j)/(h-1)*2-1, floats);
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
		return *this;
	}
	trimesh_t::trimesh_t(int w, int h,
			void (*fn) (float s, float t, std::vector<float> &vertices)):
			mesh_t() {
		generate(w, h, fn);
	}
	trimesh_t& trimesh_t::generate(int w, int h,
			void (*fn) (float s, float t, std::vector<float> &vertices)) {
		auto index = floats.size()/3;
		for(int row = 0; row <= h; row++) {
			for(int col = 0; col <= row; col++, index++) {
				float r = row/float(h),
					  s = row ? (col*2-row)*r/row : 0, t = (1-r)*2-1;
				fn(s, t, floats);
				if(col > 0) {
					ints.emplace_back(index);
					ints.emplace_back(index-row-1);
					ints.emplace_back(index-1);
					if(col < row) {
						ints.emplace_back(index);
						ints.emplace_back(index-row);
						ints.emplace_back(index-row-1);
					}
				}
			}
		}
		return *this;
	}
	hexmesh_t::hexmesh_t(int w, int h,
			void (*fn) (float s, float t, std::vector<float> &vertices)):
			mesh_t() {
		generate(w, h, fn);
	}
	hexmesh_t& hexmesh_t::generate(int w, int h,
			void (*fn) (float s, float t, std::vector<float> &vertices)) {
		auto index = floats.size()/3;
		float height = sqrt(3)/3;
		for(int row = 0; row <= h; row++) {
			for(int col = 0; col <= row; col++, index++) {
				float r = row/float(h),
					  s = row ? (col*2-row)*r/row : 0, t = 1-r;
				fn(s, t, floats);
				if(col > 0 && t < height) {
					ints.emplace_back(index);
					ints.emplace_back(index-row-1);
					ints.emplace_back(index-1);
					if(col < row) {
						ints.emplace_back(index);
						ints.emplace_back(index-row);
						ints.emplace_back(index-row-1);
					}
				}
			}
		}
		for(int row = 0; row <= h; row++) {
			for(int col = 0; col <= row; col++, index++) {
				float r = row/float(h),
					  s = row ? (col*2-row)*r/row : 0, t = 1-r;
				fn(-s, -t, floats);
				if(col > 0 && t < height) {
					ints.emplace_back(index);
					ints.emplace_back(index-row-1);
					ints.emplace_back(index-1);
					if(col < row) {
						ints.emplace_back(index);
						ints.emplace_back(index-row);
						ints.emplace_back(index-row-1);
					}
				}
			}
		}
		return *this;
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
