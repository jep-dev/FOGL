#include "../inc/system.hpp"

namespace System {
	bool readFile(const char *fname, std::string &lines) {
		std::ifstream file(fname);
		if(file.is_open()) {
			std::string line;
			while(!file.eof() && std::getline(file, line)) {
				lines.append(line + "\r\n");
			}
			file.close();
			return true;
		}
		return false;
	}
	bool readFile(const char *fname, 
			std::vector<std::string> &lines) {
		std::ifstream file(fname);
		if(file.is_open()) {
			std::string line;
			while(!file.eof() && std::getline(file, line)) {
				lines.push_back(line);
			}
			file.close();
			return true;
		}
		return false;
	}
};
