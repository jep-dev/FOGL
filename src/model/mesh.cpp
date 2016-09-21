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
		auto offset = vertices.size()/3;
		for(int i = 0; i < w; i++) {
			for(int j = 0; j < h; j++) {
				auto index = j * w + i + offset;
				fn(float(i)/(w-1)*2-1, float(j)/(h-1)*2-1, vertices);
				if(i < w-1 && j < h-1) {
					faces.emplace_back(index);
					faces.emplace_back(index+1);
					faces.emplace_back(index+w+1);
					faces.emplace_back(index);
					faces.emplace_back(index+w+1);
					faces.emplace_back(index+w);
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
		auto index = vertices.size()/3;
		for(int row = 0; row <= h; row++) {
			for(int col = 0; col <= row; col++, index++) {
				float r = row/float(h),
					  s = row ? (col*2-row)*r/row : 0, t = (1-r)*2-1;
				fn(s, t, vertices);
				if(col > 0) {
					faces.emplace_back(index);
					faces.emplace_back(index-row-1);
					faces.emplace_back(index-1);
					if(col < row) {
						faces.emplace_back(index);
						faces.emplace_back(index-row);
						faces.emplace_back(index-row-1);
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
		auto index = vertices.size()/3;
		float height = sqrt(3)/3;
		for(int row = 0; row <= h; row++) {
			for(int col = 0; col <= row; col++, index++) {
				float r = row/float(h),
					  s = row ? (col*2-row)*r/row : 0, t = 1-r;
				fn(s, t, vertices);
				if(col > 0 && t < height) {
					faces.emplace_back(index);
					faces.emplace_back(index-row-1);
					faces.emplace_back(index-1);
					if(col < row) {
						faces.emplace_back(index);
						faces.emplace_back(index-row);
						faces.emplace_back(index-row-1);
					}
				}
			}
		}
		for(int row = 0; row <= h; row++) {
			for(int col = 0; col <= row; col++, index++) {
				float r = row/float(h),
					  s = row ? (col*2-row)*r/row : 0, t = 1-r;
				fn(-s, -t, vertices);
				if(col > 0 && t < height) {
					faces.emplace_back(index);
					faces.emplace_back(index-row-1);
					faces.emplace_back(index-1);
					if(col < row) {
						faces.emplace_back(index);
						faces.emplace_back(index-row);
						faces.emplace_back(index-row-1);
					}
				}
			}
		}
		return *this;
	}
	std::ostream& operator<<(std::ostream& os, const mesh_t& mesh) {
		int i = 0;
		for(auto v : mesh.vertices) {
			if(i == 0) os << "v ";
			os << v << ' ';
			i++;
			if(i == 3) {
				i = 0;
				os << '\n';
			}
		}
		for(auto f : mesh.faces) {
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
