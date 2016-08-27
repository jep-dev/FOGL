#ifndef OBJ_HPP
#define OBJ_HPP

#include <vector>
#include <memory>
#include <GL/gl3w.h>

namespace Model {
	/*! An enumeration of supported types */
	typedef enum {
		e_el_c=0,  ///< \ref element_t
		e_el_f,    ///< \ref face_t
		e_el_g,    ///< \ref group_t
		e_el_l,    ///< \ref line_t
		e_el_m,    ///< \ref mtllib_t
		e_el_o,    ///< \ref object_t
		e_el_u,    ///< \ref usemtl_t
		e_el_v,    ///< \ref vertex_t
		e_el_vn,   ///< \ref vertex_norm_t
		e_el_vp,   ///< \ref vertex_param_t
		e_el_total ///< The total number of supported types
	} e_el;

	/// The abstract base of each obj element
	template<e_el type>
	struct element_t {
		static constexpr const char *prefix = "";
		virtual ~element_t(void) {}
	};
	
	/// A comment, used only for storage
	template<> struct element_t<e_el_c> {
		static constexpr const char *prefix = "#";
	};

	/// A set of vertices with or without texture coordinates
	template<> struct element_t<e_el_f> {
		static constexpr const char *prefix = "f";
	};

	/// A collection of faces, lines, and vertices
	template<> struct element_t<e_el_g> {
		static constexpr const char *prefix = "g";
	};

	/// A pair of vertex indices 
	template<> struct element_t<e_el_l> {
		static constexpr const char *prefix = "l";
	};

	/// A material library file
	template<> struct element_t<e_el_m> {
		static constexpr const char *prefix = "mtllib";
	};

	/// An object; see \ref group_t
	template<> struct element_t<e_el_o> {
		static constexpr const char *prefix = "o";
	};

	/// A reference to a loaded material
	template<> struct element_t<e_el_u> {
		static constexpr const char *prefix = "usemtl";
	};


	/// A single point, containing at least x, y, z coordinates
	template<> struct element_t<e_el_v> {
		static constexpr const char *prefix = "v";
	};

	/// A normal vector; coordinates x, y, z
	template<> struct element_t<e_el_vn> {
		static constexpr const char *prefix = "vn";
	};

	/// A single point in parameter space; coordinates u[, v [,w]]
	template<> struct element_t<e_el_vp> {
		static constexpr const char *prefix = "vp";
	};

	inline std::ostream& operator<<(std::ostream& lhs, e_el const& rhs) {
		return lhs << std::string(rhs == e_el_c ?
				element_t<e_el_c>::prefix : rhs == e_el_f ?
				element_t<e_el_f>::prefix : rhs == e_el_g ?
				element_t<e_el_g>::prefix : rhs == e_el_l ?
				element_t<e_el_l>::prefix : rhs == e_el_m ?
				element_t<e_el_m>::prefix : rhs == e_el_o ?
				element_t<e_el_o>::prefix : rhs == e_el_u ?
				element_t<e_el_u>::prefix : rhs == e_el_v ?
				element_t<e_el_v>::prefix : rhs == e_el_vn ? 
				element_t<e_el_vn>::prefix : element_t<e_el_vp>::prefix);
	}

	/// An object 
	struct obj_t {
		/*! The enumeration and count of possible states */
		typedef enum {
			e_ok=0,        ///< Parsing status is OK
			e_err_io,      ///< An IO exception has occurred
			e_err_unknown, ///< An unknown exception has occurred
			e_err_total    ///< The total number of statuses
		} e_status;

		static constexpr const unsigned int
			mask_has_floats = (1<<e_el_v) | (1<<e_el_vn) | (1<<e_el_vp),
			mask_has_ints = (1<<e_el_f) | (1<<e_el_l),
			mask_has_strings = (1<<e_el_c) | (1<<e_el_g)
				| (1<<e_el_m) | (1<<e_el_o) | (e_el_u);
		
		/** Loads an obj file with the given path into a vector of elements
		 * @param fname The path to the obj file
		 * @param elements The destination, a vector of elements
		 * @return e_status_ok (0) if and only if the load was successful
		 */
		static e_status load(const char *fname, obj_t &elements);
		static e_el parse_type(std::string word);
		e_status parse(std::string line, const char *delim = " ");

		std::vector<std::string> strings;
		std::vector<GLint> ints;
		std::vector<GLfloat> floats;
		std::vector<GLuint> nStrings, nInts, nFloats;
		std::vector<e_el> types;
	};
	
}

#endif
