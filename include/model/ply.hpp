#ifndef PLY_HPP
#define PLY_HPP

// Stuff
#include <algorithm>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <map>
#include <vector>

// Printing sutff
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

// Threading stuff
#include <future>

// Mem
#include <cstring>
#include <cstdint>

// Local
#include "util.hpp"
#include "math.hpp"
#include "math/quat.hpp"
#include "math/dual.hpp"
#include "math/affine.hpp"
#include "system.hpp"

namespace Model {
	namespace Ply {
		enum STATUS {
			OK=0,
			NO_FILE,   NO_PLY,
			NO_FMT,    NO_ELEM,
			NO_ENDH,   BAD_FMT,
			BAD_ELEM,  BAD_PROP,
			BAD_TYPE,  EARLY_EOF,
			UNKNOWN
		};

		struct Primitive {
			enum ID {
				INT8=0,  UINT8,
				INT16,   UINT16,
				INT32,   UINT32,
				FLOAT32, FLOAT64
			};

			friend std::ostream&
			operator<<(std::ostream& lhs, ID const& rhs) {
				return lhs << NAMES[rhs];
			}

			static bool getID(const std::string &from, ID &to) {
				for(int i = 0; i < 8; ++i) {
					auto name = NAMES[i], alias = ALIASES[i];
					if(from == name || from == alias) {
						to = (ID) i;
						return true;
					}
				}
				return false;
			}
			static const std::vector<std::string> NAMES, ALIASES;
		};

		struct Property {
			const std::string name;
			const bool is_list;
			const Primitive::ID valueType, sizeType;

			std::vector<int> counts, indices;

			friend std::ostream& operator<<
					(std::ostream& lhs, const Property& rhs) {
				lhs << "property ";
				if(rhs.is_list) {
					lhs << "list " << rhs.sizeType << " ";
				}
				return lhs << rhs.valueType << " " 
					<< rhs.name;
			}
		};
		
		struct Buffer {
			std::vector<uint8_t> data;
			template<typename T>
			void push_back(T const& t) {
				static constexpr int N = sizeof(T);
				auto buf = (uint8_t*) &t;
				data.insert(data.end(), buf, buf+N);
			}
			template<typename S, typename... T>
			void push_back(S const& s, T... t) {
				push_back(s);
				push_back(t...);
			}
		};

		struct Element: public Buffer {
			std::string name;
			bool has_list = false,
					 has_list_multisize = false;
			unsigned int instances;

			std::vector<Property> properties;
			template<typename T>
			bool readProperty(std::ifstream &file,
					Property &prop, int n);
			bool readProperty(std::ifstream &file,
					Property &prop);

			friend std::ostream& operator<<
					(std::ostream& lhs, const Element& rhs) {
				lhs << "element " << rhs.name << " " 
					<< rhs.instances;
				for(auto property : rhs.properties) {
					lhs << "\r\n" << property;
				}
				return lhs;
			}
		};

		struct Header {
			STATUS status = OK;
			std::string statusContext;

			bool is_ascii, is_lendian;
			std::vector<std::string> comments;
			std::vector<Element> elements;
			Header(const char*);
		};
		
	}
}

#endif
