#include <iostream>

#include "main.hpp"
#include "util.hpp"

#ifndef VERT_PATH
#define VERT_PATH "share/fallback.vert"
#endif
#ifndef FRAG_PATH
#define FRAG_PATH "share/shade.frag"
#endif

void printErrors(std::ostream &oss) {}

template<typename T1, typename... TN>
void printErrors(std::ostream &oss, T1 &t1, TN &... tn) {
	for(auto e : t1) {
		oss << e << std::endl;
	}
	printErrors(oss, tn...);
}

int main(int argc, const char *argv[]) {
	const char *obj_fname = nullptr,
		  *vert_fname = nullptr,
		  *frag_fname = nullptr;
	if(argc >= 2) obj_fname = argv[1];
	//else obj_fname = OBJ_PATH;
	if(argc >= 3) vert_fname = argv[2];
	else vert_fname = VERT_PATH;
	if(argc >= 4) frag_fname = argv[3];
	else frag_fname = FRAG_PATH;
	if(argc >= 5) {
		std::cout << "Usage: " << argv[0] <<
			" [obj_fname [vert_fname [frag_fname]]]" << std::endl;
		return 1;
	}

	using namespace Model;
	model model;
	if(obj_fname) {
		obj_t obj;
		auto status = obj_t::load(obj_fname, obj);
		if(status != obj_t::e_ok) {
			std::cout << "Failed to load model "
				<< obj_fname << std::endl;
			return 1;
		}
		model = obj;
		std::cout << "Initialized model with obj "
			<< obj_fname << '.' << std::endl;
	} else {
		model = mesh_t(25, 25,
		[](float s, float t, std::vector<float> &vertices) {
			auto theta = s*M_PI*2, phi = t*M_PI;
			vertices.emplace_back(cos(theta)*sin(phi)); // X
			vertices.emplace_back(sin(theta)*sin(phi)); // Y
			vertices.emplace_back(cos(phi));            // Z
		});
		std::cout << "Initialized model with ad-hoc mesh." << std::endl;
	}

	std::atomic_bool alive(true);

	View::view view(alive, vert_fname, frag_fname);
	if(!alive) {
		std::cout << "View construction failed." << std::endl;
		printErrors(std::cout, view.errors);
		return 1;
	} else if(!view.init(alive)) {
		std::cout << "View initialization failed." << std::endl;
		printErrors(std::cout, view.errors);
		return 1;
	} else {
		int gl_major, gl_minor, glfw_major, glfw_minor, glfw_rev;
		glGetIntegerv(GL_MAJOR_VERSION, &gl_major);
		glGetIntegerv(GL_MINOR_VERSION, &gl_minor);
		glfwGetVersion(&glfw_major, &glfw_minor, &glfw_rev);
		std::cout << "Initialized view with OpenGL "
			<< gl_major << '.' << gl_minor << " and GLFW "
			<< glfw_major << '.' << glfw_minor << '.' << glfw_rev
			<< '.' << std::endl;
	}
	
	Control::control ctl(alive, model, view);
	if(!alive) {
		std::cout << "Control construction failed." << std::endl;
		printErrors(std::cout, ctl.errors);
		return 1;
	} else if(!ctl.init(alive)) {
		std::cout << "Control initialization failed." << std::endl;
		printErrors(std::cout, view.errors, ctl.errors);
		return 1;
	}
	std::cout << "Initialized control." << std::endl;
	if(!ctl.run(alive)) {
		printErrors(std::cout, view.errors, ctl.errors);
	}
	return 0;
}
