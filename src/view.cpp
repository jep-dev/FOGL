#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../inc/view.hpp"

namespace View {

	void printErrors(const char *prefix = "") {
		GLenum err;
		std::string errmsg;
		while((err = glGetError()) != GL_NO_ERROR) {
			switch(err) {
				case GL_INVALID_ENUM:
					{errmsg = "GL Invalid Enum";} break;
				case GL_INVALID_VALUE:
					{errmsg = "GL Invalid Value";} break;
				case GL_INVALID_OPERATION:
					{errmsg = "Invalid Operation";} break;
				default:
					{errmsg = "Unhandled GL Error";} break;
			}
			std::cout << prefix << errmsg << std::endl;
		}
	}

	void view::project(int w, int h) {
		static float t = 0;
		t += 0.1f;
		const float aspect = h/float(w),
			fov = float(1 / tan(60 * M_PI / 180)),
			near = 1.0, far = 10.0,
			x = fov*aspect, y = fov, 
			z1 = (far+near)/(far-near),
			z2 = 2*far*near/(far-near),
			transformData[] = {
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0,-5.0, 1.0
			}, 
			xyData[] = { x,  0.0, 0.0,   y},
			zwData[] = {z1, -1.0,  z2, 0.0};

		printErrors("44: ");
		glUniformMatrix4fv(transformID,
				1, GL_FALSE, transformData);
		printErrors("47: ");

		glUniformMatrix2fv(projXYID,
				1, GL_FALSE, xyData);
		printErrors("51: ");

		glUniformMatrix2fv(projZWID,
				1, GL_FALSE, zwData);
		printErrors("55: ");
	}
	bool view::attach(const char *vPath, const char *fPath) {
		if(!shader.loadFromFile(vPath, fPath)) {
			return false;
		}
		sf::Shader::bind(&shader);
		glGetIntegerv(GL_CURRENT_PROGRAM, &progID);

		//glBindFragDataLocation(progID, 0, "FragColor");
		
		printErrors("66: ");
		transformID = glGetUniformLocation(progID, "transform");
		projXYID = glGetUniformLocation(progID, "projXY");
		projZWID = glGetUniformLocation(progID, "projZW");
		GLint query;
		std::cout
 			<< "Program: " << (glGetIntegerv(GL_CURRENT_PROGRAM, 
						&query), query)	
			<< "; uniforms: " << (glGetProgramiv(progID,
					GL_ACTIVE_UNIFORMS, &query), query)
			<< "; attributes: " << (glGetProgramiv(progID,
					GL_ACTIVE_ATTRIBUTES, &query), query)
			<< std::endl;

		
		printErrors("70: ");
		return true;
	}
	
	void view::redraw(void) {
		if(!done) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vbuf);
			glVertexAttribPointer(0, 3, GL_FLOAT, 
					GL_FALSE, 0, (void*) 0);
			printErrors("82: ");
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
			glDrawElements(GL_TRIANGLES, 12*3,
					GL_UNSIGNED_INT, (void*) 0);
			glDisableVertexAttribArray(0);
			printErrors("87: ");
			auto sz = win.getSize();
			project(sz.x, sz.y);
			win.display();
		}
	}

	void view::run(void (*update)(void), int rate) {
		sf::Event ev;
		win.setActive(true);
		win.setVerticalSyncEnabled(true);
		win.setFramerateLimit(rate);

		while(!done) {
			while(!done && win.pollEvent(ev)) {
				switch(ev.type) {
					case sf::Event::Closed: {
						done = true;
						win.close();
						break; }
					case sf::Event::Resized: {
						auto sz = ev.size;
						glViewport(0, 0, sz.width, sz.height);
						break; }
					default:
						break;
				}
			}
			printErrors("115: ");
			if(!done) {
				update();
				redraw();
			}
		}
	}

	view::view(int w, int h, const char *title):
		win(sf::VideoMode(w,h), 
			title, sf::Style::Default,
			sf::ContextSettings(24, 8, 0, 3, 0)) {
		
		glewExperimental = GL_TRUE;
		glewInit();
		glEnable(GL_DEPTH_TEST);
		//glClearColor(0.1, 0.18, 0.13, 1.0);
		
		glGenVertexArrays(1, &vaID);
		glBindVertexArray(vaID);

		const GLfloat vbufData[] = {
			+1,+1,+1, // 0  E U N 
			+1,+1,-1, // 1  E U S
			+1,-1,+1, // 2  E D N
			+1,-1,-1, // 3  E D S
			-1,+1,+1, // 4  W U N
			-1,+1,-1, // 5  W U S
			-1,-1,+1, // 6  W D N
			-1,-1,-1, // 7  W D S
		};
		const GLuint ibufData[] = {
			1,2,0, 1,3,2, // E
			0,5,1, 0,4,5, // U
			4,7,5, 4,6,7, // W
			6,3,7, 6,2,3, // D
			2,4,0, 2,6,4, // N
			3,5,7, 3,1,5  // S
		};
		//glFrontFace(GL_FRONT);
		glGenBuffers(1, &vbuf);
		glBindBuffer(GL_ARRAY_BUFFER, vbuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vbufData), 
				vbufData, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &ibuf);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibufData),
				ibufData, GL_DYNAMIC_DRAW);
		attach("resources/shade.vert", 
				"resources/shade.frag");

		printErrors("167: ");
		//win.setActive(false);
	}
	view::~view(void) {
		done = true;
		if(win.isOpen()) {
			win.close();
		}
	}
}
