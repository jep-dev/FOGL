#include "model/obj.hpp"

#include <iostream>
#include <ostream>
#include <sstream>
#include <type_traits>

#ifndef OBJ_PATH
#define OBJ_PATH "share/icosahedron.obj"
#endif

int main(int argc, const char **argv) {
	using namespace Model;
	const char *fname;
	if(argc == 2) {
		fname = argv[1];
	} else {
		fname = OBJ_PATH;
	}
	obj_t obj;
	endl(std::cout << "File: " << fname);
	switch(obj_t::load(fname, obj)) {
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
	endl(std::cout << "Floats:" << obj.floats.size());
	for(auto it : obj.floats) std::cout << it << " ";
	endl(std::cout << "\nIntegers:" << obj.ints.size());
	for(auto it : obj.ints) std::cout << it << " ";
	endl(std::cout << "\nStrings:" << obj.strings.size());
	for(auto it : obj.strings) std::cout << it << " ";
	endl(std::cout);*/
}
