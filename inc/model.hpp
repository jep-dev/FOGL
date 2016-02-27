#ifndef MODEL_HPP
#define MODEL_HPP

#include <algorithm>
#include <map>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

#include <cstring>
#include <cstdint>

#include "quat.hpp"
#include "dual.hpp"
#include "system.hpp"

namespace Model {
	namespace Ply {

		struct Status {
			enum ID {
				OK=0,
				MISSING_FILE,
				MISSING_PLY, 
				MISSING_FORMAT,
				MISSING_END_HEADER,
				INVALID_FORMAT,
				INVALID_ELEMENT,
				INVALID_PROPERTY,
				INVALID_TYPE,
				ORPHAN_PROPERTY,
				UNEXPECTED,
				EARLY_EOF,
				UNKNOWN
			};
			operator bool(void) const {
				return !id;
			}
			friend std::ostream& operator<<
					(std::ostream& lhs, ID const& rhs) {
				return lhs << (labels.count(rhs) ? 
					labels.at(rhs) : "");
			}
			friend std::ostream& operator<<
					(std::ostream& lhs, Status const& rhs) {
				if(rhs) {
					return lhs;
				}
				if(rhs.filename.size()) {
					lhs << "In file '" << rhs.filename 
						<< "'...\r\n";
				}
				if(rhs.context.size()) {
					lhs << "At line '" << rhs.context 
						<< "'...\r\n";
				}
				lhs << rhs.id;
				if(rhs.details.size()) {
					lhs << ": " << rhs.details;
				}
				return lhs;
			}
			ID id;
			std::string filename, context, details;
		private:
			static const std::map<ID, std::string> labels;
		};

		struct Primitive {
			enum ID {
				INT8=0,  UINT8,
				INT16,   UINT16,
				INT32,   UINT32,
				FLOAT32, FLOAT64
			};

			friend std::ostream& operator<<
					(std::ostream& lhs, ID const& rhs) {
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
			Status status;
			bool is_ascii, is_lendian;
			std::vector<std::string> comments;
			std::vector<Element> elements;
			Header(const char*);
		};
		
	}
}

#endif
