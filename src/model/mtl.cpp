#include <fstream>
#include "model.hpp"

namespace Model {
	e_mtl material_t::parse_type(std::string word) {
		if(word == mtl_t<e_mtl_bump>::prefix) return e_mtl_bump;
		if(word == mtl_t<e_mtl_d>::prefix) return e_mtl_d;
		if(word == mtl_t<e_mtl_disp>::prefix) return e_mtl_disp;
		if(word == mtl_t<e_mtl_decal>::prefix) return e_mtl_illum;
		if(word == mtl_t<e_mtl_illum>::prefix) return e_mtl_illum;
		if(word == mtl_t<e_mtl_ka>::prefix) return e_mtl_ka;
		if(word == mtl_t<e_mtl_kd>::prefix) return e_mtl_kd;
		if(word == mtl_t<e_mtl_ks>::prefix) return e_mtl_ks;
		if(word == mtl_t<e_mtl_map_bump>::prefix) return e_mtl_map_bump;
		if(word == mtl_t<e_mtl_map_d>::prefix) return e_mtl_map_d;
		if(word == mtl_t<e_mtl_map_ka>::prefix) return e_mtl_map_ka;
		if(word == mtl_t<e_mtl_map_kd>::prefix) return e_mtl_map_kd;
		if(word == mtl_t<e_mtl_map_ks>::prefix) return e_mtl_map_ks;
		if(word == mtl_t<e_mtl_map_ns>::prefix) return e_mtl_map_ns;
		if(word == mtl_t<e_mtl_newmtl>::prefix) return e_mtl_newmtl;
		if(word == mtl_t<e_mtl_ns>::prefix) return e_mtl_ns;
		if(word == mtl_t<e_mtl_refl>::prefix) return e_mtl_refl;
		if(word == mtl_t<e_mtl_tr>::prefix) return e_mtl_tr;
		return e_mtl_total;
	}
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
