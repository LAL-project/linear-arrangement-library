# Installation of the interface for Python (Linux)

This library has been interfaced to [Python 3](https://www.python.org/) using the software [SWIG](http://www.swig.org/) (version 4.0.1). In order to compile the interface, users need to install the `SWIG` tool in their system. As for the rest, one only needs the `make` tool.

## Configuring the _Makefile_

In order to compile the interface, the user has to configure one of the build files. For this, it is required that you know the minor version of Python installed in their system. As stated above, the major version is required to be `3`. The minor version the interface has been tested on is `6` (i.e., we have been using Python 3.6), but should work on any version `3.x`. Moreover, one has to know where LAL has been installed in the system. With this information, the user has to modify the variables in the file [Makefile](https://github.com/lluisalemanypuig/linear-arrangement-library/blob/master/python-interface/Makefile) accordingly.

First of all, modify the variables `LAL_INC_DIR` and `LAL_LIB_DIR`. For example,

	# where are LAL's include files
	LAL_INC_DIR = /usr/local/include
	# where are LAL's library files
	LAL_LIB_DIR = /usr/local/lib

Secondly, specify the director where Python's header files are located at, and where to find the binaries. To do this, modify the variable `PYTHON_INC_DIR`. For example, 

	PYTHON_INC_DIR = /usr/include/python3.6

Thirdly, the user also can choose the destination directory of LAL's python interface. Modify the variable `LAL_PY_DEST`. This could be

	LAL_PY_INSTALL = /usr/lib/python3.6

The directories above are used by default. Last but not least the user must specify the compiler to be used. One requirement is that the compiler used must have support for `C++17`'s standard. In particular, the compiler must support the flags

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
