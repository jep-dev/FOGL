#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../inc/view.hpp"

namespace View {

	void redraw(sf::RenderWindow & win) {
		if(win.isOpen()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			win.display();
			sf::sleep(sf::milliseconds(200));
		}
	}


	void run(sf::RenderWindow &win,
			void (*update)(void)) {
		sf::Event ev;
		
		glViewport(0, 0, 1, 1);

		bool live = true;
		while(live) {
			while(win.pollEvent(ev)) {
				switch(ev.type) {
					case sf::Event::Closed:
						live = false;
						break;
					case sf::Event::Resized:
						break;
					default:
						break;
				}
			}
			if(live) {
				update();
				redraw(win);
			} else {
				win.close();
				break;
			}
		}
	}

	int test(void) {
		sf::VideoMode mode(256, 256, 32);
		sf::ContextSettings settings{24,8,0,3,0};
		sf::RenderWindow win(mode, "Title", 
				sf::Style::Default, settings);
		
		run(win, [](void){
			static int t = 0;
			std::cout << "Frame " << t++ << std::endl;
		});
		return 0;
	}
}
