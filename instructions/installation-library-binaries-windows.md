# Installation of LAL from binaries (Windows)

**NOTE** We provide LAL in compiled form only for [`MSYS2`](https://www.msys2.org/) and `mingw64`.

**NOTE** These steps enable you to use the C++17 core of the library. However, if you want to use LAL via Python or some other language, head over to the appropriate repository within [this organization](https://github.com/LAL-project).

Head over to [LAL's official webpage](https://cqllab.upc.edu/lal) and download the appropriate C++17 core depending on the release you want to use. The file should be named `windows10-c++-libs-msys64.zip`. In a file browser, navigate to your `msys64` installation directory. It is usually

	C:/msys64
	
Now,

- Uncompress the files inside `bin` in `windows10-c++-libs-msys64.zip` into `mingw64/bin`. You can further uncompress the files inside `lib`, this time into `mingw64/lib`, should you want to use static libraries.
- Uncompress the contents of the folder `include` within `windows10-c++-libs-msys64.zip` into `mingw64/include`. You should have moved the only folder inside named `lal`, not just its contents.

You should be ready to use C++ LAL.

## Compile against LAL

### Compile against the debug build

To link an executable file against the debug build of LAL, issue the following command

	$ g++ -D_GLIBCXX_DEBUG -std=c++17 *.o -I /path/to/lal/headers -L /path/to/lal -llal -lgmp -fopenmp -lpthread

### Compile against the release build

To link an executable file against the release build of LAL, issue the following command

	$ g++ -std=c++17 *.o -I /path/to/lal/headers -L /path/to/lal -llaloptimized -lgmp -fopenmp -lpthread
