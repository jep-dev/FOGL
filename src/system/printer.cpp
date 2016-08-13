#include "system.hpp"
#include <fstream>

namespace System {
	bool uni_special(char ch) {
		return (ch & 0xc0) == 0x80;
	}
	int uni_strlen(const char *word) {
		int len = 0;
		for(; *word; word++) {
			if(!uni_special(*word)) {
				len++;
			}
		}
		return len;
	}
	int uni_strlen(const std::string &word) {
		return uni_strlen(word.c_str());
	}

	int strlen(const char *word) {
		int len = 0;
		bool escape = false, bracket = false;
		for(; *word; word++) {
			char ch = *word;
			if(!uni_special(ch)) {
				if(bracket) {
					if(ch == 'm') {
						bracket = escape = false;
					}
				} else if(escape) {
					if(ch == '[') {
						bracket = true;
					}
				} else if(ch == '\e') {
					escape = true;
				} else {
					len++;
				}
			}
		}
		return len;
	}

	std::string repeat(int w, char c = ' ') {
		std::ostringstream oss;
		oss << std::setw(w) << std::setfill(c);
		return oss.str();
	}

	template<typename T>
	std::string stringify(const T &t) {
		std::ostringstream oss;
		oss << t;
		return oss.str();
	}
	template<typename T1, typename T2, typename... TN>
	std::string stringify(const T1 &t1, const T2 &t2, const TN &... tn) {
		return stringify(t1) + " " + stringify(t2, tn...);
	}

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
