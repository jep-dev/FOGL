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
	/*obj_t obj;
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
	}*/

	int w = 10, h = 10;
	/*mesh_t mesh(w, h, [](float s, float t, std::vector<float> &vertices) {
		auto theta = s*M_PI*2, phi = t*M_PI;
		vertices.emplace_back(cos(theta)*sin(phi));
		vertices.emplace_back(sin(theta)*sin(phi));
		vertices.emplace_back(cos(phi));
	});*/
	auto mesh = trimesh_t(w, h, [](float s, float t,
				std::vector<float> &vertices) {
		vertices.emplace_back(s);
		vertices.emplace_back(t);
		vertices.emplace_back(0);
	}).generate(w, h, [](float s, float t, std::vector<float> &vertices) {
		vertices.emplace_back(s+1);
		vertices.emplace_back(t+1);
		vertices.emplace_back(0);
	});

	graph g(h*w);
	for(int row = 0, index = 0; row < h; row++) {
		float t = row/float(h-1);
		for(int col = 0; col < w; col++, index++) {
			float s = col/float(w-1);
			float x[3]{s,t,0};
			g.nodes[index] = x;
			if(row > 0)
				node::connect(&g.nodes[index], &g.nodes[index-w]);
			if(col > 0)
				node::connect(&g.nodes[index], &g.nodes[index-1]);
			if(row > 0 && col > 0)
				node::connect(&g.nodes[index], &g.nodes[index-w-1]);
		}
	}
	mesh_t g_mesh;
	for(auto n : g.nodes) {
		g_mesh.vertices.emplace_back(n.x);
		g_mesh.vertices.emplace_back(n.y);
		g_mesh.vertices.emplace_back(n.z);
	}
	for(auto f : g.faces())
		g_mesh.faces.emplace_back(f);
	//std::cout << g_mesh << std::endl;
	std::ofstream file;
	file.open("share/sheet.obj");
	if(!file.is_open()) {
		std::cout << "Could not open file." << std::endl;
	} else {
		file << g_mesh;
		file.close();
	}

	//std::cout << mesh << std::endl;
}
