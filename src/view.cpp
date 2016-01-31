#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../inc/view.hpp"

namespace View {

	void view::project(int w, int h) {
		static float t = 0;
		t += 0.05f;
		float aspect = h/float(w),
			  fov = float(1 / tan(60 * M_PI / 180)),
			  near = 1.0f, far = 10.0f,
			  x = fov*aspect, y = fov, 
			  z1 = (far+near)/(far-near),
			  z2 = 2*far*near/(far-near),
			transformData[] = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				-2, 0,-5, 1
			}, frustumData[] = {
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0,z1,-1,
				0, 0,z2, 0
			};

		glUniformMatrix4fv(transformID, 
				1, GL_FALSE, transformData);
		glUniformMatrix4fv(frustumID, 
				1, GL_FALSE, frustumData);
	}
	bool view::attach(const char *vPath, const char *fPath) {
		if(!shader.loadFromFile(vPath, fPath)) {
			return false;
		}
		sf::Shader::bind(&shader);
		glGetIntegerv(GL_CURRENT_PROGRAM, &progID);
		transformID = glGetUniformLocation(progID, "transform");
		frustumID = glGetUniformLocation(progID, "frustum");
		return true;
	}
	
	void view::redraw(void) {
		if(win.isOpen()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vbuf);
			glVertexAttribPointer(0, 3, GL_FLOAT, 
					GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
			glDrawElements(GL_TRIANGLES, 12*3,
					GL_UNSIGNED_INT, (void*)0);
			glDisableVertexAttribArray(0);

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

		bool live = true;
		while(live) {
			while(live && win.pollEvent(ev)) {
				switch(ev.type) {
					case sf::Event::Closed:
						live = false;
						break;
					case sf::Event::Resized: {
						auto sz = ev.size;
						glViewport(0, 0, sz.width, sz.height);
						break;
					}
					default:
						break;
				}
			}
			if(live) {
				update();
				redraw();
			}
		}
	}

	view::view(int w, int h, const char *title):
		win(sf::VideoMode(w,h), 
			title, sf::Style::Default,
			sf::ContextSettings(24, 8, 4, 3, 0)) {
		
		glewExperimental = GL_TRUE;
		glewInit();

		glGenVertexArrays(1, &vaID);
		glBindVertexArray(vaID);

		const GLfloat vbufData[] = {
			+1,+1,+1, // E U N 
			+1,+1,-1, // E U S
			+1,-1,+1, // E D N
			+1,-1,-1, // E D S
			-1,+1,+1, // W U N
			-1,+1,-1, // W U S
			-1,-1,+1, // W D N
			-1,-1,-1, // W D S
		};
		const GLuint ibufData[] = {
			//{ 1, 0, 2, 3 } E
			1,2,0, 1,3,2,
			//{ 0, 1, 5, 4 } U
			0,5,1, 0,4,5,
			//{ 4, 5, 7, 6 } W
			4,7,5, 4,6,7,
			//{ 6, 7, 3, 2 } D
			6,3,7, 6,2,3,
			//{ 2, 0, 4, 6 } N
			2,4,0, 2,6,4,
			//{ 3, 7, 5, 1 } S
			3,5,7, 3,1,5
		};

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
		win.setActive(false);
	}
	view::~view(void) {
		if(win.isOpen()) {
			win.close();
		}
	}

	int test(void) {
		return 0;
	}
}
