#ifndef VIEW_HPP
#define VIEW_HPP

#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

namespace View {

	struct view {
		bool initialized = false,
			 done = false;
		GLint progID;
		GLuint vaID, vbuf, ibuf, 
			   transformID, projXYID, projZWID;
		sf::RenderWindow win;
		sf::Shader shader;
		void project(int w, int h);
		void redraw(void);
		bool attach(const char *vPath, const char *fPath);
		void run(void (*)(void), int rate = 60);
		view(int w, int h, const char *title);
		virtual ~view(void);
	};
}

#endif
