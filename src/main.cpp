#include <chrono>
#include <iostream>
#include <thread>

#include "../inc/main.hpp"
#include "../inc/model.hpp"
#include "../inc/view.hpp"

int main(int argc, const char **argv) {

	View::view display(512, 512, "View");
	
/*	auto modelFn = [](void* data) {
		// Update model
		bool cond = true;
		auto alive = (View::GateCond*) data;
		while(cond) {
			if(SDL_LockMutex(alive -> gate) == 0) {
				cond = alive -> value;
				SDL_UnlockMutex(alive -> gate);
			}
			if(cond) {
				SDL_Delay(100);
			}
		}
		std::cout << "Model shutting down." << std::endl;
		return 0;
	};

	auto modelThread = SDL_CreateThread(
			modelFn, "Model", &display.alive);*/
	display.run([]{}, 60);
	SDL_Delay(100);
	return 0;
}
