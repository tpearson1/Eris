# Introduction
Eris Engine is a lightweight package based game engine built using C++.

# Installation
Eris is currently only supported on Linux.

To begin, clone the repository, install any needed dependencies listed below,
and type `make` to compile the core package and the 'load' program. To compile
and run a package called 'x', type `./load -c x` from the project directory.

## Dependencies
* [Rapidjson](https://github.com/Tencent/rapidjson) is used for the
serialization system, and is a submodule for this repository. If you don't
initialize and update the git submodules, Eris won't compile.
* The package system currently uses [Make](https://www.gnu.org/software/make)
and [g++](https://gcc.gnu.org), so these are required to
compile and use Eris.
* [OpenGL](https://www.opengl.org), [GLFW](https://github.com/glfw/glfw),
and [GLEW](http://glew.sourceforge.net) need to be installed to use the 'base'
package.
* To load images, [libpng](http://www.libpng.org/pub/png/libpng.html) is used.
To use the 'base' package, this library must be installed.
* The [Open Asset Import Library](https://github.com/assimp/assimp) must be
present for use of the 'base' package.
* To use be able to create and run tests, you will need to have the single header
for [Catch](https://github.com/philsquared/Catch) accessible from a default
include directory.

# License
Eris is licensed under the MIT license. The license can be found
[here](LICENSE.md).
