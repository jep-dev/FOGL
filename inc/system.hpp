#include <fstream>
#include <string>
#include <vector>

namespace System {
	bool readFile(const char *fname, 
			std::string &lines);
	bool readFile(const char *fname, 
			std::vector<std::string> &lines);
}
