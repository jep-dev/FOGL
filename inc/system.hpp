#include <fstream>
#include <string>
#include <sstream>
#include <vector>

namespace System {
	using std::vector;
	using std::string;
	using std::istringstream;
	using std::ifstream;
	bool readFile(const char *fname, string &lines);
	bool readFile(const char *fname, vector<string> &lines);
	int split(string const& line, vector<string> &words);
}
