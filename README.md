# FOGL
Asynchronous model/view/controller engine/framework; under construction.

#### Requirements:
* Python 2.7 or 3.4 -- for GL3W
* [OpenMP](www.openmp.org) -- for future parallel computation
* [Clang](clang.llvm.org) or any compiler with support for C++11 and OpenMP
* [OpenGL](https://www.opengl.org) 3.30 Core or better
* [OpenGL](http://glu.g-truc.net/0.1.0/index.html) -- Utility Library (GLU)
* [GL3W](https://github.com/skaslev/gl3w) -- OpenGL extension loading
* [GLFW](https://github.com/glfw/glfw) -- windows with OpenGL and event handling
* [SOIL](https://www.opengl.org/wiki/Image_Libraries#SOIL) -- Simple OpenGL Image Library
* [Boost](https://github.com/boostorg/boost) -- versatile C++ support library
* [Doxygen](https://github.com/doxygen/doxygen) (optional) -- see Documentation section
* [Graphviz](http://www.graphviz.org/) (optional) -- see Documentation section

Several of these requirements can be installed via the command line rather than manually (download/configure/build/install):

`sudo apt-get install build-essential clang libomp-dev`

`sudo apt-get install python3 libboost-all-dev libgl1-mesa-dev libglu1-mesa-dev libsoil-dev`

`sudo apt-get install doxygen graphviz texlive-latex-base latex-make latex-mk`

You cannot install gl3w the same way; it must be run locally (see Building)

#### Documentation
Comments for declarations are kept in the project headers, and use the QT variant of Doxygen formatting. To generate HTML and/or LaTex from these comments, simply run `doxygen` from the `doc` directory, with or without modifying the Doxyfile to your liking.

#### Building:

1. Run `git clone https://github.com/skaslev/gl3w ../gl3w`

2. In `../gl3w/`, run `python gl3w_gen.py` or `python3 gl3w_gen.py` to generate OpenGL headers

3. Run `git clone https://github.com/boostorg/boost ../boost`

4. In `../boost/`, run `bootstrap.sh`, then `b2`

5. In `./`, Update `Makefile` as necessary, then run `make`

6. Run one of the following:

 a. `ln -s ../gl3w/libgl3w.so your_path` where `your_path` is a directory in your `LD_LIBRARY_PATH`

 b. Add `:../gl3w/` to the tail of your `LD_LIBRARY_PATH`

The Makefile targets gl3w for convenience, but building gl3w separately is always an option. OpenGL and GLU are not provided by gl3w; Boost and other dependencies are not targeted and must be met or made independently.

All of the requirements support Linux, Windows, and OS X. This project only supports Linux, but will support Windows XP or better before release. 

#### Todo:
- [ ] include/util/types.hpp (152) Distinguish between permutations with and without duplicates; the current permutation test relies on the exclusive-or of two parameter packs, not necessarily sets
- [x] include/system.hpp (13) Remove file reading function and more in favor of in-place ifstream, etc.
- [ ] Support multiple groups/objects in Wavefront obj
- [ ] Support mtl for Wavefront obj
- [ ] Migrate from GLFW to GTK
