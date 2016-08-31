#include "model/obj.hpp"

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
		int nf = obj.nFloats[counter], ni = obj.nInts[counter],
				ns = obj.nStrings[counter];
		std::cout << *beg << " ";
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
}
