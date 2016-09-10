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
	e_el obj_t::parse_type(std::string line) {
		std::istringstream iss(line);
		std::string word;
		iss >> word;
		if(word == element_t<e_el_c>::prefix) return e_el_c;
		if(word == element_t<e_el_f0>::prefix) {
			auto type = e_el_f0;
			iss >> word;
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
			return type;
		}
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

	obj_t::e_status obj_t::parse(std::string line, e_el type) {
		auto status = obj_t::e_status::e_ok;
		boost::tokenizer<boost::char_separator<char>>
			tk(line, boost::char_separator<char>(" ")),
			s_tk(line, boost::char_separator<char>(" /"));
		for(auto it = std::begin(tk); it != std::end(tk); ++it) {
			auto word = *it++;
			int n_bools = 0, n_floats = 0, n_ints = 0, n_strings = 0;
			if(type == e_el_s) {
				if(it == std::end(tk)) {
					bools.emplace_back(true);
				} else {
					auto word = *it++;
					bools.emplace_back(word == "1" || word == "on");
				}
				n_bools++;
			}
			if(mask_has_floats & (1<<type)) {
				float val;
				while(it != std::end(tk)) {
					val = boost::lexical_cast<float>(*it++);
					floats.push_back(val);
					n_floats++;
				}
			}
			if(mask_has_ints & (1<<type)) {
				int index;
				if(type == e_el_f0 || type == e_el_f1
						|| type == e_el_f2 || type == e_el_f3) {
					auto s_it = std::begin(s_tk);
					s_it++;
					while(s_it != std::end(s_tk)) {
						index = boost::lexical_cast<int>(*s_it++)-1;
						ints.push_back(index);
						n_ints++;
					}
				} else {
					it++;
					word = *it;
					while(it != std::end(tk)) {
						index = boost::lexical_cast<int>(*it++);
						ints.push_back(index);
						n_ints++;
					}
				}
			}
			if(mask_has_strings & (1<<type)) {
				std::ostringstream oss;
				while(it != std::end(tk)) {
					oss << *it++ << " ";
				}
				n_strings++;
				strings.emplace_back(oss.str());
			}
			nBools.emplace_back(n_bools);
			nFloats.emplace_back(n_floats);
			nInts.emplace_back(n_ints);
			nStrings.emplace_back(n_strings);
			if(!n_bools && !n_floats && !n_ints && !n_strings) {
				status = e_err_unknown;
			} 
			types.emplace_back(type);
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
		auto status = e_ok;
		for(std::string line; std::getline(file, line);) {
			auto type = obj.parse_type(line);
			auto st = obj.parse(line, type);
			if(status == e_ok) status = st;
		}
		auto prev = e_el_total;
		int i = 0, nb = 0, nf = 0, ni = 0, ns = 0;
		for(auto cur : obj.types) {
			if(cur != prev) {
				switch(cur) {
					case e_el_v: obj.v_beg.emplace_back(nf); break;
					case e_el_vn: obj.vn_beg.emplace_back(nf); break;
					case e_el_vp: obj.vp_beg.emplace_back(nf); break;
					case e_el_f0: obj.f0_beg.emplace_back(ni); break;
					case e_el_f1: obj.f1_beg.emplace_back(ni); break;
					case e_el_f2: obj.f2_beg.emplace_back(ni); break;
					case e_el_f3: obj.f3_beg.emplace_back(ni); break;
					default: break;
				}
				switch(prev) {
					case e_el_v: obj.v_end.emplace_back(nf); break;
					case e_el_vn: obj.vn_end.emplace_back(nf); break;
					case e_el_vp: obj.vp_end.emplace_back(nf); break;
					case e_el_f0: obj.f0_end.emplace_back(ni); break;
					case e_el_f1: obj.f1_end.emplace_back(ni); break;
					case e_el_f2: obj.f2_end.emplace_back(ni); break;
					case e_el_f3: obj.f3_end.emplace_back(ni); break;
					default: break;
				}
			}
			nb += obj.nBools[i];
			nf += obj.nFloats[i];
			ni += obj.nInts[i];
			ns += obj.nStrings[i];
			prev = cur;
			i++;
		}
		switch(prev) {
			case e_el_v: obj.v_end.emplace_back(nf); break;
			case e_el_vn: obj.vn_end.emplace_back(nf); break;
			case e_el_vp: obj.vp_end.emplace_back(nf); break;
			case e_el_f0: obj.f0_end.emplace_back(ni); break;
			case e_el_f1: obj.f1_end.emplace_back(ni); break;
			case e_el_f2: obj.f2_end.emplace_back(ni); break;
			case e_el_f3: obj.f3_end.emplace_back(ni); break;
			default: break;
		}

		file.close();
		return status;
	}
}
