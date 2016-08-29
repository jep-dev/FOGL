#include "model.hpp"
#include "system.hpp"
#include <iostream>
#include <fstream>

namespace Model {
	namespace Ply {

		const std::vector<std::string> 
		Primitive::NAMES {
			"int8", "uint8", "int16", "uint16",
				"int32", "uint32", "float32", "float64"
		}, Primitive::ALIASES {
			"char", "uchar", "short", "ushort", 
				"int", "uint", "float", "double"
		};

		bool Primitive::getID(const std::string &from, ID &to) {
			for(int i = 0; i < 8; ++i) {
				auto name = NAMES[i], alias = ALIASES[i];
				if(from == name || from == alias) {
					to = (ID) i;
					return true;
				}
			}
			return false;
		}

		std::ostream& operator<<(std::ostream& lhs,
				Primitive::ID const& rhs) {
			return lhs << Primitive::NAMES[rhs];
		}

		std::ostream& operator<<(std::ostream& lhs,
				Property const& rhs) {
			lhs << "property ";
			if(rhs.is_list) {
				lhs << "list " << rhs.sizeType << " ";
			}
			return lhs << rhs.valueType << " " 
				<< rhs.name;
		}

		std::ostream& operator<<(std::ostream& lhs,
				Element const& rhs) {
			lhs << "element " << rhs.name << " " 
				<< rhs.instances;
			for(auto property : rhs.properties) {
				lhs << "\r\n" << property;
			}
			return lhs;
		}

		Header::Header(const char *fname) {
			static constexpr const char 
				*ascii = "format ascii 1.0",
				*lendian = "format binary_little_endian 1.0",
				*bendian = "format binary_bit_endian 1.0";

			std::ifstream file;
			file.open(fname, std::ios::in);
			if(!file.is_open()) {
				status = NO_FILE;
				return;
			}
			std::size_t end_header;

			Element el;
			bool has_ply = false, has_fmt = false,
					 has_endh = false, is_element = false;
			for(std::string word, line; 
					!status && std::getline(file, line);) {

				if(!has_ply) {
					if(line == "ply") {
						has_ply = true;
						continue;
					}
					status = NO_PLY;
					break;
				}

				std::istringstream iss(line);
				if(!(iss >> word)) {
					continue;
				} else if(word == "comment") {
					comments.push_back(line.substr(word.size()+1));
					continue;
				}

				if(!has_fmt) {
					if(line == ascii) {
						is_ascii = true;
					} else if(line == lendian) {
						is_ascii = false;
						is_lendian = true;
					} else if(line == bendian) {
						is_ascii = false;
					} else if(word == "format") {
						status = BAD_FMT;
						statusContext = line;
						break;
					} else {
						status = NO_FMT;
						break;
					}
					has_fmt = true;
					continue;
				}

				has_endh = line == "end_header";
				if(has_endh || word == "element") {
					if(is_element) {
						elements.push_back(el);
					}
					if(has_endh) {
						end_header = file.tellg();
						break;
					}
					el = Element{};
					if(iss >> el.name >> el.instances) {
						is_element = true;
						el.data.reserve(el.instances);
						continue;
					}
					status = BAD_ELEM;
					statusContext = line;
					break;
				}

				if(word == "property") {
					if(!is_element) {
						status = NO_ELEM;
						statusContext = line;
						break;
					}
					if(iss >> word) {
						Primitive::ID value, size;
						if(word == "list") {
							std::string t1, t2, name;
							if(iss >> t1 >> t2 >> name) {
								if(!Primitive::getID(t1, size)) {
									status = BAD_TYPE;
									statusContext = t1;
									break;
								}
								if(!Primitive::getID(t2, value)) {
									status = BAD_TYPE;
									statusContext = t2;
									break;
								}
								el.has_list = true;
								el.properties.emplace_back(
										Property {name, true, value, size});
								continue;
							}
						} else {
							std::string name;
							if(iss >> name) {
								if(!Primitive::getID(word, value)) {
									status = BAD_TYPE;
									statusContext = word;
									break;
								}
								el.properties.emplace_back(
										Property{name, false, value, value});
								continue;
							}
						}
					}
					status = BAD_PROP;
					statusContext = line;
					break;
				}
				status = UNKNOWN;
				statusContext = word;
				break;
			}
			file.close();

			if(!status) {
				if(!has_ply) {
					status = NO_PLY;
				} else if(!has_fmt) {
					status = NO_FMT;
				} else if(!has_endh) {
					status = NO_ENDH;
				}
			}
			if(status) {
				return;
			}

			file.open(fname, is_ascii ? (std::ios::in) : 
					(std::ios::in|std::ios::binary));

			if(!file.is_open()) {
				status = NO_FILE;
				return;
			}
			file.seekg(end_header);

			for(auto &el : elements) {
				for(int el_i = 0, el_n = el.instances;
						!status && el_i < el_n; ++el_i) {
					for(auto &prop : el.properties) {
						if(ascii && !el.readProperty(file, prop)) {
							status = EARLY_EOF;
							std::ostringstream oss;
							oss << "while parsing " << el.name
								<< "[" << el_i << "/" << el_n << "]." 
								<< prop.name;
							statusContext = oss.str();
							break;
						}
					}
				}
				if(status) {
					break;
				}
			}

			if(file.is_open()) {
				file.close();
			}
		}

		template<typename T>
		bool Element::readProperty(
				std::ifstream &file, Property &prop, int n) {
			T value;
			int size = data.size();
			for(int i = 0; i < n; ++i) {
				if(!(file >> value)) {
					return false;
				}
				push_back(value);
			}
			prop.indices.push_back(size);
			return true;
		}
		bool Element::readProperty(
				std::ifstream &file, Property &prop) {
			int n;
			if(prop.is_list) {
				if(file >> n) {
					prop.counts.push_back(n);
				} else {
					return false;
				}
			} else {
				n = 1;
			}
			switch(prop.valueType) {
				case Primitive::INT8:
					return readProperty<int8_t>(file, prop, n);
				case Primitive::UINT8:
					return readProperty<uint8_t>(file, prop, n);
				case Primitive::INT16:
					return readProperty<int16_t>(file, prop, n);
				case Primitive::UINT16:
					return readProperty<uint16_t>(file, prop, n);
				case Primitive::INT32:
					return readProperty<int32_t>(file, prop, n);
				case Primitive::UINT32:
					return readProperty<uint32_t>(file, prop, n);
				case Primitive::FLOAT32:
					return readProperty<float>(file, prop, n);
				case Primitive::FLOAT64:
					return readProperty<double>(file, prop, n);
				default:
					return false;
			}
		}
	}
}
