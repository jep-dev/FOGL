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
			std::cout << "Status: I/O Error\n" << std::endl;
			break;
		case obj_t::e_ok:
			std::cout << "Status: OK\n" << std::endl;
			break;
		case obj_t::e_err_unknown:
			std::cout << "Status: Unknown\n" << std::endl;
			break;
		default:
			std::cout << "(unknown status)\n" << std::endl;
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
			case obj_t::e_el_c: endl(std::cout << *cit++); break;
			case obj_t::e_el_f: endl(std::cout << *fit++); break;
			case obj_t::e_el_g: endl(std::cout << *git++); break;
			case obj_t::e_el_l: endl(std::cout << *lit++); break;
			case obj_t::e_el_mtllib: endl(std::cout << *mit++); break;
			case obj_t::e_el_o: endl(std::cout << *oit++); break;
			case obj_t::e_el_usemtl: endl(std::cout << *uit++); break;
			case obj_t::e_el_v: endl(std::cout << *vit++); break;
			case obj_t::e_el_vn: endl(std::cout << *vnit++); break;
			case obj_t::e_el_vp: endl(std::cout << *vpit++); break;
			default: break;
		}
	}
}
