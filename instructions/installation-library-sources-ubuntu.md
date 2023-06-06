# Installation of LAL and its dependencies (Ubuntu)

In this file is detailed step by step the compilation and installation process of this library and all its dependencies from sources. In order to compile the library, you need to have `g++` and `cmake` tools installed in your system, and the `GMP` library.

## Installing the dependencies

### Compiler

You will need a compiler that supports the `C++17` standard. We use GNU's `g++` (we have tested with several versions). There are several guides on the Internet explaining how to install said compiler. For this you should install the latest version of `g++` available. For example, you can run:

	$ sudo apt install build-essential

In order to check that the compiler has been installed, issue the following command:

	$ g++ --version

You should see something like this:

	g++-11 (Ubuntu 11.1.0-1ubuntu1~20.04) 11.1.0
	Copyright (C) 2021 Free Software Foundation, Inc.
	This is free software; see the source for copying conditions.  There is NO
	warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

### CMake

Install CMake by issuing the command

	$ sudo apt install cmake

A version newer than the minimum required to compile LAL (`3.10.2`) should have been installed. To check this, run

	$ cmake --version

### GMP

The [GMP](https://gmplib.org/) library can be installed via the package manager:

	$ sudo apt install libgmp10 libgmp10-dev

At the moment of writing this, the command above installs GMP version `6.2.0`.

Alternatively, it can be installed from sources. First, download the source files, uncompress the file and issue the following commands in the new directory

	$ ./configure --enable-cxx
	$ make -j4
	$ make check -j4   # GMP developers strongly recommend to run these commands, and so do we.
	$ make install

### [git](https://git-scm.com/)

	$ sudo apt install git

Alternatively, you can download the sources through the web browser.

## Compiling and installing LAL

The following commands provide an easy and quick way of downloading, compiling and installing the library. First, download LAL's sources from its [github page](https://github.com/LAL-project/linear-arrangement-library.git)

	$ git clone https://github.com/LAL-project/linear-arrangement-library.git

The following commands are used to compile the library in both `debug` and `release` builds. The `cmake` command is configured by default to install the compiled files in the `/usr` directory. In order to change this, simply specify the desired installation directory by adding this

	-DCMAKE_INSTALL_PREFIX=directory

to the `cmake` command. See below for examples.

### Release build

First, you should make the build directory. This can be done using three commands. First, navigate to the library's directory.

	$ cd linear-arrangement-library

Make the directory, and navigate into it.

	$ mkdir lal-release
	$ cd lal-release

#### Prepare the build

You can prepare the build with the predefined settings like this:

	$ cmake ../lal

You can also set your own settings specifying the appropriate flags to `cmake`. For example, you can change the installation directory like this:

	$ cmake -DCMAKE_INSTALL_PREFIX=~/Desktop ../lal

Run only one of the two commands above (*one* of the two calls to `cmake`). The output of the `cmake` program may be important for us in case errors arise during compilation. Please, save the output to a text file in case errors arise.

#### Compile and install

	$ make -j4
	$ make install

### Debug build

The steps to make the debug build are very similar to the ones to make a release build. 

#### Make the build directory

	$ cd linear-arrangement-library
	$ mkdir lal-debug
	$ cd lal-debug

#### Prepare the build

You can prepare the build with the predefined settings like this:

	$ cmake -DCMAKE_BUILD_TYPE=Debug ../lal

You can also set your own settings specifying the appropriate flags to `cmake`. For example, you can change the installation directory like this:

	$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=~/Desktop ../lal

Run only one of the two commands above (*one* of the two calls to `cmake`). The output of the `cmake` program may be important for us in case errors arise during compilation. Please, save the output to a text file in case errors arise.

#### Compile and install

	$ make -j4
	$ sudo make install

## Generating LAL's `html` documentation

### Dependencies

In order to generate the `html` documentation of LAL (with [`doxygen`](https://www.doxygen.nl/index.html)) you need to install the following software.

#### epstopdf

	$ sudo apt install texlive-font-utils

#### ghostscript

	$ sudo apt install ghostscript

#### Bibtex

	$ sudo apt install texlive-bibtex-extra

#### [`doxygen`](https://www.doxygen.nl/index.html)

	$ sudo apt install doxygen

### Making the documentation

Inside the directory `linear-arrangement-library`, run the following command.

	$ ./make_docs.sh

This will generate a directory called `docshtml`. In order to visualize it, choose a web browser and open the `index.html` file (`Ctrl+o` or `File > Open File...`) that you will find in

	linear-arrangement-library/docshtml/html/index.html
