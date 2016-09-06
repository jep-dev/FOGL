#ifndef VIEW_SHADE_HPP
#define VIEW_SHADE_HPP

#include <string>
#include <vector>

namespace View {
	bool compile(const char*, GLuint&, std::vector<std::string>&);
	bool link(const char*, const char*, GLuint&, std::vector<std::string>&);
}

#endif
