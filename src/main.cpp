#include "../inc/main.hpp"
#include "../inc/control.hpp"
#include "../inc/model.hpp"
#include "../inc/view.hpp"

#include <X11/Xlib.h>
#include <thread>
#include <iostream>

#include <SFML/System/Time.hpp>
#include <SFML/System/Sleep.hpp>

int main(int argc, const char **argv) {
	XInitThreads();
	View::view display(512, 512, "View");
	
	display.win.setActive(false);
	auto runFrame = []{
		// One view frame
	};
	auto runModel = [&display]{
		// One model frame
		display.win.setActive(false);
		static int t = 0;
		while(!display.done) {
			std::cout << "Model: t = " << t++ << std::endl;
			sf::sleep(sf::milliseconds(1000));
		}
	};
	auto runView = [&display, &runFrame]{
		display.run(runFrame, 1);
	};

	std::thread viewThread(runView), 
		modelThread(runModel);

	viewThread.join();
	modelThread.join();

	return 0;
}
