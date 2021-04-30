# Linear Arrangement Library

This library implements several algorithms to solve problems related to linear arrangements. For example, it implements algorithms for

- calculation of the expectation and variance of the number of crossings,
- calculation of the expectation and variance of the sum of the length of the edges,
- efficient algorithms to compute the number of crossings,
- calculation of optimal arrangements with respect to the sum of the length of the edges,
- and many others.

Some of the algorithms listed above are optimised for trees. The library also provides functions for generation of

- random generation of labelled and unlabelled free trees,
- exhaustive generation of labelled and unlabelled free trees,
- random generation of labelled rooted trees.

## Installation of the library

We wrote different sets of instructions for installing LAL's compiled distribution files for

- Windows in [this](https://github.com/lluisalemanypuig/linear-arrangement-library/blob/master/instructions/installation-library-windows.md) file.
- Linux in [this](https://github.com/lluisalemanypuig/linear-arrangement-library/blob/master/instructions/installation-library-linux.md) file.

## Compilation and installation of the library from sources

We wrote different sets of instructions for compiling and installing LAL from sources for

- Windows in [this](https://github.com/lluisalemanypuig/linear-arrangement-library/blob/master/instructions/compilation-library-windows.md) file.
- Linux in [this](https://github.com/lluisalemanypuig/linear-arrangement-library/blob/master/instructions/compilation-library-linux.md) file.

## Compilation and installation of the Python interface

We wrote different sets of instructions for compiling and installing LAL's Python interface from sources for

- Windows in [this](https://github.com/lluisalemanypuig/linear-arrangement-library/blob/master/instructions/compilation-pyhton-interface-windows.md) file.
- Linux in [this](https://github.com/lluisalemanypuig/linear-arrangement-library/blob/master/instructions/compilation-pyhton-interface-linux.md) file.

## Compile a program that uses LAL

See [this](https://github.com/lluisalemanypuig/linear-arrangement-library/blob/master/instructions/compiling-against-LAL.md) file for instructions on how to compile and link a C++ program that uses LAL.

## Author information

This library is the result of the joint effort of several people (in alphabetical order):

- Alemany-Puig, Lluís (see [his webpage](https://cqllab.upc.edu/people/lalemany/) and [his Research Gate profile](https://www.researchgate.net/profile/Lluis_Alemany-Puig)).
- Esteban, Juan Luis (see [his webpage](https://www.cs.upc.edu/~esteban/) and [his Research Gate profile](https://www.researchgate.net/profile/Juan_Esteban13))
- Ferrer-i-Cancho, Ramon (see [his webpage](https://cqllab.upc.edu/people/rferrericancho/) and [his Research Gate profile](https://www.cs.upc.edu/~rferrericancho/)).

## Citing the library

For now we do not have any formal publication of this library, so we do not have a preferred way of citing it. Please, contact any of the authors for further information about this matter.

## Documentation

The C++ documentation can be generated in _html_ format with [doxygen](http://doxygen.nl/). Simply issue the following command from the directory's root directory

	$ ./make_docs.sh c++
