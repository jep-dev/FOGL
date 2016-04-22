#include "system.hpp"

namespace System {
	bool readFile(const char *fname, std::string &lines) {
		std::ifstream file(fname);
		if(!file.is_open()) {
			return false;
		}
		for(std::string line; std::getline(file, line);) {
			lines.append(line + "\r\n");
		}
		file.close();
		return true;
	}
	bool readFile(const char *fname, std::vector<std::string> &lines) {
		std::ifstream file(fname);
		if(!file.is_open()) {
			return false;
		}
		for(std::string line; std::getline(file, line);) {
			lines.push_back(line);
		}
		file.close();
		return true;
	}

	int split(std::string const& line, std::vector<std::string> &words) {
		int wc = 0;
		std::string word;
		std::istringstream iss(line);
		while(iss >> word) {
			words.push_back(word);
			wc++;
		}
		return wc;
	}
};
