#include "model/obj.hpp"

#include <cstring>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
//#include <sstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

namespace Model {
	const char *obj_t::prefix(obj_t::e_element el) {
		switch(el) {
			case obj_t::e_comment: return "#";
			case obj_t::e_face: return "f";
			case obj_t::e_vertex: return "v";
			case obj_t::e_normal: return "vn";
			default: return "";
		}
	}
	obj_t::e_status obj_t::parse(std::string line) {
		std::istringstream iss(line);
		std::string word;
		iss >> word;
		for(int el = 0; el < e_element_total; el++) {
			if(word == prefix(static_cast<e_element>(el))) {
				switch(el) {
					case e_comment: comments.emplace_back(line); break;
					case e_face: parse_face(line); break;
					case e_vertex: parse_vertex(line); break;
					case e_normal: parse_normal(line); break;
					default: break;
				}
			}
		}
		return e_ok;
	}
	obj_t::e_status obj_t::parse_face(std::string line) {
		std::istringstream iss(line);
		std::string word;
		iss >> word;
		if(line.find("//") != std::string::npos) {
			int f0, fn0, f1, fn1, f2, fn2;
			sscanf(line.c_str(), "f %d//%d %d//%d %d//%d",
					&f0, &fn0, &f1, &fn1, &f2, &fn2);
			faces.emplace_back(f0-1);
			faces.emplace_back(f1-1);
			faces.emplace_back(f2-1);
			for(auto i : {fn0,fn1,fn2}) {
				for(int j = 0; j < 3; j++) {
					vertex_normals.emplace_back(normals[(i-1)*3+j]);
				}
			}
		} else {
			int index;
			while(iss >> index) {
				faces.emplace_back(index-1);
			}
		}
		return e_ok;
	}
	obj_t::e_status obj_t::parse_vertex(std::string line) {
		std::istringstream iss(line);
		std::string word;
		iss >> word;
		float value;
		while(iss >> value) {
			vertices.emplace_back(value);
		}
		return e_ok;
	}
	obj_t::e_status obj_t::parse_normal(std::string line) {
		std::istringstream iss(line);
		std::string word;
		iss >> word;
		float value;
		while(iss >> value) {
			normals.emplace_back(value);
		}
		return e_ok;
	}

	obj_t::e_status obj_t::load(const char *fname, obj_t &obj) {
		std::ifstream file;
		file.open(fname, std::ios::in);
		if(!file.is_open()) {
			return e_err_io;
		}
		auto status = e_ok;
		std::string line;
		while(std::getline(file, line)) {
			status = obj.parse(line);
			if(status != e_ok) break;
		}
		file.close();
		return status;
	}
}
