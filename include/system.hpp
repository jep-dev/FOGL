#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace System {

	// TODO Remove and use ifstream for each time?
	/*! Reads the contents of a file into a single packed string
 	 * \param fname The path to a file
 	 * \param lines The entire contents of the file
 	 */
	bool readFile(const char *fname, std::string &lines);

	/// \copydoc readFile
	bool readFile(const char *fname, std::vector<std::string> &lines);
	std::string repeat(int w, char c = ' ');
	
	// TODO Replace with Boost::Regex or Tokenizer?
	/// Splits a string along tokens
	int split(std::string const &line, std::vector<std::string> words);

	// TODO Replace 'stringify' template with standard insertion operator?
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
