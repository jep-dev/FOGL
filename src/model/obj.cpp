#include "model/obj.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>

namespace Model {
	static e_obj_status load(const char *fname, element_t** elements) {
		std::ifstream file;
		file.open(fname, std::ios::in);
		if(!file.is_open()) {
			return e_status_io;
		}
		for(std::string line; std::getline(file, line);) {
			boost::tokenizer<> tk(line);
			bool flip = true;
			e_obj_element element;
			e_obj_status status = e_status_unknown;
			for(auto it = std::begin(tk); it != std::end(tk); ++it) {
				auto word = *it;
				if(flip) {
					flip = false;
					for(e_obj_element oe = 0; oe < e_element_total; oe++) {
						if(word == prefixes[oe]) {
							element = oe;
						}
					}
					// TODO Parsing
					switch(element) {
					case e_element_comment: {
					} break;
					case e_element_face: {
					} break;
					case e_element_group: {
					} break;
					case e_element_line: {
					} break;
					case e_element_vertex: {
					} break;
					default: {
						element = e_element_total;
						status = e_obj_status;
					} break;
					}
				}
			}
		}
		file.close();
	}
}
