#ifndef OBJ_HPP
#define OBJ_HPP

#include <vector>
#include <memory>

namespace Model {
	/// An object 
	struct obj_t {
		/*! The enumeration and count of possible states */
		typedef enum {
			e_ok=0,       ///< Parsing status is OK
			e_err_io,     ///< An IO exception has occurred
			e_err_unknown,///< An unknown exception has occurred
			e_err_total   ///< The total number of statuses
		} e_status;

		/*! An enumeration of supported types */
		typedef enum {
			e_el_c=0,    ///< \ref element_t
			e_el_f,      ///< \ref face_t
			e_el_g,      ///< \ref group_t
			e_el_l,      ///< \ref line_t
			e_el_mtllib, ///< \ref mtllib_t
			e_el_o,      ///< \ref object_t
			e_el_usemtl, ///< \ref usemtl_t
			e_el_v,      ///< \ref vertex_t
			e_el_vn,     ///< \ref vertex_norm_t
			e_el_vp,     ///< \ref vertex_param_t
			e_el_total   ///< The total number of supported types
		} e_el;

		/// The abstract base of each obj element
		struct element_t {
			static constexpr const char *prefix = "";
			const e_el type;
			element_t(e_el type): type(type) {}
			virtual ~element_t(void) {}
		};

		/// A comment, used only for storage
		struct comment_t : public element_t {
			static constexpr const char *prefix = "#";
			friend std::ostream& operator<<(std::ostream& os,
					comment_t const& el);
			std::string contents;
			comment_t(std::string contents):
				element_t(e_el_c),
				contents(contents) {}
		};

		/// A set of vertices with or without texture coordinates
		struct face_t : public element_t {
			static constexpr const char *prefix = "f";
			friend std::ostream& operator<<(std::ostream& os,
					face_t const& el);
			std::vector<unsigned int> vertices, coordinates;
			bool tex_coords = false;
			face_t(bool has_tex = false):
				element_t(e_el_f), tex_coords(has_tex) {}
		};

		/// A collection of faces, lines, and vertices
		struct group_t : public element_t {
			static constexpr const char *prefix = "g";
			friend std::ostream& operator<<(std::ostream& os,
					group_t const& el);
			std::string name;
			group_t(std::string name):
				element_t(e_el_g), name(name) {}
		};

		/// A pair of vertex indices 
		struct line_t : public element_t {
			static constexpr const char *prefix = "l";
			friend std::ostream& operator<<(std::ostream& os,
					line_t const& el);
			std::array<unsigned int, 2> vertices;
			line_t(void): element_t(e_el_l) {}
		};

		/// A material library file
		struct mtllib_t : public element_t {
			static constexpr const char *prefix = "mtllib";
			friend std::ostream& operator<<(std::ostream& os,
					mtllib_t const& el);
			std::string path;
			mtllib_t(std::string path) :
				element_t(e_el_mtllib), path(path) {}
		};

		/// An object; see \ref group_t
		struct object_t : public element_t {
			static constexpr const char *prefix = "o";
			friend std::ostream& operator<<(std::ostream& os,
					object_t const& el);
			std::string name;
			object_t(std::string name):
				element_t(e_el_o), name(name) {}
		};

		/// A reference to a loaded material
		struct usemtl_t : public element_t {
			static constexpr const char *prefix = "usemtl";
			friend std::ostream& operator<<(std::ostream& os,
					usemtl_t const& el);
			std::string name;
			usemtl_t(std::string name):
				element_t(e_el_usemtl), name(name) {}
		};


		/// A single point, containing at least x, y, z coordinates
		struct vertex_t : public element_t {
			static constexpr const char *prefix = "v";
			friend std::ostream& operator<<(std::ostream& os,
					vertex_t const& vertex);
			std::vector<float> point;
			vertex_t(void): element_t(e_el_v) {}
		};

		/// A normal vector; coordinates x, y, z
		struct vertex_norm_t : public element_t {
			static constexpr const char *prefix = "vn";
			friend std::ostream& operator<<(std::ostream& os,
					vertex_norm_t const& norm);
			std::vector<float> point;
			vertex_norm_t(void): element_t(e_el_vn) {}
		};

		/// A single point in parameter space; coordinates u[, v [,w]]
		struct vertex_param_t : public element_t {
			static constexpr const char *prefix = "vp";
			friend std::ostream& operator<<(std::ostream& os,
					vertex_param_t const& param);
			std::vector<float> point;
			vertex_param_t(void): element_t(e_el_vp) {}
		};

		/** Loads an obj file with the given path into a vector of elements
		 * @param fname The path to the obj file
		 * @param elements The destination, a vector of elements
		 * @return e_status_ok (0) if and only if the load was successful
		 */
		static e_status load(const char *fname, obj_t &elements);

		std::vector<comment_t> comments;
		std::vector<face_t> faces;
		std::vector<group_t> groups;
		std::vector<line_t> lines;
		std::vector<mtllib_t> mtllibs;
		std::vector<object_t> objects;
		std::vector<usemtl_t> usemtls;
		std::vector<vertex_t> vertices;
		std::vector<vertex_norm_t> norms;
		std::vector<vertex_param_t> params;
		std::vector<e_el> types;
	};
}

#endif
