# Installation of LAL and its dependencies (Linux)

In this file is detailed step by step the compilation and installation process of this library and all its dependencies from sources.

## Installing the dependencies

### GMP

The GMP library can be installed via the typical package managers, or from sources, following the usual procedure. First, download the source files and issue

	./configure --enable-cxx
	make -j 4
	make check -j 4
	sudo make install

### Compiler

You will need a compiler that supports the ```C++17``` standard.

## Compiling and installing LAL

The following commands provide an easy and quick way of downloading, compiling and installing the library. First, download LAL's sources from its [github page](https://github.com/lluisalemanypuig/linear-arrangement-library.git)

	git clone https://github.com/lluisalemanypuig/linear-arrangement-library.git

### RELEASE mode

	cd linear-arrangement-library ; mkdir lal-release ; cd lal-release
	cmake ../lal
	make -j 4
	sudo make install

### DEBUG mode

	cd linear-arrangement-library ; mkdir lal-debug ; cd lal-debug
	cmake -DCMAKE_BUILD_TYPE=Debug ../lal
	make -j 4
	sudo make install

