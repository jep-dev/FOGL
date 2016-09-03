#include "model/obj.hpp"

#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <type_traits>

#ifndef OBJ_PATH
#define OBJ_PATH "share/cube.obj"
#endif
#ifndef MTL_PATH
#define MTL_PATH "share/cube.mtl"
#endif

int main(int argc, const char **argv) {
	using namespace Model;
	const char *obj_fname, *mtl_fname;
	if(argc >= 3) {
		mtl_fname = argv[2];
	} else {
		mtl_fname = MTL_PATH;
	}
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
	/*std::cout << "Contiguous data: " << std::endl;
	endl(std::cout << "Bools:" << obj.bools.size());
	for(auto it : obj.bools) std::cout << it << " ";
	endl(std::cout << "\nFloats:" << obj.floats.size());
	for(auto it : obj.floats) std::cout << it << " ";
	endl(std::cout << "\nIntegers:" << obj.ints.size());
	for(auto it : obj.ints) std::cout << it << " ";
	endl(std::cout << "\nStrings:" << obj.strings.size());
	for(auto it : obj.strings) std::cout << it << " ";
	endl(std::cout);*/

	/*auto f_it = std::begin(obj.floats);
	auto i_it = std::begin(obj.ints);
	auto nf_it = std::begin(obj.nFloats);
	auto ni_it = std::begin(obj.nInts);
	for(auto t_it = std::begin(obj.types);
			t_it != std::end(obj.types); t_it++) {
		bool vmatch = *t_it == e_el_v,
			 vnmatch = *t_it == e_el_vn,
			 f2match = *t_it == e_el_f2;
		if(*nf_it > 0) {
			if(vmatch) {
				std::cout << "\nVertex: ";
			} else if(vnmatch) {
				std::cout << "\nVertex norm: ";
			}
			for(int i = 0, iMax = *nf_it; i < iMax; ++i) {
				if(vmatch || vnmatch) {
					std::cout << *f_it << " ";
				}
				f_it++;
			}
		}
		if(*ni_it > 0) {
			if(f2match) {
				std::cout << "\nFace: ";
			}
			for(int i = 0, iMax = *ni_it; i < iMax; ++i) {
				if(f2match) {
					std::cout << *i_it << " ";
				}
				i_it++;
			}
		}
		nf_it++;
		ni_it++;
	}
	std::cout << "\n\n";*/

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
	}

	material_t mtl;
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
	}

	mesh_t mesh([](float u, float v, float* uv) {
		uv[0] = u;
		uv[1] = v;
		uv[2] = 0;
	});

	int w = 2, h = 2, size = w*h*3;
	float mesh_points[size];
	int mesh_faces[size*2];
	mesh.vertices(w, h, mesh_points);
	mesh.faces(w, h, mesh_faces);
	std::cout << "\nMesh points: ";
	for(auto f : mesh_points) {
		std::cout << f << ' ';
	}
	std::cout << "\nMesh faces:";
	for(auto i : mesh_faces) {
		std::cout << i << ' ';
	}
	endl(std::cout);
}
