#ifndef MTL_HPP
#define MTL_HPP

#include "model.hpp"

namespace Model {
	typedef enum {
		e_mtl_bump = 0, ///< \ref mtl_t<e_mtl_bump>
		e_mtl_d,        ///< \ref mtl_t<e_mtl_d>
		e_mtl_disp,     ///< \ref mtl_t<e_mtl_disp>
		e_mtl_decal,    ///< \ref mtl_t<e_mtl_decal>
		e_mtl_illum,    ///< \ref mtl_t<e_mtl_illum>
		e_mtl_ka,       ///< \ref mtl_t<e_mtl_ka>
		e_mtl_kd,       ///< \ref mtl_t<e_mtl_kd>
		e_mtl_ks,       ///< \ref mtl_t<e_mtl_ks>
		e_mtl_map_bump, ///< \ref mtl_t<e_mtl_map_bump>
		e_mtl_map_d,    ///< \ref mtl_t<e_mtl_map_d>
		e_mtl_map_ka,   ///< \ref mtl_t<e_mtl_map_ka>
		e_mtl_map_kd,   ///< \ref mtl_t<e_mtl_map_kd>
		e_mtl_map_ks,   ///< \ref mtl_t<e_mtl_map_ks>
		e_mtl_map_ns,   ///< \ref mtl_t<e_mtl_map_ns>
		e_mtl_newmtl,   ///< \ref mtl_t<e_mtl_newmtl>
		e_mtl_ns,       ///< \ref mtl_t<e_mtl_ns>
		e_mtl_refl,     ///< \ref mtl_t<e_mtl_refl>
		e_mtl_tr,       ///< \ref mtl_t<e_mtl_tr>
		e_mtl_total     ///< The total number of enumerated materials
	} e_mtl;            ///< An enumeration of material types

	/// Abstract base for material types
	template<e_mtl MTL> struct mtl_t {
		static constexpr const char *prefix = "";
		virtual ~mtl_t(void) {}
	};

	/// Bump map
	template<> struct mtl_t<e_mtl_bump> {
		static constexpr const char *prefix = "bump";
	};

	/// Alpha
	template<> struct mtl_t<e_mtl_d> {
		static constexpr const char *prefix = "d";
	};

	/// Displacement
	template<> struct mtl_t<e_mtl_disp> {
		static constexpr const char *prefix = "disp";
	};

	/// Stencil decal
	template<> struct mtl_t<e_mtl_decal> {
		static constexpr const char *prefix = "decal";
	};

	/// Illumination
	template<> struct mtl_t<e_mtl_illum> {
		static constexpr const char *prefix = "illum";
	};

	/// Texture origin
	template<> struct mtl_t<e_mtl_ka> {
		static constexpr const char *prefix = "Ka";
	};

	/// Texture alpha
	template<> struct mtl_t<e_mtl_kd> {
		static constexpr const char *prefix = "Kd";
	};

	/// Texture specular
	template<> struct mtl_t<e_mtl_ks> {
		static constexpr const char *prefix = "Ks";
	};

	/// Bump map
	template<> struct mtl_t<e_mtl_map_bump> {
		static constexpr const char *prefix = "map_bump";
	};

	/// Alpha map
	template<> struct mtl_t<e_mtl_map_d> {
		static constexpr const char *prefix = "map_d";
	};

	/// Ambient map
	template<> struct mtl_t<e_mtl_map_ka> {
		static constexpr const char *prefix = "map_Ka";
	};

	/// Diffuse map
	template<> struct mtl_t<e_mtl_map_kd> {
		static constexpr const char *prefix = "map_Kd";
	};
	
	/// Diffuse map
	template<> struct mtl_t<e_mtl_map_ks> {
		static constexpr const char *prefix = "map_Ks";
	};

	/// Specular highlight map
	template<> struct mtl_t<e_mtl_map_ns> {
		static constexpr const char *prefix = "map_Ns";
	};

	/// Material declaration
	template<> struct mtl_t<e_mtl_newmtl> {
		static constexpr const char *prefix = "newmtl";
	};

	/// Specular highlight
	template<> struct mtl_t<e_mtl_ns> {
		static constexpr const char *prefix = "Ns";
	};

	/// Reflection
	template<> struct mtl_t<e_mtl_refl> {
		static constexpr const char *prefix = "refl";
	};
	
	/// Inverse alpha
	template<> struct mtl_t<e_mtl_tr> {
		static constexpr const char *prefix = "tr";
	};

	/// Stream insertion operator for material values
	inline std::ostream& operator<<(std::ostream& lhs, e_mtl const& rhs) {
		return lhs << std::string(
			rhs == e_mtl_bump ? mtl_t<e_mtl_bump>::prefix
			: rhs == e_mtl_d ? mtl_t<e_mtl_d>::prefix
			: rhs == e_mtl_disp ? mtl_t<e_mtl_disp>::prefix
			: rhs == e_mtl_decal ? mtl_t<e_mtl_decal>::prefix
			: rhs == e_mtl_illum ? mtl_t<e_mtl_illum>::prefix
			: rhs == e_mtl_ka ? mtl_t<e_mtl_ka>::prefix
			: rhs == e_mtl_kd? mtl_t<e_mtl_kd>::prefix
			: rhs == e_mtl_ks? mtl_t<e_mtl_ks>::prefix
			: rhs == e_mtl_map_bump? mtl_t<e_mtl_map_bump>::prefix
			: rhs == e_mtl_map_d? mtl_t<e_mtl_map_d>::prefix
			: rhs == e_mtl_map_ka? mtl_t<e_mtl_map_ka>::prefix
			: rhs == e_mtl_map_kd? mtl_t<e_mtl_map_kd>::prefix
			: rhs == e_mtl_map_ks? mtl_t<e_mtl_map_ks>::prefix
			: rhs == e_mtl_map_ns? mtl_t<e_mtl_map_ns>::prefix
			: rhs == e_mtl_newmtl? mtl_t<e_mtl_newmtl>::prefix
			: rhs == e_mtl_ns? mtl_t<e_mtl_ns>::prefix
			: rhs == e_mtl_refl? mtl_t<e_mtl_refl>::prefix
			: mtl_t<e_mtl_tr>::prefix);
	}

	/// The serialization/deserialization for wavefront materials
	struct material_t {
		typedef enum {
			e_ok=0,
			e_err_io,
			e_err_unknown,
			e_err_total
		} e_status;

		static constexpr const unsigned int
			mask_has_bools = 0;
		static constexpr const unsigned int
			mask_has_floats = (1<<e_mtl_d)
				| (1<<e_mtl_ka) | (1<<e_mtl_kd) | (1<<e_mtl_ks);
		static constexpr const unsigned int
			mask_has_ints = 0;
		static constexpr const unsigned int
			mask_has_strings = (e_mtl_total - 1) ^ mask_has_floats;

		/** Parse a single type from the head of a line
		 * @param word The first word of a line in a material file
		 * @return The material value, or e_mtl_total
		 */
		e_mtl parse_type(std::string word);


		/** Parse and store line using the given delimiter string
		 * @param line A whole line of text from a material file
		 * @param delim The delimiter used by Boost::tokenizer
		 * @return \ref e_ok if and only if the line was parsed successfully
		 */
		e_status parse(std::string line, const char *delim = " ");

		/** Load and store a file into the given material
 		 * @param fname The path to a material file (.mtl)
 		 * @param material The destination of the loaded material(s)
 		 * @return \ref e_ok if and only if the file was parsed successfully
 		 */
		e_status load(const char *fname, material_t &material);

		std::vector<bool> bools;
		std::vector<float> floats;
		std::vector<int> ints;
		std::vector<std::string> strings;
		std::vector<e_mtl> types;
	};
}

#endif
