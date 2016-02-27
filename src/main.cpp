#include "main.hpp"
#include "model.hpp"
#include "view.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

int main(int argc, const char **argv) {
	using msec=std::chrono::milliseconds;


	// Shared state
	std::atomic_bool alive(true);

	// Delay between model updates
	msec delay(1000);


	// Update view based on state
	auto updateView = [&alive]() -> bool {
		return alive;
	};
	auto quit = [&alive]() -> void {
		alive = false;
	};


	// Update model based on state & delay
	auto updateModel = [&alive, &delay]{
		static int t = 0;
		while(alive) {
			std::cout << "Model update #" << t++ << std::endl;
			std::this_thread::sleep_for(delay);
		}
	};

	View::view display;
	if(display.valid) {
		std::thread modelThread(updateModel);
		display.run(updateView, quit);
		modelThread.join();
	}

	glfwTerminate();

	return 0;
}
