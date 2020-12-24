# Installation of the interface for Python (Windows)

This library has been interfaced to [Python 3](https://www.python.org/) using the software [SWIG](http://www.swig.org/) (version 4.0.1). In order to compile the interface, you need to have the `SWIG` tool installed in your system. As for the rest, you only need the `make` tool.

## Installing Python 3

Go to Python's [webpage](https://www.python.org/) and download one of the `3.x` builds for Windows. We have tested the interface with Python 3.6, but any `3.x` should work. Python builds for Windows can be found [here](https://www.python.org/downloads/windows/). When installing Python, make sure to check the box that says `Add Python 3.x to PATH`.

## Installing SWIG

Download and install `SWIG` from its webpage. Authors of `SWIG` already provide downloadable files with prebuilt executables for windows. Download these and install them in your system. It is key to include the path to the installation folder in the `PATH` environment variable. For example, you can install SWIG in

	C:/programming/swig

and then add the path you used to the `PATH` environment variable.

## Configuring the _Makefile_

In order to compile the interface, you need to configure one of the build files (change a few variables' contents). For this, it is required that you know the minor version of Python installed in their system. As stated above, the major version is required to be `3`. The minor versions the interface has been tested on are `6`, `7` and `8`, but should work on any version `3.x`. Moreover, one has to know where LAL has been installed in the system. With this information, modify some of the variables in the file
	
	linear-arrangement-library/python-interface/Makefile

First of all, modify the variables `LAL_INC_DIR` and `LAL_LIB_DIR`. For example, if you installed LAL from sources and followed the instructions in [this](https://github.com/lluisalemanypuig/linear-arrangement-library/blob/master/instructions/compilation-library-windows.md) file, you will need

	# where are LAL's include files
	LAL_INC_DIR = C:/programming/c++/include
	# where are LAL's library files
	LAL_LIB_DIR = C:/programming/c++/bin

Secondly, specify the version of Python against which the interface is linked. Indicate where Python's header files are located at, and where to find the binaries. To do this, modify the variable `MINOR_VERSION_PYTHON`. For example, 

	# Python's minor version
	MINOR_VERSION_PYTHON = 8

Replace the `8` above with the minor version of Python installed in your system.

Thirdly, you can also choose the destination directory of LAL's python interface. Modify the variable `LAL_PY_DEST`. This could be

	# Directory where LAL's interface will be installed to
	LAL_PY_DEST = C:/programming/python_lib

Last but not least, you must specify the compiler to be used. One requirement is that the compiler used has to support for `C++17`'s standard. In particular, the compiler must support the flags

	-std=c++17 -fPIC -fopenmp

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

It only remains one final step. This step depends on the minor version of Python you have. You need to help Python find LAL's `.dll` files

	liblal.dll liblaldebug.dll

Locate the destination directory of these files that you chose when installing LAL. Now add to the `PYTHONPATH` environment variable the directory to which the interface has been installed. This is the directory given to the variable `LAL_PY_DEST` in the Makefile (as explained above). This way, the python interpreter can find the `.py` files generated. This is sufficient for minor versions `6` and `7`. For minor version `8` copy the `.dll` files into your Python installation directory. For example,

	C:/Users/User/AppData/Python/Python3.8/

Note: this workaround does not look too good, but importing LAL does not work without this step.
