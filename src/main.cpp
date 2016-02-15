#include "../inc/main.hpp"
#include "../inc/model.hpp"
#include "../inc/view.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include <X11/Xlib.h>
#include <SFML/Graphics.hpp>

int main(int argc, const char **argv) {
	/*Model::model m;
	Model::ply("resources/test.ply", m);*/

	XInitThreads();

	std::atomic_bool alive(true);

	sf::VideoMode mode(512, 512);
	auto style = sf::Style::Default;
	sf::ContextSettings ctx(24, 8, 0, 3, 3, 1);
	sf::RenderWindow win(mode, "View", style, ctx);

	auto viewCB = []{};
	auto modelCB = [&alive]{
		static int t = 0;
		std::chrono::milliseconds ms(1000);
		while(alive) {
			std::cout << "Model update #" << t++ << std::endl;
			if(t == 10) {
				alive = false;
				break;
			}
			std::this_thread::sleep_for(ms);
		}
		
	};

	win.setFramerateLimit(60);
	View::view display(win, alive);
	if(display.valid) {
		std::thread modelThread(modelCB);
		display.run(viewCB);
		modelThread.join();
	}

	return 0;
}
