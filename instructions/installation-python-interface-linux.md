# Installation of the interface for Python (Linux)

This library has been interfaced to [Python 3](https://www.python.org/) using the software [SWIG](http://www.swig.org/) (version 4.0.1).

The interface files are already uploaded to the repository, so users need not install the ```SWIG``` tool in their system. One only needs to compile the files using the ```make``` tool. The installation procedure is the same in both Linux and Windows with slight variations.

## Giving values to the environment variables

- First of all, the user must know the minor version of Python installed in their system. As stated above, the major version is 3. The minor version the interface has been tested on is 6 (i.e., we have been using Python 3.6). Using this information, the user has to modify the variables in the file [Makefile.dist](https://github.com/lluisalemanypuig/linear-arrangement-library/blob/master/python-interface) accordingly. Both Linux and Windows users must specify the directory where the header files for Python interfacing are located at. In Linux this might be:

		PYTHON_INCLUDE	= /usr/include/python3.6

	The user also must know the destination directory for installing the python module generated using _SWIG_. In Linux, this might be:

		LAL_PY_INSTALL	= /usr/lib/python3.6

	The directories above are used by default.

- Last but not least the user must specify the compiler to be used. The compiler used by default is the _g++_ compiler. One requirement is that the compiler used must have support for C++11's standard. In particular, the compiler must support the flags

		COMMON_FLAGS	= -std=c++17 -fPIC -fopenmp

## Compiling and installing the interface

The interface can be compiled in both ```DEBUG``` and ```RELEASE``` modes. Each mode is linked against the corresponding compilation of the library.

### RELEASE compilation and installation

	cd python-interface
	make -f Makefile.dist release
	make -f Makefile.dist installrelease

### DEBUG compilation and installation

	cd python-interface
	make -f Makefile.dist debug
	make -f Makefile.dist installdebug
