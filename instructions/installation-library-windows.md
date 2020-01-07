# Installation of the binaries (Windows)

You should have been provided with three folders
- `docs`: contains the Doxygen documentation for the C++ code.
- `include`: contains the header files `.hpp` of the C++ code.
- `lib`: contains LAL's `.dll` files.
- `python3.x`: contains the `.py` and `.pyd` files for Python version `3.x`.

Alternatively, place the `include` and `bin` folders in a directory of your choice, say `C:/path/to/LAL`, and add to the environment variables to the left the paths to their right

	CPLUS_INCLUDE_PATH     C:/path/to/LAL/include
	LIBRARY_PATH           C:/path/to/LAL/bin
	PYTHON_PATH            C:/path/to/LAL/python3.x

LAL has been statically compiled against GMP, so as to alleviate the effort in the user's side when using the library, so we do not provide binaries of GMP. However, we provide its header files (`.hpp`).
