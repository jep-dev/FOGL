#ifndef OBJ_HPP
#define OBJ_HPP

namespace Model {
	/// An object 
	struct obj_t {
		/*! The enumeration and count of possible states */
		typedef enum e_obj_status : uint8_t{
			e_status_ok=0,   ///< Parsing status is OK
			e_status_io,     ///< An IO exception has occurred
			e_status_unknown,///< An unknown exception has occurred
			e_status_total   ///< The total number of statuses
		} e_obj_status;

		/*! An enumeration of supported types */
		typedef enum e_obj_element : uint8_t {
			e_element_comment=0, ///< \ref element_t
			e_element_face,      ///< \ref face_t
			e_element_group,     ///< \ref group_t
			e_element_line,      ///< \ref line_t
			e_element_vertex,    ///< \ref vertex_t
			e_element_total      ///< The total number of supported types
		} e_obj_element;

		static constexpr const char *prefixes[e_element_total]{
			"#", "f", "g", "l", "v"
		};

		/// The abstract base of each obj element
		struct element_t {
		protected:
			e_obj_element type;
		};

		/// A comment, used only for storage
		struct comment_t : element_t {
			const char *contents;
		protected:
			const e_obj_element type = e_element_comment;
		};

		/// A set of vertices with or without texture coordinates
		struct face_t : element_t {
			unsigned int *vertices, *coordinates;
			bool tex_coords = false;
		protected:
			const e_obj_element type = e_element_face;
		};

		/// A collection of faces, lines, and vertices
		struct group_t : element_t {
		protected:
			const e_obj_element type = e_element_group;
		};

		/// A pair of vertex indices 
		struct line_t : element_t {
		protected:
			const e_obj_element type = e_element_line;
		};

		/// A single point, containing at least x, y, z coordinates
		struct vertex_t : element_t {
		protected:
			const e_obj_element type = e_element_vertex;
		};

		static e_obj_status load(const char *fname, element_t** elements);
	};
}

#endif
