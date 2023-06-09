# Installation of LAL from binaries (Ubuntu)

**NOTE** *This has only been tested on Ubuntu 20.04 and compiled with `g++-11`.*

Be aware that the steps below do not actually install LAL in your computer; you will be able to use LAL but Windows will not be aware that you "installed" it, that is, you will not be able to remove LAL via the useful

	$ sudo apt remove ...

Head over to [LAL's official webpage](https://cqllab.upc.edu/lal) and download the appropriate C++17 core depending on the release you want to use. The file should be named `ubuntu20-c++-libs.zip`. Uncompress the files in any directory, say the Desktop. Open a command line terminal and navigate to the directory you chose. If you used the Desktop, run the command

	$ cd ~/Desktop

## Install with `sudo` permissions

With `sudo` permissions, the easiest procedure to installing the library may be to move the `include` and `lib` folders into the `/usr` directory. For this, execute the commands

	$ sudo cp -r include/* /usr/include/
	$ sudo cp lib/* /usr/lib

## Install without `sudo` permissions

If you do not have `sudo` permissions, an alternative way of installing LAL is to place the `include` and `lib` folders inside a directory of your choice, say, `/path/to/LAL` and then updating the necessary environment variables. To do so, get the full path of these directories. The path can be easily found by opening a terminal in said directories (navigate into them with a file browser, right-click on them and select `Open Terminal here` or similar) and running

	$ echo $PWD

Finally, update the environment variables by running the following commands:

	$ export CPLUS_INCLUDE_PATH=/path/to/LAL/include:$CPLUS_INCLUDE_PATH
	$ export LD_LIBRARY_PATH=/path/to/LAL/lib:$LD_LIBRARY_PATH

We recommend to add these three commands in your `.bashrc` file, or equivalent, so that they are run automatically every time a new terminal is opened.

## Compile against LAL

### Compile against the debug build

To link an executable file against the debug build of LAL, issue the following command

	$ g++ -D_GLIBCXX_DEBUG -std=c++17 *.o -I /path/to/lal/headers -L /path/to/lal -llal

### Compile against the release build

To link an executable file against the release build of LAL, issue the following command

	$ g++ -std=c++17 *.o -I /path/to/lal/headers -L /path/to/lal -llaloptimized
