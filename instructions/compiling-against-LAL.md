# Compiling against the library

Programs that use LAL have to be compiled with a compiler that supports `C++17` (e.g., GNU's `g++` 9.2.0). Here it is assumed that the library `GMP` is installed in the system and the compile used is GNU's `g++`.

When compiling a `.cpp` into a `.o` file, you have two options:

- Configure your system's environment variables so that the compiler can find the header files. For example, GNU's `g++` uses the environment variable `CPLUS_INCLUDE_PATH`.
- Indicate in the compilation command the header directory (with `-I`).

When compiling one or more `.o` into an executable file, again, you have two options:

- Configure your system's environment variables so that the compiler can find the (dynamic) library; the extensions of these files are `.so` in Linux, `.dll` in Windows, and `.dylib` in macOS. For example, GNU's `g++` uses the environment variable `LIBRARY_PATH`.
- Indicate in the compilation command the directory with the library (with `-L`).

## Debug builds

If LAL is not installed (or environment variables have not been set):

	$ g++ -std=c++17 -D_GLIBCXX_DEBUG -c file.cpp -I/path/to/LAL
	$ g++ -o exe file.o -L/path/to/LAL -llaldebug -lgmp -fopenmp

In a configured system (i.e., the libraries can be found in the directories in the corresponding environment variables),

	$ g++ -std=c++17 -D_GLIBCXX_DEBUG -c file.cpp
	$ g++ -o exe file.o -llaldebug -lgmp -fopenmp

**Note** in debug builds, the `.cpp` files have to be compiled with the symbol `_GLIBCXX_DEBUG` defined, hence the `-D_GLIBCXX_DEBUG`.

## Release builds

If LAL is not installed (or environment variables have not been set):

	$ g++ -std=c++17 -c file.cpp -I/path/to/LAL
	$ g++ -o exe file.o -L/path/to/LAL -llal -lgmp -fopenmp

In a configured system (i.e., the libraries can be found in the directories in the corresponding environment variables),

	$ g++ -std=c++17 -c file.cpp
	$ g++ -o exe file.o -llal -lgmp -fopenmp
