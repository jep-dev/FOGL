#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../inc/view.hpp"

void View::test(void) {
	sf::VideoMode mode(256, 256, 32);
	sf::RenderWindow win(mode, "Title");

	sf::Event ev;
	while(win.isOpen()) {
		while(win.pollEvent(ev)) {
			switch(ev.type) {
				case sf::Event::Closed:
					win.close();
					break;
				default:
					break;
			}
		}
		if(!win.isOpen())
			break;

		win.clear();
		win.display();

		sf::sleep(sf::milliseconds(200));
	}
}
