#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace System {
	/// \copydoc readFile
	bool readFile(const char *fname, std::vector<std::string> &lines);
	std::string repeat(int w, char c = ' ');
	
	/// Splits a string along tokens
	int split(std::string const &line, std::vector<std::string> words);

	/*! String cast (lexical cast operator) template
 	 * \tparam The type of the instance to convert to a string
 	 */
	template<class T> std::string stringify(const T &t);

	/// \copydoc stringify
	template<class T1, class T2, class... TN>
	std::string stringify(const T1 &t1, const T2 &t2, const TN &... tn);
}

#include "system/printer.hpp"
#include "system/net.hpp"


#endif
