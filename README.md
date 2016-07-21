# GLOMP
Asynchronous model/view/controller engine/framework; under construction.

#### Requirements:
* Python 2.7 or 3.4
* OpenGL 3.30 Core or better
* OpenGL Utility Library (GLU)
* [GL3W](https://github.com/skaslev/gl3w) -- OpenGL extension loading
* [GLFW](https://github.com/glfw/glfw) -- windows with OpenGL and event handling
* [Boost](https://github.com/boostorg/boost) -- versatile C++ support library

#### Building:

1. Run `git clone https://github.com/skaslev/gl3w ../gl3w`

2. In `../gl3w/`, run `gl3w_gen.py` to generate OpenGL headers

3. Run `git clone https://github.com/boostorg/boost ../boost`

4. In `../boost/`, run `bootstrap.sh`, then `b2`

5. In `./`, Update `Makefile` as necessary, then run `make`

The Makefile targets gl3w for convenience, but building gl3w separately is always an option. OpenGL and GLU are not provided by gl3w; Boost and other dependencies are not targeted and must be met or made independently.

All of the requirements support Linux, Windows, and OS X. This project only supports Linux, but will support Windows XP or better before release. 
