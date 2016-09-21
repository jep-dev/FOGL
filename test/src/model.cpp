#include "model/obj.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <type_traits>

#ifndef OBJ_PATH
#define OBJ_PATH "share/cube.obj"
#endif
/*#ifndef MTL_PATH
#define MTL_PATH "share/cube.mtl"
#endif*/

int main(int argc, const char **argv) {
	using namespace Model;
	const char *obj_fname, *mtl_fname;
	if(argc >= 2) {
		obj_fname = argv[1];
	} else {
		obj_fname = OBJ_PATH;
	}
	obj_t obj;
	endl(std::cout << "File: " << obj_fname);
	switch(obj_t::load(obj_fname, obj)) {
		case obj_t::e_err_io:
			std::cout << "Status: I/O Error\n" << std::endl;
			break;
		case obj_t::e_ok:
			std::cout << "Status: OK\n" << std::endl;
			std::cout << obj << std::endl;
			std::cout << obj.faces.size () << ", "
				<< obj.vertices.size() << ", "
				<< obj.normals.size() << "." << std::endl;
			break;
		case obj_t::e_err_unknown:
			std::cout << "Status: Unknown\n" << std::endl;
			break;
		default:
			break;
	}

	int w = 1, h = 1;
	/*mesh_t mesh(w, h, [](float s, float t, std::vector<float> &vertices) {
		auto theta = s*M_PI*2, phi = t*M_PI;
		vertices.emplace_back(cos(theta)*sin(phi));
		vertices.emplace_back(sin(theta)*sin(phi));
		vertices.emplace_back(cos(phi));
	});*/
	auto mesh = trimesh_t(w, h, [](float s, float t, std::vector<float> &vertices) {
		vertices.emplace_back(s);
		vertices.emplace_back(t);
		vertices.emplace_back(0);
	}).generate(w, h, [](float s, float t, std::vector<float> &vertices) {
		vertices.emplace_back(s+1);
		vertices.emplace_back(t+1);
		vertices.emplace_back(0);
	});
	//std::cout << mesh << std::endl;
	/*std::ofstream file;
	file.open("share/sphere.obj");
	if(!file.is_open()) {
		std::cout << "Could not open file." << std::endl;
	} else {
		file << mesh;
		file.close();
	}*/
}
