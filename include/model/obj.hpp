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
			e_el_c=0,  ///< \ref element_t
			e_el_f,    ///< \ref face_t
			e_el_g,    ///< \ref group_t
			e_el_l,    ///< \ref line_t
			e_el_o,    ///< \ref object_t
			e_el_v,    ///< \ref vertex_t
			e_el_total ///< The total number of supported types
		} e_el;

		/// The abstract base of each obj element
		struct element_t {
			static constexpr const char *prefix(void);
			const e_el type;
			friend std::ostream& operator<<(std::ostream& os,
					element_t const& el);
			element_t(e_el type): type(type) {}
			virtual ~element_t(void) {}
		};

		/// A comment, used only for storage
		struct comment_t : public element_t {
			static constexpr const char *prefix(void) {
				return "#";
			}
			friend std::ostream& operator<<(std::ostream& os,
					comment_t const& comment) {
				return os << std::string(comment.contents);

			}
			std::string contents;
			comment_t(std::string contents):
				element_t(e_el_c),
				contents(contents) {}
		};

		/// A set of vertices with or without texture coordinates
		struct face_t : public element_t {
			static constexpr const char *prefix(void) {
				return "f";
			}
			std::vector<unsigned int> vertices, coordinates;
			bool tex_coords = false;
			face_t(bool has_tex = false):
				element_t(e_el_f), tex_coords(has_tex) {}
		};

		/// A collection of faces, lines, and vertices
		struct group_t : public element_t {
			static constexpr const char *prefix(void) {
 			   return "g";
			}
			group_t(void): element_t(e_el_g) {}
		};

		/// A pair of vertex indices 
		struct line_t : public element_t {
			static constexpr const char *prefix(void) {
				return "l";
			}
			std::vector<unsigned int> vertices;
			line_t(void): element_t(e_el_l) {}
		};

		/// An object; see \ref group_t
		struct object_t : public element_t {
			static constexpr const char *prefix(void) {
				return "o";
			}
			std::vector<unsigned int> members;
			object_t(void): element_t(e_el_o) {}
		};

		/// A single point, containing at least x, y, z coordinates
		struct vertex_t : public element_t {
			static constexpr const char *prefix(void) {
				return "v";
			}
			std::vector<float> point;
			vertex_t(void): element_t(e_el_v) {}
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
		std::vector<object_t> objects;
		std::vector<vertex_t> vertices;
		std::vector<e_el> types;
	};
}

#endif
