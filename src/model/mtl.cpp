#include <fstream>
#include "model.hpp"

namespace Model {
	material_t::e_status
	material_t::parse(std::string line, const char *delim) {
		auto status = material_t::e_ok;
		return status;
	}

	material_t::e_status
	material_t::load(const char *fname, material_t &material) {
		std::ifstream file;
		file.open(fname);
		if(!file.is_open()) {
			return material_t::e_err_io;
		}
		material_t mtl;
		auto status = material_t::e_status::e_ok;
		for(std::string line; std::getline(file, line);) {
			status = mtl.parse(line);
		}
		file.close();
		return status;
	}
};
