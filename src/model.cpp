#include "model.hpp"

namespace Model {
	namespace Ply {

		const std::map<Status::ID, std::string> Status::labels {
			{Status::MISSING_FILE, "File not found"},
			{Status::MISSING_PLY, "Expected 'ply'"},
			{Status::MISSING_FORMAT, "Expected format"},
			{Status::MISSING_END_HEADER, "Expected 'end_header'"},
			{Status::INVALID_FORMAT, "Invalid format"},
			{Status::INVALID_ELEMENT, "Invalid element"},
			{Status::INVALID_PROPERTY, "Invalid property"},
			{Status::INVALID_TYPE, "Invalid type"},
			{Status::ORPHAN_PROPERTY, "Orphan property"},
			{Status::UNEXPECTED, "Unexpected token"},
			{Status::EARLY_EOF, "End of file"}
		};

		const std::vector<std::string> 
		Primitive::NAMES {
			"int8", "uint8", "int16", "uint16",
				"int32", "uint32", "float32", "float64"
		}, Primitive::ALIASES {
			"char", "uchar", "short", "ushort", 
				"int", "uint", "float", "double"
		};

		Header::Header(const char *fname) {
			static constexpr const char 
				*ascii = "format ascii 1.0",
				*lendian = "format binary_little_endian 1.0",
				*bendian = "format binary_bit_endian 1.0";

			status.id = Status::OK;
			status.filename = fname;

			std::ifstream file;
			file.open(fname, std::ios::in);
			if(!file.is_open()) {
				status.id = Status::MISSING_FILE;
				return;
			}
			std::size_t end_header;

			Element el;
			bool has_ply = false,
					 has_fmt = false,
					 has_endh = false,
					 is_element = false;
			for(std::string word, line; 
					status && std::getline(file, line);) {

				if(!has_ply) {
					if(line == "ply") {
						has_ply = true;
						continue;
					}
					status.id = Status::MISSING_PLY;
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
						is_lendian = false;
					} else if(word == "format") {
						status.id = Status::INVALID_FORMAT;
						status.context = line;
						break;
					} else {
						status.id = Status::MISSING_FORMAT;
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
					status.id = Status::INVALID_ELEMENT;
					status.context = line;
					break;
				}

				if(word == "property") {
					if(!is_element) {
						status.id = Status::ORPHAN_PROPERTY;
						status.context = line;
						break;
					}
					if(iss >> word) {
						Primitive::ID value, size;
						if(word == "list") {
							std::string t1, t2, name;
							if(iss >> t1 >> t2 >> name) {
								if(!Primitive::getID(t1, size)) {
									status.id = Status::INVALID_TYPE;
									status.details = t1;
									status.context = line;
									break;
								}
								if(!Primitive::getID(t2, value)) {
									status.id = Status::INVALID_TYPE;
									status.details = t2;
									status.context = line;
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
									status.id = Status::INVALID_TYPE;
									status.context = line;
									status.details = word;
									break;
								}
								el.properties.emplace_back(
										Property{name, false, value, value});
								continue;
							}
						}
					}
					status.id = Status::INVALID_PROPERTY;
					status.context = line;
					break;
				}
				status.id = Status::UNEXPECTED;
				status.context = line;
				status.details = word;
				break;
			}
			file.close();

			if(status) {
				if(!has_ply) {
					status.id = Status::MISSING_PLY;
				} else if(!has_fmt) {
					status.id = Status::MISSING_FORMAT;
				} else if(!has_endh) {
					status.id = Status::MISSING_END_HEADER;
				}
			}
			if(!status) {
				return;
			}

			file.open(fname, is_ascii ? (std::ios::in) : 
					(std::ios::in|std::ios::binary));

			if(!file.is_open()) {
				status.id = Status::MISSING_FILE;
				return;
			}
			file.seekg(end_header);

			for(auto &el : elements) {
				for(int el_i = 0, el_n = el.instances;
						status && el_i < el_n; ++el_i) {
					for(auto &prop : el.properties) {
						if(ascii && !el.readProperty(file, prop)) {
							status.id = Status::EARLY_EOF;
							std::ostringstream oss;
							oss << "while parsing " << el.name
								<< "[" << el_i << "/" << el_n << "]." 
								<< prop.name;
							status.details = oss.str();
							break;
						}
					}
				}
				if(!status) {
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
