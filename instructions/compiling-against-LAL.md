# Compiling against the library

Programs that use LAL have to be compiled with a compiler that supports `C++17` (e.g., GNU's `g++` 9.2.0).

When compiling a `.cpp` into a `.o` file, you have two options:
- configure your system's environment variables so that the compiler can find the header files,
- or indicate in the compilation command the header directory (with `-I`).

When compiling one or more `.o` into an executable file, again, you have two options:
- configure your system's environment variables so that the compiler can find the binaries (`.so` in Linux and `.dll` in Windows),
- or indicate in the compilation command the directory with the binaries (with `-L`).

There are a few differences between compiling against LAL in Linux and in Windows. The following examples show the minimum requirements for compiling the libraries.

## Compiling in Linux

Here it is assumed that the library GMP is installed in your system.

### Debug builds

In an unconfigured system

	$ g++ -std=c++17 -D_GLIBCXX_DEBUG -c file.cpp -I/path/to/LAL
	$ g++ -o exe file.o -L/path/to/LAL -llaldebug -lgmp -fopenmp

In a configured system (i.e., the libraries can be found in the directories in the corresponding environment variables),

	$ g++ -std=c++17 -D_GLIBCXX_DEBUG -c file.cpp
	$ g++ -o exe file.o -llaldebug -lgmp -fopenmp

Nota benne: in debug compilations, the `.cpp` files have to be compiled with the symbol `_GLIBCXX_DEBUG` defined, hence the `-D_GLIBCXX_DEBUG`.

### Release builds

In an unconfigured system

	$ g++ -std=c++17 -c file.cpp -I/path/to/LAL
	$ g++ -o exe file.o -L/path/to/LAL -llal -lgmp -fopenmp

In a configured system (i.e., the libraries can be found in the directories in the corresponding environment variables),

	$ g++ -std=c++17 -c file.cpp
	$ g++ -o exe file.o -llal -lgmp -fopenmp

## Compiling in Windows

LAL's binary distribution files for Windows OS (`.dll`) that we generate are linked statically against the library GMP, so we can omit the `-lgmp` flag. However, if you installed LAL from sources then the linkage with GMP might be dynamic. In this case, add the `-lgmp` flag after `-llal` or `-llaldebug` accordingly.

In any case, one needs to specify the include directory with GMP's header files. These are distributed with LAL.

### Debug builds

In an unconfigured system

	g++ -std=c++17 -c -D_GLIBCXX_DEBUG file.cpp -I/path/to/LAL -I/path/to/GMP
	g++ -o exe file.o -lpthread -L/path/to/LAL -llaldebug -fopenmp

In a configured system (i.e., the libraries can be found in the directories in the corresponding environment variables),

	g++ -std=c++17 -lpthread -c file.cpp
	g++ -o exe file.o -lpthread -llaldebug -fopenmp

Nota benne: in debug compilations, the `.cpp` files have to be compiled with the symbol `_GLIBCXX_DEBUG` defined, hence the `-D_GLIBCXX_DEBUG`.

### Release builds

In an unconfigured system

	g++ -std=c++17 -c file.cpp -I/path/to/LAL -I/path/to/GMP
	g++ -o exe file.o -L/path/to/LAL -llal -fopenmp

In a configured system (i.e., the libraries can be found in the directories in the corresponding environment variables),

	g++ -std=c++17 -c file.cpp
	g++ -o exe file.o -llal -fopenmp
