#include "../inc/model.hpp"

namespace Model {

	bool ply(const char *fname, model &out) {

		static const char 
			*ascii = "ascii",
			*lendian = "binary_little_endian",
			*bendian = "binary_big_endian";

		vector<string> lines;
		if(System::readFile(fname, lines)) {
			PlyFormat fmt;
			bool has_ply(false),
				 has_fmt(false),
				 has_endh(false);
			for(auto line : lines) {
				string word;
				std::istringstream iss(line);
				if(iss >> word) {
					if(!has_endh) {
						if(word == "comment") {
							continue;
						}
						if(!has_ply && word == "ply") {
							has_ply = true;
							continue;
						}
						if(!has_fmt && iss >> word) {
							if(word == ascii) {
								fmt = PlyFormat::ASCII;
							} else if(word == lendian) {
								fmt = PlyFormat::LENDIAN;
							} else if(word == bendian) {
								fmt = PlyFormat::BENDIAN;
							} else {
								fmt = PlyFormat::UNDEF;
								return false;
							}
							continue;
						}
					}
				} 
			}
			return false;
		}
	}

}
