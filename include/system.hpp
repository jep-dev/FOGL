#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <string>
#include <vector>

namespace System {
	std::string repeat(int w, char c = ' ');
	
	/// Splits a string along tokens
	int split(std::string const &line, std::vector<std::string> words);
}

#include "system/printer.hpp"
#include "system/net.hpp"


#endif
