# GLOMP
Asynchronous model/view/controller engine; under construction.

#### Requirements:
* OpenGL 3.30 Core or better
* [GL3W](https://github.com/skaslev/gl3w) -- OpenGL extension loading
* [GLFW](https://github.com/glfw/glfw) -- windows with OpenGL and event handling
* [Boost](https://github.com/boostorg/boost) -- version 1.60 or greater for unit testing

#### Building:
1. Create directories bin, lib, and gl3w
2. In gl3w...
 a. Clone from the GL3W repository
 b. Run `gl3w_gen.py`
3. Make: `make`
