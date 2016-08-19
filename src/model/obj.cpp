#include "model/obj.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>

namespace Model {
	obj_t::e_obj_status obj_t::load(const char *fname,
			std::vector<element_t> &elements) {
		std::ifstream file;
		file.open(fname, std::ios::in);
		if(!file.is_open()) {
			return e_status_io;
		}
		e_obj_element element;
		e_obj_status status = e_status_unknown;
		for(std::string line; std::getline(file, line);) {
			boost::tokenizer<> tk(line);
			bool flip = true;
			for(auto it = std::begin(tk); it != std::end(tk); ++it) {
				auto word = *it;
				if(flip) {
					flip = false;
					for(uint8_t oe = 0; oe < e_element_total; oe++) {
						if(word == prefixes[oe]) {
							element = static_cast<e_obj_element>(oe);
						}
					}
					// TODO Parse obj types
					switch(element) {
					case e_element_comment:
						//
						break;
					case e_element_face: break;
					case e_element_group: break;
					case e_element_line: break;
					case e_element_vertex: break;
					default:
						element = e_element_total;
						status = e_status_unknown;
					}
				}
			}
		}
		file.close();
		return status;
	}
}
