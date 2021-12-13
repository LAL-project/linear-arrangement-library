# Installation of LAL from binaries (Linux)

**NOTE** This has *only* been tested on Ubuntu 20.04.

LAL package for Linux are distributed in four folders

- `docs`: contains the Doxygen documentation of the C++ code.
- `include`: contains the header files `.hpp` of the C++ code.
- `lib`: contains LAL's `.so` files (the compiled C++ source files necessary for the Python interface).
- `python3.x`: contains the `.py` and `.so` files for Python version `3.x`.

The easiest procedure to installing the library may be to move the `include` and `lib` folders into the `/usr` directory. For this, execute the commands

	$ sudo cp -r include/lal/ /usr/include/
	$ sudo cp lib/* /usr/lib

Folder `python3.x`  be copied into `/usr/lib`.

If you do not have `sudo` permissions, an alternative way of installing LAL is to place the `include`, `lib` and `python3.x` folders in a directory of your choice, say, `/path/to/LAL` and then updating the necessary environment variables. To do so, get the full path of these directories (since it is needed). The path can be easily found by opening a terminal in said directories and running

	$ echo $PWD

Finally, update the environment variables by running the following commands:

	$ export PATH=/path/to/LAL/include:$PATH
	$ export LD_LIBRARY_PATH=/path/to/LAL/lib:$LD_LIBRARY_PATH
	$ export PYTHONPATH=/path/to/LAL/python3.x:$PYTHONPATH

Remember to replace the `x` in the path `/path/to/LAL/python3.x` in the last command of the list above with the appropriate minor version of Python. We recommend to add these three commands in your `.bashrc` file, or equivalent, so that they are run every time a new terminal is opened.
