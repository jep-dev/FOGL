#include "model/obj.hpp"

#include <iostream>
#include <ostream>
#include <sstream>
#include <type_traits>

int main(int argc, const char **argv) {
	using namespace Model;
	obj_t obj;
	auto fname = "share/test.obj";
	endl(std::cout << "File: " << fname);
	switch(obj_t::load(fname, obj)) {
		case obj_t::e_status_io:
			std::cout << "Status: I/O Error" << std::endl;
			break;
		case obj_t::e_status_ok:
			std::cout << "Status: OK" << std::endl;
			break;
		case obj_t::e_status_unknown:
			std::cout << "Status: Unknown" << std::endl;
			break;
		default:
			std::cout << "(unknown status)" << std::endl;
			break;
	}
	auto cit = std::begin(obj.comments);
	auto fit = std::begin(obj.faces);
	auto lit = std::begin(obj.lines);
	auto git = std::begin(obj.groups);
	auto vit = std::begin(obj.vertices);
	for(auto t : obj.types) {
		switch(t) {
			case obj_t::e_element_comment: {
				endl(std::cout << "Comment: " << (*cit++).contents);
			} break;
			case obj_t::e_element_face: {
				std::cout << "Face: ";
				for(auto iit : (*fit++).vertices) {
					std::cout << iit << " ";
				}
				endl(std::cout);
			} break;
			case obj_t::e_element_group: {
			} break;
			case obj_t::e_element_line: {
				std::cout << "Line: ";
				for(auto iit : (*lit++).vertices) {
					std::cout << iit << " ";
				}
				endl(std::cout);
			} break;
			case obj_t::e_element_vertex: {
				std::cout << "Vertex: ";
				for(auto iit : (*vit++).point) {
					std::cout << iit << " ";
				}
				endl(std::cout);
			} break;
			default: break;
		}
	}
}
