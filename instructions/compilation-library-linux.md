# Installation of LAL and its dependencies (Linux)

In this file is detailed step by step the compilation and installation process of this library and all its dependencies from sources.

## Installing the dependencies

### GMP

The GMP library can be installed via the typical package managers, or from sources, following the usual procedure. First, download the source files and issue

	$ ./configure --enable-cxx
	$ make -j4
	$ make check -j4   # optional but recommended
	$ make install

### Compiler

You will need a compiler that supports the `C++17` standard. We use GNU's `g++` (version 9.2.0).

## Compiling and installing LAL

The following commands provide an easy and quick way of downloading, compiling and installing the library. First, download LAL's sources from its [github page](https://github.com/lluisalemanypuig/linear-arrangement-library.git)

	$ git clone https://github.com/lluisalemanypuig/linear-arrangement-library.git

The following commands are used to compile the library in both `debug` and `release` builds. The `cmake` command is configured by default to install the compiled files in the `/usr` directory. In order to change this, simply specify the desired installation directory with the variable

	-DCMAKE_INSTALL_PREFIX=directory

### RELEASE build

	$ cd linear-arrangement-library ; mkdir lal-release ; cd lal-release
	$ cmake ../lal
	$ make -j4
	$ make install

### DEBUG build

	$ cd linear-arrangement-library ; mkdir lal-debug ; cd lal-debug
	$ cmake -DCMAKE_BUILD_TYPE=Debug ../lal
	$ make -j4
	$ make install

