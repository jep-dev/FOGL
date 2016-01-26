#ifndef VIEW_HPP
#define VIEW_HPP

#include <iostream>

#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace View {
	void redraw(sf::RenderWindow&);
	void run(sf::RenderWindow&, void (*)(void));
	int test(void);
}

#endif
