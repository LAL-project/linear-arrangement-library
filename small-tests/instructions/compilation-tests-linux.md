# Compilation of the tests (Linux)

LAL comes with a set of tests to make sure that there are no bugs in the code and that the library was compiled successfully. To compile the code that will run the tests we use CMake. To compile the tests, however, we need to tell CMake the directory where the branch of LAL we want to test is.

## Cloning LAL

Clone the repository of LAL into a directory. For example, we can run

	$ cd  # go to our home directory
	$ git clone git@github.com:LAL-project/linear-arrangement-library.git
	$ cd linear-arrangement-library

Then, we choose one of the branches we want to test. In this branch (`master`) we should use 

	$ git checkout master

The library has to be compiled into two subdirectories within `linear-arrangement-library`. These two directories have to be called `lal-release` and `lal-debug` and will contain, respectively, the release and debug compilations.

## Compile the tests

### Release

First, we create the appropriate directories that will contain the executable files.

	$ cd linear-arrangement-library
	$ cd small-tests; mkdir build-release; cd build-release
	
Now we invoke CMake. To do so, we need to specify an absolute path to the library. To do this, we have to define the symbol
	
	-DLAL_DEV_DIR=...

And replace `...` with the aforementioned absolute path. For instance, if you cloned the repositories in your home folder, you could run

	$ cmake -DLAL_DEV_DIR=$HOME/linear-arrangement-library ../src
	$ make -j

### Debug

First, we create the appropriate directories that will contain the executable files.

	$ cd linear-arrangement-library
	$ cd small-tests; mkdir build-debug; cd build-debug
	
Now we invoke CMake. To do so, we need to specify an absolute path to the library. To do this, we have to define the symbol
	
	-DLAL_DEV_DIR=...

And replace `...` with the aforementioned absolute path. For instance, if you cloned the repositories in your home folder, you could run

	$ cmake -DLAL_DEV_DIR=$HOME/linear-arrangement-library -DCMAKE_BUILD_TYPE=Debug ../src
	$ make -j
