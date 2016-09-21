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
			break;
		case obj_t::e_err_unknown:
			std::cout << "Status: Unknown\n" << std::endl;
			break;
		default:
			break;
	}
	int counter = 0, fCounter = 0, iCounter = 0, sCounter = 0;
	for(auto beg = std::begin(obj.types); beg != std::end(obj.types); beg++) {
		int nb = obj.nBools[counter], nf = obj.nFloats[counter],
			ni = obj.nInts[counter], ns = obj.nStrings[counter];
		std::cout << std::setw(4) << counter << ". " << *beg << " ";
		for(int i = 0; i < nb; ++i) {
			std::cout << obj.bools[fCounter++] << " ";
		}
		for(int i = 0; i < nf; ++i) {
			std::cout << obj.floats[fCounter++] << " ";
		}
		for(int i = 0; i < ni; ++i) {
			std::cout << obj.ints[iCounter++] << " ";
		}
		for(int i = 0; i < ns; ++i) {
			std::cout << obj.strings[sCounter++] << " ";
		}
		counter++;
		endl(std::cout);
	}

	for(int i = 0; i < 7; i++) {
		std::vector<int> begs, ends;
		switch(i) {
			case 0: begs = obj.v_beg; ends = obj.v_end;
				std::cout << "Vertex range: " << std::endl;
				break;
			case 1: begs = obj.vn_beg; ends = obj.vn_end;
				std::cout << "Normal range: " << std::endl;
				break;
			case 2: begs = obj.vp_beg; ends = obj.vp_end;
				std::cout << "Texture range: " << std::endl;
				break;
			case 3: begs = obj.f0_beg; ends = obj.f0_end;
				std::cout << "Face(0) range: " << std::endl;
				break;
			case 4: begs = obj.f1_beg; ends = obj.f1_end;
				std::cout << "Face(1) range: " << std::endl;
				break;
			case 5: begs = obj.f2_beg; ends = obj.f2_end;
				std::cout << "Face(2) range: " << std::endl;
				break;
			case 6: begs = obj.f3_beg; ends = obj.f3_end;
				std::cout << "Face(3) range: " << std::endl;
				break;
			default: break;
		}
		auto beg_start = std::begin(begs), beg_stop = std::end(begs),
			 end_start = std::begin(ends), end_stop = std::end(ends);
		while(beg_start != beg_stop) {
			std::cout << "{" << *beg_start++ << ", "
				<< *end_start++ << "}" << std::endl;
		}
	}*/

	/*material_t mtl;
	std::cout << "\nFile: " << mtl_fname << std::endl;
	switch(mtl.load(mtl_fname, mtl)) {
		case material_t::e_ok: {
			std::cout << "Status: OK" << std::endl;
		} break;
		case material_t::e_err_io: {
			std::cout << "Status: I/O error" << std::endl;
		} break;
		case material_t::e_err_unknown: {
			std::cout << "Status: Unknown" << std::endl;
		} break;
		default: {} break;
	}*/

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
	/*std::ofstream file;
	file.open("share/sphere.obj");
	if(!file.is_open()) {
		std::cout << "Could not open file." << std::endl;
	} else {
		file << mesh;
		file.close();
	}*/
	std::cout << mesh << std::endl;
}
