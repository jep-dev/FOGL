#include <fstream>
#include "system.hpp"

namespace System {
	bool Printer_Base::uni_special(char ch) {
		return (ch & 0xc0) == 0x80;
	}
	int Printer_Base::uni_strlen(const char *word) {
		int len = 0;
		for(; *word; word++) {
			if(!uni_special(*word)) {
				len++;
			}
		}
		return len;
	}
	int Printer_Base::uni_strlen(const std::string &word) {
		return uni_strlen(word.c_str());
	}

	int Printer_Base::strlen(const char *word) {
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

	std::string Printer_Base::repeat(int w, char c) {
		std::ostringstream oss;
		oss << std::setw(w) << std::setfill(c);
		return oss.str();
	}

	template<typename T>
	std::string Printer_Base::stringify(const T &t) {
		std::ostringstream oss;
		oss << t;
		return oss.str();
	}
	template<typename T1, typename T2, typename... TN>
	std::string Printer_Base::stringify(const T1 &t1,
			const T2 &t2, const TN &... tn) {
		return stringify(t1) + " " + stringify(t2, tn...);
	}

	int Printer_Base::split(std::string const& line,
			std::vector<std::string> &words) {
		int wc = 0;
		std::string word;
		std::istringstream iss(line);
		while(iss >> word) {
			words.push_back(word);
			wc++;
		}
		return wc;
	}
}
