#ifndef OBJ_HPP
#define OBJ_HPP

#include "model.hpp"
#include <sstream>

namespace Model {

	/// A wavefront object container/parser
	struct obj_t : public model {
		/*! The enumeration and count of possible states */
		typedef enum {
			e_ok=0,        ///< Parsing status is OK
			e_err_io,      ///< An IO exception has occurred
			e_err_unknown, ///< An unknown exception has occurred
			e_err_total    ///< The total number of statuses
		} e_status;
		/*! The enumeration and count of parsed types */
		typedef enum {
			e_comment = 0,
			e_face,
			e_vertex,
			e_normal,
			e_element_total
		} e_element;
		static const char *prefix(e_element el);

		/** Loads an obj file with the given path into a vector of elements
		 * @param fname The path to the obj file
		 * @param elements The destination, a vector of elements
		 * @return e_status_ok (0) if and only if the load was successful
		 */
		static e_status load(const char *fname, obj_t &elements);
		
		/** Parses data from a complete line and a type
 		 * @param line A complete line from a wavefront obj
 		 */
		e_status parse(std::string line);
		e_status parse_face(std::string line);
		e_status parse_vertex(std::string line);
		e_status parse_normal(std::string line);
		friend std::ostream& operator<<(std::ostream& os, obj_t const& obj) {
			int i = -1;
			for(auto v : obj.vertices) {
				i++;
				if(i == 0) {
					os << prefix(e_vertex);
				}
				os << ' ' << v;
				if(i == 2) {
					i = -1;
					os << '\n';
				}
			}
			i = -1;
			for(auto vn : obj.normals) {
				i++;
				if(i == 0) {
					os << prefix(e_normal);
				}
				os << ' ' << vn;
				if(i == 2) {
					i = -1;
					os << '\n';
				}
			}
			i = -1;
			for(auto f : obj.faces) {
				i++;
				if(i == 0) {
					os << prefix(e_face);
				}
				os << ' ' << f;
				if(i == 2) {
					i = -1;
					os << '\n';
				}
			}
			return os;
		}
	};
	
}

#endif
