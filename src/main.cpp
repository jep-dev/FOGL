#include "main.hpp"
#include "model.hpp"
#include "view.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

//#include <X11/Xlib.h>

int main(int argc, const char **argv) {
	/*Model::model m;
	Model::ply("resources/test.ply", m);*/

	//XInitThreads();

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

	//win.setFramerateLimit(60);
	View::view display(win, alive);
	if(display.valid) {
		std::thread modelThread(modelCB);
		display.run(viewCB);
		modelThread.join();
	}

	glfwTerminate();

	return 0;
}
