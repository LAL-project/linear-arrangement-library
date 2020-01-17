# Installation of LAL and its dependencies (Windows)

Here we first detail step by step the configuration of the Windows system in order to compile and install the library from sources. That is, we first give instructions on how to install the necessary tools and where to find them (step 0). Then, we list the library's dependencies and explain how to install them, from sources (step 1). We finish by explaining how to compile and install this library from sources (step 2). Instructions given in step 1 and step 2, assume that the you have done the basic setup explained in step 0. Other steps can be followed, but we do not provide them. You are encouraged to read through all steps before actually performing any of them.

## 0. Downloading the necessary tools and doing a basic setup of the system

- 0.1. Download [MinGW](https://nuwen.net/mingw.html). Download the most recent file. Version 17.1 (last visited 5th January 2020) provides all the tools you need (and some more). Use at your own risk.

	* 0.1.1. Extract the files in the following directory

			C:/programming

	* 0.1.2. Add the following paths to the `PATH` environment variable

			C:/programming/MinGW/bin
			C:/programming/MinGW/git/bin

- 0.2. Download part of the GnuWin32 toolkit. We need the tool [m4](https://www.gnu.org/software/m4/). Download both [binaries](https://sourceforge.net/projects/gnuwin32/files/m4/1.4.14-1/m4-1.4.14-1-bin.zip/download) and [dependencies](https://sourceforge.net/projects/gnuwin32/files/m4/1.4.14-1/m4-1.4.14-1-dep.zip/download) (last visited 5th January 2020).

	* 0.2.1. Unpack the directories in the compressed files and move them to

			C:/programming/GnuWin32

	* 0.2.2. Add the following paths to the `PATH` environment variable
	
			C:/programming/GnuWin32/bin

- 0.3. Download [CMake](https://cmake.org/) for Windows. Version 3.16.2 can be downloaded [here](https://github.com/Kitware/CMake/releases/download/v3.16.2/cmake-3.16.2-win64-x64.zip) (last visited 5th January 2020)

	* 0.3.1. Extract the files and move them to

			C:/programming/cmake/

	* 0.3.2. Add to the PATH environment variable the path

			C:/programming/cmake/bin

- 0.4. Since we are going to be installing several libraries, create the following directories

		C:/programming/c++/bin
		C:/programming/c++/include
		C:/programming/c++/lib
		C:/programming/c++/share
	
	* 0.4.1. Add the following directories to the `PATH` environment variable
	
			C:/programming/c++/bin
			C:/programming/c++/lib

- 0.5. Moreover, add the following environment variables, and set their contents to directory to their right

		CPLUS_INCLUDE_PATH      C:/programming/c++/include
		LIBRARY_PATH            C:/programming/c++/lib

The next steps require using the Git-Bash command-line interface. In order to launch it, double click on the file

	C:/programming/MinGW/git/git-bash

## 1. Installing the dependencies

### Installation of GMP

Instead of installing the [GMP](https://gmplib.org/) library by compiling its sources, one can download the compiled library from a trustworthy source. The downloaded files should provide three folders
	
	include/
	lib/
	share/

which should be moved into the recently created directory

	C:/programming/c++/

Alternatively, the sources can be compiled and installed following these steps:

- 1.1. Download the source files. Windows users might want to download
[this](https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2) file (last visited 5th January 2020).
- 1.2. Extract the files into

		C:/programming/sources

- 1.3. In the command line interface launched in step 0.5 navigate to the directory with GMP's source files.

		$ cd C:/programming/sources/gmp-6.1.2

- 1.4. Compile and install the library.

		$ ./configure --prefix=C:/programming/c++ --enable-cxx
		$ make -j4
		$ make check -j4
		$ make install

### Compiler

You will need a compiler that supports the `C++17` standard. We use GNU's `g++` (version 9.2.0). The downloaded MinGW files (step 0.1) should provide such a compiler.

## 2. Compiling and installing LAL

The library can be installed in both `Release` and `Debug` mode. First of all

- Download LAL's source files from its [github page](https://github.com/lluisalemanypuig/linear-arrangement-library.git) either using a web browser or the command-line terminal

		$ cd C:/programming/sources
		$ git clone https://github.com/lluisalemanypuig/linear-arrangement-library.git

- In the command-line interface launched in step 0.5. navigate to the directory with LAL's source files.

		$ cd C:/programming/sources/linear-arrangement-library

Now it only remains to compile and install the library

### Release build

	$ mkdir lal-release ; cd lal-release
	$ cmake -G "MSYS Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_INSTALL_PREFIX:PATH=C:/programming/c++ ../lal
	$ make -j4 && make install

### Debug build

	$ mkdir lal-debug ; cd lal-debug
	$ cmake -G "MSYS Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_INSTALL_PREFIX:PATH=C:/programming/c++ -DCMAKE_BUILD_TYPE=Debug ../lal
	$ make -j4 && make install