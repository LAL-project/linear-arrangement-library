# Installation of the interface for Python (Windows)

This library has been interfaced to [Python 3](https://www.python.org/) using the software [SWIG](http://www.swig.org/) (version 4.0.1). In order to compile the interface, users need to install the `SWIG` tool in their system. As for the rest, one only needs the `make` tool.

## Installing Python 3

Go to Python's [webpage](https://www.python.org/) and download one of the `3.x` builds for Windows. We have tested the interface with Python 3.6, but any `3.x` should work. Python builds for Windows can be found [here](https://www.python.org/downloads/windows/). When installing Python, make sure to check the box that says `Add Python 3.x to PATH`. If you did not check it, you can add them after the installation manually. Add to the `PATH` environment varialbe the paths

	(...)/Python3x/Scripts/
	(...)/Python3x/

where `x` is the minor version of Python and (...) represents the path to the installation directory of Python. We recommend using

	C:/programming/

## Installing SWIG

Download and install SWIG from its webpage. Authors of SWIG already provide downloadable files with prebuilt executables for windows. Download these and install them in your system. It is key to include the path to the installation folder in the `PATH` environment variable. For example, one can install SWIG in

	C:/programming/swig

and then add this path to the `PATH` environment variable.

## Configuring the _Makefile_

In order to compile the interface, the user has to configure one of the build files. For this, it is required that you know the minor version of Python installed in their system. As stated above, the major version is required to be `3`. The minor version the interface has been tested on is `6` (i.e., we have been using Python 3.6), but should work on any version `3.x`. Moreover, one has to know where LAL has been installed in the system. With this information, the user has to modify the variables in the file [Makefile](https://github.com/lluisalemanypuig/linear-arrangement-library/blob/master/python-interface/Makefile) accordingly.

First of all, modify the variables `LAL_INC_DIR` and `LAL_LIB_DIR`. For example, if you installed LAL from sources and followed the instructions in [this](https://github.com/lluisalemanypuig/linear-arrangement-library/blob/master/instructions/compilation-library-windows.md) file, you will need

	# where are LAL's include files
	LAL_INC_DIR = C:/programming/c++/include
	# where are LAL's library files
	LAL_LIB_DIR = C:/programming/c++/bin

Secondly, specify the directory where Python's header files are located at, and where to find the binaries. To do this, modify the variable `PYTHON_INC_DIR`. For example, 

	PYTHON_INC_DIR = C:/programming/Python36/include

Thirdly, the user also can choose the destination directory of LAL's python interface. Modify the variable `LAL_PY_DEST`. This could be

	LAL_PY_INSTALL = C:/programming/python_lib

Fourth, one has to specify Python's minor version in another variable so that the linkage can be done properly. For this modify the variable `PYTHON_LIBRARY` so that the contents of

	# Python3 linkage
	PYTHON_LIBRARY = -lpython3x -lpython3

match your minor version (replace `x` for the minor version of your choice).

Last but not least the user must specify the compiler to be used. One requirement is that the compiler used has to support for `C++17`'s standard. In particular, the compiler must support the flags

	FLAGS = -std=c++17 -fPIC -fopenmp

and also,

	-O3 -DNDEBUG                        # for release builds
	-g -O0 -DDEBUG -D_GLIBCXX_DEBUG     # for debug builds

In order to specify compiler, modify variable `CXX`. By default, we use GNU's `g++` (version 9.2.0)

	# Compiler to be used
	CXX = g++

## Compiling and installing the interface

We offer two different builds for the python interface `debug` and `release`. Each build is linked against the corresponding compilation of the library.

### RELEASE compilation and installation

	$ cd python-interface
	$ make BUILD=release ENVIR=dist -j4
	$ make BUILD=release ENVIR=dist install

### DEBUG compilation and installation

	$ cd python-interface
	$ make BUILD=debug ENVIR=dist -j4
	$ make BUILD=debug ENVIR=dist install

## Using the Python interface

It only remains one final step. Navigate to the directory that the user specified in the variable `LAL_LIB_DIR`. Now copy the files

	liblal.dll liblaldebug.dll

into the directory that the user specified in the variable `LAL_PY_INSTALL` so that python can load the DLLs correctly.
