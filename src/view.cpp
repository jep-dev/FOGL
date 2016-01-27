#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../inc/view.hpp"

namespace View {

	float aspect(int w, int h) {
		return w/float(h);
	}
	void project(sf::RenderWindow & win, 
			int w, int h) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, aspect(w,h), 1, 100);
	}

	void redraw(sf::RenderWindow & win) {
		if(win.isOpen()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glColor3f(0.6, 0.3, 0.3);
			glBegin(GL_TRIANGLES);
			glVertex3f(-0.25, 0, -0.25);
			glVertex3f(0, 0, 0.25);
			glVertex3f(0.25, 0, -0.25);
			glEnd();

			auto sz = win.getSize();
			project(win, sz.x, sz.y);
			gluLookAt(0,-1,0, 0,1,0, 0,0,1);
			win.display();
		}
	}


	void run(sf::RenderWindow &win,
			void (*update)(void)) {
		sf::Event ev;
		
		win.setVerticalSyncEnabled(true);
		win.setFramerateLimit(60);

		bool live = true;
		while(live) {
			while(live && win.pollEvent(ev)) {
				switch(ev.type) {
					case sf::Event::Closed:
						live = false;
						break;
					case sf::Event::Resized: {
						auto sz = ev.size;
						glViewport(0, 0, sz.width, sz.height);
						break;
					}
					default:
						break;
				}
			}
			if(live) {
				update();
				redraw(win);
			}
		}
	}

	int test(void) {
		static sf::VideoMode vmode(640,480);
		static sf::RenderWindow win(vmode, 
				"Title", sf::Style::Default, 
				sf::ContextSettings(24, 8, 0, 3, 0));

		run(win, [](void){});
		return 0;
	}
}
