#include "model/obj.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <memory>
//#include <sstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

namespace Model {
	e_el obj_t::parse_type(std::string word) {
		if(word == element_t<e_el_c>::prefix) return e_el_c;
		if(word == element_t<e_el_f>::prefix) return e_el_f;
		if(word == element_t<e_el_g>::prefix) return e_el_g;
		if(word == element_t<e_el_l>::prefix) return e_el_l;
		if(word == element_t<e_el_m>::prefix) return e_el_m;
		if(word == element_t<e_el_o>::prefix) return e_el_o;
		if(word == element_t<e_el_u>::prefix) return e_el_u;
		if(word == element_t<e_el_v>::prefix) return e_el_v;
		if(word == element_t<e_el_vn>::prefix) return e_el_vn;
		if(word == element_t<e_el_vp>::prefix) return e_el_vp;
		return e_el_total;
	}

	obj_t::e_status obj_t::parse(std::string line, const char *delim) {
		auto status = obj_t::e_status::e_ok;
		boost::tokenizer<boost::char_separator<char>> tk(line,
			boost::char_separator<char>(delim));
		for(auto it = std::begin(tk); it != std::end(tk); ++it) {
			auto word = *it++;
			auto type = parse_type(word);
			if((mask_has_strings & (1<<type)) != 0) {
				std::ostringstream oss;
				//std::string words;
				while(it != std::end(tk)) {
					oss << *it++ << delim;
				}
				strings.emplace_back(oss.str());
				nFloats.emplace_back(0);
				nInts.emplace_back(0);
				nStrings.emplace_back(1);
				types.emplace_back(type);
				break;
			} else if((mask_has_ints & (1<<type)) != 0) {
				int index, nIndices = 0;
				while(it != std::end(tk)) {
					index = boost::lexical_cast<int>(*it++);
					ints.push_back(index);
					nIndices++;
				}
				nFloats.emplace_back(0);
				nInts.emplace_back(nIndices);
				nStrings.emplace_back(0);
				types.emplace_back(type);
				break;
			} else if((mask_has_floats & (1<<type)) != 0) {
				float val;
				int nValues = 0;
				while(it != std::end(tk)) {
					val = boost::lexical_cast<float>(*it++);
					floats.push_back(val);
					nValues++;
				}
				nFloats.emplace_back(nValues);
				nInts.push_back(0);
				nStrings.push_back(0);
				types.emplace_back(type);
			} else {
				std::cout << word << ": unknown element prefix"
					<< std::endl;
				status = e_err_unknown;
			}
			break;
		}
		return status;
	}

	obj_t::e_status obj_t::load(const char *fname, obj_t &obj) {
		std::ifstream file;
		file.open(fname, std::ios::in);
		if(!file.is_open()) {
			return e_err_io;
		}
		e_el element;
		e_status status = e_ok;

		for(std::string line; std::getline(file, line);) {
			auto st = obj.parse(line);
			if(status == e_ok) status = st;
			// TODO Break or no?
		}

		file.close();
		return status;
	}
}
