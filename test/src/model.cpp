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
		case obj_t::e_err_io:
			std::cout << "Status: I/O Error" << std::endl;
			break;
		case obj_t::e_ok:
			std::cout << "Status: OK" << std::endl;
			break;
		case obj_t::e_err_unknown:
			std::cout << "Status: Unknown" << std::endl;
			break;
		default:
			std::cout << "(unknown status)" << std::endl;
			break;
	}
	auto cit = std::begin(obj.comments);
	auto fit = std::begin(obj.faces);
	auto git = std::begin(obj.groups);
	auto lit = std::begin(obj.lines);
	auto mit = std::begin(obj.mtllibs);
	auto oit = std::begin(obj.objects);
	auto uit = std::begin(obj.usemtls);
	auto vit = std::begin(obj.vertices);
	auto vnit = std::begin(obj.norms);
	auto vpit = std::begin(obj.params);
	for(auto t : obj.types) {
		switch(t) {
			case obj_t::e_el_c: {
				std::cout << *cit++ << std::endl;
			} break;
			case obj_t::e_el_f: {
				std::cout << *fit++ << std::endl;
			} break;
			case obj_t::e_el_g: {
				std::cout << *git++ << std::endl;
			} break;
			case obj_t::e_el_l: {
				std::cout << "Line: ";
				for(auto iit : (*lit++).vertices) {
					std::cout << iit << " ";
				}
				endl(std::cout);
			} break;
			case obj_t::e_el_mtllib: {
				std::cout << "Mtllib: " << mit++->path << std::endl;
			} break;
			case obj_t::e_el_o: {
				std::cout << "Object: " << (*oit++).name << std::endl;
			} break;
			case obj_t::e_el_usemtl: {
				std::cout << "Usemtl: " << uit++->name << std::endl;
			} break;
			case obj_t::e_el_v: {
				std::cout << "Vertex: ";
				for(auto iit : (*vit++).point) {
					std::cout << iit << " ";
				}
				endl(std::cout);
			} break;
			case obj_t::e_el_vn: {
				std::cout << "Vertex norm: ";
				for(auto iit : (*vnit++).point) {
					std::cout << iit << " ";
				}
				endl(std::cout);
			} break;
			case obj_t::e_el_vp: {
				std::cout << "Vertex params: ";
				for(auto iit : (*vpit++).point) {
					std::cout << iit << " ";
				}
				endl(std::cout);
			} break;
			default: break;
		}
	}
}
