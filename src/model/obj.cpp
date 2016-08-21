#include "model/obj.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <memory>
//#include <sstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

namespace Model {
	obj_t::e_obj_status obj_t::load(const char *fname, obj_t &obj) {
		std::ifstream file;
		file.open(fname, std::ios::in);
		if(!file.is_open()) {
			return e_status_io;
		}
		e_obj_element element;
		e_obj_status status = e_status_ok;
		boost::char_separator<char> sep(" ");
		for(std::string line; std::getline(file, line);) {
			boost::tokenizer<boost::char_separator<char>> tk(line, sep);
			std::cout << "Line: (" << line << ")" << std::endl;
			for(auto it = std::begin(tk); it != std::end(tk); ++it) {
				auto word = *it++;
				if(word == comment_t::prefix()) {
					std::string comment;
					while(it != std::end(tk)) {
						comment += *it++ + ' ';
					}
					obj.comments.emplace_back(comment);
					obj.types.emplace_back(e_element_comment);
					break;
				} else if(word == face_t::prefix()) {
					int index;
					face_t face;
					while(it != std::end(tk)){
						index = boost::lexical_cast<int>(*it++);
						face.vertices.push_back(index);
					}
					obj.faces.push_back(face);
					obj.types.push_back(e_element_face);
					break;
				} else if(word == group_t::prefix()) {
				} else if(word == line_t::prefix()) {
				} else if(word == vertex_t::prefix()) {
				} else {
					std::cout << word << ": unknown element prefix"
						<< std::endl;
					element = e_element_total;
					status = e_status_unknown;
				}
				break;
			}
		}
		file.close();
		return status;
	}
}
