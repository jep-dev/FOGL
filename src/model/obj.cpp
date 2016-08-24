#include "model/obj.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <memory>
//#include <sstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

namespace Model {
	std::ostream& operator<<(std::ostream &os,
			obj_t::comment_t const& comment) {
		return os << std::string(comment.contents);
	}
	std::ostream& operator<<(std::ostream &os, obj_t::line_t const& line) {
		return os << std::string("Line: ") << line.vertices[0] << ", "
			<< line.vertices[1] << std::endl;
	}
	std::ostream& operator<<(std::ostream &os, obj_t::face_t const& face) {
		os << "Face: ";
		for(auto v : face.vertices) {
			os << v << " ";
		}
		if(face.tex_coords) {
			std::cout << "\n\tTex coords: ";
			for(auto c : face.coordinates) {
				std::cout << c << " ";
			}
		}
		return endl(os);
	}
	std::ostream& operator<<(std::ostream &os, obj_t::group_t const& group) {
		return os << "Group: " << group.name << std::endl;
	}
	std::ostream& operator<<(std::ostream &os, obj_t::object_t const& obj) {
		return os << "Object: " << obj.name << std::endl;
	}
	std::ostream &operator<<(std::ostream &os, obj_t::element_t const& el) {
		return os;
	}
	obj_t::e_status obj_t::load(const char *fname, obj_t &obj) {
		std::ifstream file;
		file.open(fname, std::ios::in);
		if(!file.is_open()) {
			return e_err_io;
		}
		e_el element;
		e_status status = e_ok;

		boost::char_separator<char> sep(" ");
		for(std::string line; std::getline(file, line);) {
			boost::tokenizer<boost::char_separator<char>> tk(line, sep);
			for(auto it = std::begin(tk); it != std::end(tk); ++it) {
				auto word = *it++;
				if(word == comment_t::prefix()) {
					std::string comment;
					while(it != std::end(tk)) {
						comment += *it++ + ' ';
					}
					obj.comments.emplace_back(comment);
					obj.types.emplace_back(e_el_c);
					break;
				} else if(word == face_t::prefix()) {
					int index;
					face_t face;
					while(it != std::end(tk)) {
						index = boost::lexical_cast<int>(*it++);
						face.vertices.push_back(index);
					}
					obj.faces.push_back(face);
					obj.types.emplace_back(e_el_f);
					break;
				} else if(word == group_t::prefix()) {
					// TODO Parse group_t from line
				} else if(word == line_t::prefix()) {
					int index;
					line_t line;
					for(int i = 0; i < 2 && it != std::end(tk); ++i, ++it) {
						index = boost::lexical_cast<int>(*it);
						line.vertices[i] = index;
					}
					obj.lines.push_back(line);
					obj.types.emplace_back(e_el_l);
				} else if(word == object_t::prefix()) {
					obj.objects.emplace_back(*it++);
					obj.types.push_back(e_el_o);
				} else if(word == vertex_t::prefix()) {
					float point;
					vertex_t vertex;
					while(it != std::end(tk)) {
						point = boost::lexical_cast<float>(*it++);
						vertex.point.push_back(point);
					}
					obj.vertices.push_back(vertex);
					obj.types.emplace_back(e_el_v);
				} else if(word == vertex_param_t::prefix()) {
					vertex_param_t param;
					float val;
					while(it != std::end(tk)) {
						val = boost::lexical_cast<float>(*it++);
						param.point.push_back(val);
					}
					obj.params.push_back(param);
					obj.types.emplace_back(e_el_vp);
				} else {
					std::cout << word << ": unknown element prefix"
						<< std::endl;
					element = e_el_total;
					status = e_err_unknown;
				}
				break;
			}
		}
		file.close();
		return status;
	}
}
