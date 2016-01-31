#include "../inc/main.hpp"
#include "../inc/control.hpp"
#include "../inc/model.hpp"
#include "../inc/view.hpp"

#include <future>
#include <functional>

#include <iostream>

int main(int argc, const char **argv) {
	View::view v(512, 512, "View");
	std::async(std::launch::async, [&v]{
		v.run([](void){});
	});

	return 0;
}
