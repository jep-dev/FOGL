#include "main.hpp"
#include "model.hpp"
#include "view.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

int main(int argc, const char **argv) {
	if(!glfwInit()) {
		std::cout << "Could not initialize GLFW." << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, 
			GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *win = glfwCreateWindow(512, 512, 
			"View", NULL, NULL);

	if(!win) {
		std::cout << "Could not create window." << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(win);
	if(gl3wInit()) {
		std::cout << "Could not initialize gl3w." << std::endl;
		return -1;
	}

	std::atomic_bool alive(true);
	std::chrono::milliseconds ms(1000);
	auto viewCB = []{};
	auto modelCB = [&alive, &ms]{
		static int t = 0;
		while(alive) {
			std::cout << "Model update #" << t++ << std::endl;
			std::this_thread::sleep_for(ms);
		}
		
	};

	View::view display(win, alive);
	if(display.valid) {
		std::thread modelThread(modelCB);
		display.run(viewCB);
		modelThread.join();
	}

	glfwTerminate();

	return 0;
}
