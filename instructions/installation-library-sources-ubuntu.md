# Installation of LAL and its dependencies (Ubuntu)

In this file is detailed step by step the compilation and installation process of this library and all its dependencies from sources. In order to compile the library, you need to have `g++` and `cmake` tools installed in your system, and the `GMP` library.

## Installing the dependencies

### Compiler

You will need a compiler that supports the `C++17` standard. We use GNU's `g++` (version `11.1.0`). There are several guides on the Internet explaining how to install said compiler. Here we summarize [this guide](https://lindevs.com/install-g-on-ubuntu/):

	$ sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
	$ sudo apt install g++-11

In order to check that the compiler has been installed, issue the following command:

	$ g++-11 --version

You should see something like this:

	g++-11 (Ubuntu 11.1.0-1ubuntu1~20.04) 11.1.0
	Copyright (C) 2021 Free Software Foundation, Inc.
	This is free software; see the source for copying conditions.  There is NO
	warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

In order to run `g++-11` using the shorter `g++` command, you have to configure the 'alternatives' in Ubuntu.

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

### Git

You can install [git](https://git-scm.com/) in Ubuntu issuing the following command

	$ sudo apt install git

Alternatively, you can download the sources through the web browser.

## Compiling and installing LAL

The following commands provide an easy and quick way of downloading, compiling and installing the library. First, download LAL's sources from its [github page](https://github.com/LAL-project/linear-arrangement-library.git)

	$ git clone https://github.com/LAL-project/linear-arrangement-library.git

The following commands are used to compile the library in both `debug` and `release` builds. The `cmake` command is configured by default to install the compiled files in the `/usr` directory. In order to change this, simply specify the desired installation directory by adding this

	-DCMAKE_INSTALL_PREFIX=directory

to the `cmake` command. See below for examples.

### Release build

#### Make the build directory

	$ cd linear-arrangement-library
	$ mkdir lal-release
	$ cd lal-release

#### CMake with the predefined settings

	$ cmake ../lal

#### CMake with other settings

	$ cmake -DCMAKE_INSTALL_PREFIX=~/Desktop ../lal

#### Compile and install

	$ make -j4
	$ sudo make install

### Debug build

#### Make the build directory

	$ cd linear-arrangement-library
	$ mkdir lal-debug
	$ cd lal-debug

#### CMake with the predefined settings

	$ cmake -DCMAKE_BUILD_TYPE=Debug ../lal

#### CMake with other settings

	$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=~/Desktop ../lal

#### Compile and install

	$ make -j4
	$ sudo make install