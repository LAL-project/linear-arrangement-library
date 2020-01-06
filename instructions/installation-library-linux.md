# Installation of the binaries (Linux)

You should have been provided with three folders
- `docs`: this contains the Doxygen documentation for the C++ code.
- `include`: this contains the header files `.hpp` of the C++ code.
- `lib`: this includes a `python3.6` directory which contains several `.so` and `.py` files.

The easiest solution for installing the library is to move the `include` and `lib` folders into the `/usr` directory.

Alternatively, place the `include` and `lib` folders in a directory of your choice, say, `/L/A/L/` and run the following commands:

	$ export PYTHONPATH=/L/A/L/lib/python3.6:$PYTHONPATH
	$ export LD_LIBRARY_PATH=/L/A/L/lib/:$LD_LIBRARY_PATH
	$ sudo ldconfig

You are recommended to add the first two in their `.bashrc` file, or equivalent.
