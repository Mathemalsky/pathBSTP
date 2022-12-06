# pathBSTP

## running the software

### prerequisites
- [cmake](https://cmake.org/) is required for compiling
- [dear imgui](https://github.com/ocornut/imgui) is used for graphical output. dear imgui is contained as git submodule but it needs
- [glfw](https://www.glfw.org/) which needs
- [glew](https://github.com/nigels-com/glew) (can be installed by `sudo apt-get install libglew-dev`)
- [doxygen](https://www.doxygen.nl/) This is optinal for generating documentation.
- [Eigen3](https://eigen.tuxfamily.org/) Used as implementation for sparse matrices.

### building the software
```
mkdir build && cd build
cmake ..
make
```
