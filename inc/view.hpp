#ifndef VIEW_HPP
#define VIEW_HPP

#include <iostream>
#include <X11/Xlib.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

namespace View {

	struct GateCond {
		bool value;
		SDL_mutex *gate;
		GateCond(bool def = false):
			value(def), 
			gate(SDL_CreateMutex()) {}
		virtual ~GateCond(void) {
			if(gate) {
				SDL_DestroyMutex(gate);
			}
		}
	};

	struct view {
		GateCond alive;
		GLuint progID, vaID, vbuf, ibuf, 
			   transformID, projXYID, projZWID;
		SDL_Window *win;
		SDL_GLContext context;
		void project(int w, int h);
		void redraw(void);
		void run(void (*)(void), int rate = 60);
		view(int w, int h, const char *title);
		virtual ~view(void);
	private:
		bool done = false;
	};
}

#endif
