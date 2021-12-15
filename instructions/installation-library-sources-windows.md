# Installation of LAL and its dependencies (Windows)

**NOTE** *We provide binaries for Windows that can be installed with an installer (those *Next*, *Next* clicky things). We recommend you install LAL using the installer, if possible.*

Here we first detail step by step the configuration of the Windows system in order to compile and install the library from sources. That is, we first give instructions on how to install the necessary tools and where to find them. After completing this first step, almost all the dependencies and software necessary to compile the library will be installed. We finish by explaining how to compile and install this library from sources.

We encourage you to read through all steps before actually performing any of them.

## Necessary tools and basic setup of the system

We recommend installing the [`MSYS2`](https://www.msys2.org/) subsystem for simplicity. This can be done following the 7 steps in their webpage. These consist of downloading and executing the installer and executing some very well-explained commands. With a fast internet connection it should take no more than 15 minutes.

## Installing the dependencies

The dependencies should be installed using the `MSYS2 MSYS` program. In Windows' Start menu, launch `MSYS2 MSYS`, and type the following commands.

### cmake

	$ pacman -S mingw-w64-x86_64-cmake

### git

	$ pacman -S git

## Compiling and installing LAL

The following commands provide an easy and quick way of downloading, compiling and installing the library. These commands have to be run using the `MSYS2 MinGW` terminal; launch it from Windows' start menu typing `MSYS2 MingGW` (at the time of writing, the program has a blue icon which can be seen in the start menu).

First, download LAL's sources from its [github page](https://github.com/LAL-project/linear-arrangement-library.git).

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

	$ cmake -G "MSYS Makefiles" ../lal

You can also set your own settings specifying the appropriate flags to `cmake`. For example, you can change the installation directory like this:

	$ cmake -G "MSYS Makefiles" -DCMAKE_INSTALL_PREFIX=~/Desktop ../lal

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

	$ cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Debug ../lal

You can also set your own settings specifying the appropriate flags to `cmake`. For example, you can change the installation directory like this:

	$ cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=~/Desktop ../lal

Run only one of the two commands above (*one* of the two calls to `cmake`). The output of the `cmake` program may be important for us in case errors arise during compilation. Please, save the output to a text file in case errors arise.

#### Compile and install

	$ make -j4
	$ sudo make install

## Generating LAL's `html` documentation

### Dependencies

The dependencies have to be installed using `MSYS2 MSYS`. To launch it, type `MSYS2 MSYS` in Windows' Start menu.

#### epstopdf, ghostscript

The following command will install `epstopdf` and `ghostscript` and other software on which these two depend.

	$ pacman -S mingw-w64-x86_64-texlive-font-utils

#### Bibtex

	$ pacman -S mingw-w64-x86_64-texlive-bibtex-extra

#### [`doxygen`](https://www.doxygen.nl/index.html)

	$ pacman -S mingw-w64-x86_64-doxygen

### Making the documentation

Close `MSYS2 MSYS` used in the steps above and launch `MSYS2 MinGW` from Windows' Start menu.

Inside the directory `linear-arrangement-library`, run the following command.

	$ ./make_docs.sh

This will generate a directory called `docshtml`. In order to visualize it, choose a web browser and open the `index.html` file (`Ctrl+o` or `File`>`Open File...`) that you will find in

	linear-arrangement-library/docshtml/html/index.html
