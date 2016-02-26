#include "system.hpp"

namespace System {
	bool readFile(const char *fname, string &lines) {
		ifstream file(fname);
		if(!file.is_open()) {
			return false;
		}
		for(string line; std::getline(file, line);) {
			lines.append(line + "\r\n");
		}
		file.close();
		return true;
	}
	bool readFile(const char *fname, vector<string> &lines) {
		ifstream file(fname);
		if(!file.is_open()) {
			return false;
		}
		for(string line; std::getline(file, line);) {
			lines.push_back(line);
		}
		file.close();
		return true;
	}

	int split(string const& line, vector<string> &words) {
		int wc = 0;
		string word;
		istringstream iss(line);
		while(iss >> word) {
			words.push_back(word);
			wc++;
		}
		return wc;
	}
};
