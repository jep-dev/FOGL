#include "model/obj.hpp"

#include <iostream>
#include <ostream>
#include <sstream>
#include <type_traits>

int main(int argc, const char **argv) {
	using namespace Model;
	std::cout << "Testing" << std::endl;
	obj_t obj;
	switch(obj_t::load("share/test.obj", obj)) {
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
	for(auto t : obj.types) {
		switch(t) {
			case obj_t::e_element_comment: {
				std::cout << "Comment: " << (*cit++).contents << std::endl;
			} break;
			case obj_t::e_element_face: {
				std::cout << "Face: ";
				for(auto iit : (*fit++).vertices) {
					std::cout << iit << " ";
				}
				std::cout << std::endl;
			} break;
			default: break;
		}
	}
}
