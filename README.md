# Linear Arrangement Library

This library implements several algorithms to solve problems related to linear arrangements. For example, it implements algorithms for

- solving the Minimum Linear Arrangement Problem on trees under different constraints of projectivity,
- the calculation of the expectation and variance of the number of edge crossings and of the sum of the length of the edges,
- computing the number of edge crossings given a linear arrangement of a graph,
- and many others.

Some of the algorithms listed above are implemented for both general graphs and trees (the latter being more efficient). The library also provides functions for the generation of random/exhasutive labelled/unlabelled free/rooted trees. Besides, there's algorithms for the generation of arrangements under different constraints of projectivity. Users will also find utilities to process treebanks, and collection of treebanks.

For more information, visit [LAL's official webpage](https://cqllab.upc.edu/lal/).

## Installation of the library and Python interface from binaries

We wrote different sets of instructions for installing LAL's compiled distribution files for

- Windows in [this](https://github.com/LAL-project/linear-arrangement-library/blob/master/instructions/installation-library-binaries-windows.md) file.
- Linux in [this](https://github.com/LAL-project/linear-arrangement-library/blob/master/instructions/installation-library-binaries-linux.md) file. We do not currently maintain any official repository for any of the Linux distributions' package manager.

## Installation of the library from sources

We wrote different sets of instructions for compiling and installing LAL from sources for

- Windows in [this](https://github.com/LAL-project/linear-arrangement-library/blob/master/instructions/installation-library-sources-windows.md) file.
- Ubuntu in [this](https://github.com/LAL-project/linear-arrangement-library/blob/master/instructions/installation-library-sources-ubuntu.md) file.

## Installation of the Python interface from sources

We wrote different sets of instructions for compiling and installing LAL's Python interface from sources for

- Windows in [this](https://github.com/LAL-project/linear-arrangement-library/blob/master/instructions/installation-python-interface-sources-windows.md) file.
- Ubuntu in [this](https://github.com/LAL-project/linear-arrangement-library/blob/master/instructions/installation-python-interface-sources-ubuntu.md) file.

## Compile a program that uses LAL

See [this](https://github.com/LAL-project/linear-arrangement-library/blob/master/instructions/compiling-against-LAL.md) file for instructions on how to compile and link a C++ program that uses LAL.

## Author information

This library is the result of the joint effort of several people (in alphabetical order):

- Alemany-Puig, Lluís (see [his webpage](https://cqllab.upc.edu/people/lalemany/) and [his Research Gate profile](https://www.researchgate.net/profile/Lluis_Alemany-Puig)).
- Esteban, Juan Luis (see [his webpage](https://www.cs.upc.edu/~esteban/) and [his Research Gate profile](https://www.researchgate.net/profile/Juan_Esteban13))
- Ferrer-i-Cancho, Ramon (see [his webpage](https://cqllab.upc.edu/people/rferrericancho/) and [his Research Gate profile](https://www.cs.upc.edu/~rferrericancho/)).

## Citing the library

In this library users will find implementations of papers written by the developers of LAL themselves, and implementations of algorithms devised by other researchers. This library is the result of a great effort put directly by the developers, and indirectly by other researchers. Besides citing the library in an academic paper, users *must* also acknowledge the effort made by other researchers by citing their papers in which they present the algorithms implemented in this library. Such papers are cited in the C++ documentation and can be found easily in the doxygen documentation. In case a citation is lacking, or is just incorrect, do not hesitate to contact us to inform us about this issue.

For now we do not have any formal publication of this library, so we do not have a preferred way of citing it. Please, contact any of the authors for further information about this matter.

## Documentation

The C++ documentation can be found online in the [library's webpage](https://cqllab.upc.edu/lal/). Alternatively, it can be generated in _html_ format with [doxygen](http://doxygen.nl/). Simply issue the following command from the directory's root directory

	$ ./make_docs.sh c++
