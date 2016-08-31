#include "model/obj.hpp"

#include <cstring>
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
		if(word == element_t<e_el_f0>::prefix) return e_el_f0;
		if(word == element_t<e_el_g>::prefix) return e_el_g;
		if(word == element_t<e_el_l>::prefix) return e_el_l;
		if(word == element_t<e_el_m>::prefix) return e_el_m;
		if(word == element_t<e_el_o>::prefix) return e_el_o;
		if(word == element_t<e_el_u>::prefix) return e_el_u;
		if(word == element_t<e_el_s>::prefix) return e_el_s;
		if(word == element_t<e_el_v>::prefix) return e_el_v;
		if(word == element_t<e_el_vn>::prefix) return e_el_vn;
		if(word == element_t<e_el_vp>::prefix) return e_el_vp;
		return e_el_total;
	}

	obj_t::e_status obj_t::parse(std::string line, const char *delim) {
		auto status = obj_t::e_status::e_ok;
		boost::tokenizer<boost::char_separator<char>>
			tk(line, boost::char_separator<char>(delim)),
			s_tk(line, boost::char_separator<char>(" /"));
		int i = 0, j = 0;
		auto prev = e_el_total;
		for(auto it = std::begin(tk); it != std::end(tk); ++it) {
			auto word = *it++;
			auto type = parse_type(word);
			auto has_strings = false, has_floats = false,
				 has_ints = false, has_bools = false;
			if(type == e_el_s) {
				has_bools = true;
				if(it == std::end(tk)) {
					bools.emplace_back(true);
				} else {
					auto word = *it++;
					bools.emplace_back(word == "1" || word == "on");
				}
				nBools.emplace_back(1);
			} else {
				nBools.emplace_back(0);
			}
			if((mask_has_floats & (1<<type)) != 0) {
				has_floats = true;
				float val;
				int nValues = 0;
				while(it != std::end(tk)) {
					val = boost::lexical_cast<float>(*it++);
					floats.push_back(val);
					nValues++;
				}
				nFloats.emplace_back(nValues);
			} else {
				nFloats.emplace_back(0);
			}
			if((mask_has_ints & (1<<type)) != 0) {
				has_ints = true;
				int index, nIndices = 0;
				if(type == e_el_f0) {
					it++;
					word = *it;
					auto firstSlash = word.find("/"),
						 lastSlash = word.rfind("/");
					if(firstSlash != std::string::npos) {
						if(firstSlash == lastSlash) {
							type = e_el_f1;
						} else if(firstSlash == lastSlash-1) {
							type = e_el_f2;
						} else {
							type = e_el_f3;
						}
					} else {
						type = e_el_f0;
					}
					auto s_it = std::begin(s_tk);
					s_it++;
					while(s_it != std::end(s_tk)) {
						index = boost::lexical_cast<int>(*s_it++);
						ints.push_back(index);
						nIndices++;
					}
				} else {
					while(it != std::end(tk)) {
						index = boost::lexical_cast<int>(*it++);
						ints.push_back(index);
						nIndices++;
					}
				}
				nInts.emplace_back(nIndices);
			} else {
				nInts.emplace_back(0);
			}
			if((mask_has_strings & (1<<type)) != 0) {
				has_strings = true;
				std::ostringstream oss;
				while(it != std::end(tk)) {
					oss << *it++ << " ";
				}
				strings.emplace_back(oss.str());
				nStrings.emplace_back(1);
			} else {
				nStrings.emplace_back(0);
			}
			if(!has_bools && !has_floats && !has_ints && !has_strings) {
				nBools.emplace_back(0);
				nFloats.emplace_back(0);
				nInts.emplace_back(0);
				nStrings.emplace_back(0);
				status = e_err_unknown;
			} 
			types.emplace_back(type);
			if(type != prev) {
				switch(prev) {
					case e_el_v:
						v_end.emplace_back(i);
						break;
					case e_el_vn:
						vn_end.emplace_back(i);
						break;
					case e_el_vp:
						vt_end.emplace_back(i);
						break;
					case e_el_f2:
						f_end.emplace_back(j);
						break;
					default:
						break;
				}
				switch(type) {
					case e_el_v:
						v_beg.emplace_back(i);
						break;
					case e_el_vn:
						vn_beg.emplace_back(i);
						break;
					case e_el_vp:
						vt_beg.emplace_back(i);
						break;
					case e_el_f2:
						f_beg.emplace_back(j);
						break;
					default:
						break;
				}
			}
			if(mask_has_floats & (1<<type)) {
				i++;
			}
			if(mask_has_ints & (1<<type)) {
				j++;
			}
			prev = type;
			break;
		}
		switch(prev) {
			case e_el_v:
				v_end.emplace_back(i);
				break;
			case e_el_vn:
				vn_end.emplace_back(i);
				break;
			case e_el_vp:
				vt_end.emplace_back(i);
				break;
			default:
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
			auto st = obj.parse(line, " ");
			if(status == e_ok) status = st;
			// TODO Break or no?
		}

		file.close();
		return status;
	}
}
