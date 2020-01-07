# Installation of the binaries (Linux)

You should have been provided with three folders
- `docs`: contains the Doxygen documentation for the C++ code.
- `include`: contains the header files `.hpp` of the C++ code.
- `lib`: contains LAL's `.so` files.
- `python3.x`: contains the `.py` and `.so` files for Python version `3.x`.

The easiest solution for installing the library is to move the `include` and `lib` folders into the `/usr` directory. Folder `python3.x` should be moved into `/usr/lib`.

Alternatively, place the `include` and `lib` folders in a directory of your choice, say, `/path/to/LAL` and run the following commands:

	$ export PYTHONPATH=/path/to/LAL/python3.x:$PYTHONPATH
	$ export LD_LIBRARY_PATH=/path/to/LAL/lib:$LD_LIBRARY_PATH

Remember to replace the `x` with the appropriate minor version of Python. You are recommended to add them in your `.bashrc` file, or equivalent.
