#ifndef VIEW_HPP
#define VIEW_HPP

#include <atomic>
#include <iostream>

#include <GL/glew.h>
#include <SFML/Graphics.hpp>

namespace View {

	struct view {
		bool valid = false;
		std::atomic_bool &alive;
		GLuint progID, vaID, vbuf, ibuf, 
			   transformID, projXYID, projZWID;
		sf::RenderWindow &win;
		void project(int w, int h);
		void redraw(void);
		void run(void (*)(void));
		view(sf::RenderWindow &win,
				std::atomic_bool &alive);
		virtual ~view(void);
	};
}

#endif
