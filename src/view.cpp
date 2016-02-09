#include "../inc/view.hpp"
#include "../inc/shade.hpp"

namespace View {

	void printErrors(const char *prefix = "") {
		bool once = false;
		GLenum err;
		while((err = glGetError()) != GL_NO_ERROR) {
			if(!once) {
				std::cout << prefix;
				once = true;
			}
			switch(err) {
			case GL_INVALID_ENUM:
				std::cout << "invalid enum; ";
				break;
			case GL_INVALID_VALUE:
				std::cout << "invalid value; ";
				break;
			case GL_INVALID_OPERATION:
				std::cout << "invalid operation; ";
				break;
			default:
				break;
			}
		}
		 
		if(once) {
			endl(std::cout);
		}
	}

	void view::project(int w, int h) {
		static float t = 0;
		t += float(M_PI/180);

		const float aspect = h/float(w),
			fov = float(1 / tan(45 * M_PI / 180)),
			near = 1.0, far = 100.0,
			x = fov*aspect, y = fov, 
			z1 = (far+near)/(far-near),
			z2 = 2*far*near/(far-near),
			t_x = float(cos(t)*5), 
			t_y = float(sin(t)*5),
			t_z = -10,
			transformData[] = {
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				t_x, t_y, t_z, 1.0
			}, 
			xyData[] = { x,  0.0, 0.0,   y},
			zwData[] = {z1, -1.0,  z2, 0.0};

		glUniformMatrix4fv(transformID, 1, 
				GL_FALSE, transformData);
		glUniformMatrix2fv(projXYID, 1, GL_FALSE, xyData);
		glUniformMatrix2fv(projZWID, 1, GL_FALSE, zwData);
	}
	
	void view::redraw(void) {
		if(!done) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vbuf);
			glVertexAttribPointer(0, 3, GL_FLOAT, 
					GL_FALSE, 0, (void*) 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
			glDrawElements(GL_TRIANGLES, 12*3,
					GL_UNSIGNED_INT, (void*) 0);
			glDisableVertexAttribArray(0);
			auto sz = win.getSize();
			project(sz.x, sz.y);
			win.display();
		}
	}

	void view::run(void (*update)(void), int rate) {
		sf::Event ev;
		win.setActive(true);
		win.setFramerateLimit(rate);

		while(!done) {
			while(win.pollEvent(ev)) {
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
			if(done) {
				break;
			}
			update();
			redraw();
		}
	}

	view::view(int w, int h, const char *title):
		win(sf::VideoMode(w,h), 
			title, sf::Style::Default,
			sf::ContextSettings(24, 8, 0, 3, 0)) {
		
		glewExperimental = GL_TRUE;
		glewInit();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		
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
			1,0,2, 1,2,3, // E
			0,1,5, 0,5,4, // U
			4,5,7, 4,7,6, // W
			6,7,3, 6,3,2, // D
			2,0,4, 2,4,6, // N
			3,7,5, 3,5,1  // S
		};
		glGenBuffers(1, &vbuf);
		glBindBuffer(GL_ARRAY_BUFFER, vbuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vbufData), 
				vbufData, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &ibuf);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibufData),
				ibufData, GL_DYNAMIC_DRAW);

		progID = glCreateProgram;
		const char *vName = "resources/shade.vert",
			  *fName = "resources/shade.frag";
		if(!link(vName, fName, progID)) {
			done = true;
			return;
		}
		transformID = glGetUniformLocation(progID, "transform");
		projXYID = glGetUniformLocation(progID, "projXY");
		projZWID = glGetUniformLocation(progID, "projZW");
		glUseProgram(progID);
	}
	view::~view(void) {
		if(progID != 0) {
			glDeleteProgram(progID);
		}
		if(win.isOpen()) {
			win.close();
		}
	}
}
