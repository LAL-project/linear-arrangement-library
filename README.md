# Linear Arrangement Library

This library implements several algorithms to solve problems related to linear arrangements. For example, it implements algorithms that

- solve the Minimum Linear Arrangement Problem on trees under different constraints of projectivity,
- solve different variants of the Maximum Linear Arrangement Problem on bipartite graphs and trees,
- calculate the expectation and variance of the number of edge crossings and of the sum of the length of the edges under different constraints (such as projectivity and planarity),
- compute efficiently the number of edge crossings given a linear arrangement of a graph,
- and solve many other problems.

Some of the algorithms listed above are implemented for both general graphs and trees (the latter being more efficient). The library also provides functions for the generation of random/exhasutive labelled/unlabelled free/rooted trees. Besides, there's algorithms for the generation of arrangements under different constraints (such as projectivity and planarity). Users will also find utilities to process treebanks, and collection of treebanks.

For more information, visit [LAL's official webpage](https://cqllab.upc.edu/lal/). Do not forget to [take a look at the guides!](https://cqllab.upc.edu/lal/guides/).

**A word of caution** The master branch of this repository may contain unstable code.

## Installation of the library from binaries

We wrote different sets of instructions for installing LAL's compiled distribution files for

- [Ubuntu](https://github.com/LAL-project/linear-arrangement-library/blob/master/instructions/installation-library-binaries-linux.md). We do not currently maintain any official repository for any of the Linux distributions' package manager.
- [Windows](https://github.com/LAL-project/linear-arrangement-library/blob/master/instructions/installation-library-binaries-windows.md).

## Installation of the library from sources

We wrote different sets of instructions for compiling and installing LAL from sources for

- [Ubuntu](https://github.com/LAL-project/linear-arrangement-library/blob/master/instructions/installation-library-sources-ubuntu.md).
- [Windows](https://github.com/LAL-project/linear-arrangement-library/blob/master/instructions/installation-library-sources-windows.md).
- [Mac OS](https://github.com/LAL-project/linear-arrangement-library/blob/master/instructions/installation-library-sources-macos.md).

## Compile a program that uses LAL

See [this](https://github.com/LAL-project/linear-arrangement-library/blob/master/instructions/compiling-against-LAL.md) file for instructions on how to compile and link a C++ program that uses LAL.

## Author information

This library is the result of the joint effort of several people (in alphabetical order):

- Alemany-Puig, Lluís (see [his webpage](https://cqllab.upc.edu/people/lalemany/) and [his Research Gate profile](https://www.researchgate.net/profile/Lluis_Alemany-Puig)).
- Esteban, Juan Luis (see [his webpage](https://www.cs.upc.edu/~esteban/) and [his Research Gate profile](https://www.researchgate.net/profile/Juan_Esteban13))
- Ferrer-i-Cancho, Ramon (see [his webpage](https://cqllab.upc.edu/people/rferrericancho/) and [his Research Gate profile](https://www.cs.upc.edu/~rferrericancho/)).

## Citing the library

In this library users will find implementations of papers written by the developers of LAL themselves, and implementations of algorithms devised by other researchers. This library is the result of great efforts put directly by the developers, and indirectly by other researchers. In an academic paper, please,

- cite the articles presenting LAL and the Zenodo repository that corresponds to the exact version of LAL that is used,
- acknowledge the effort made by other researchers by citing their papers in which they describe the algorithms implemented in this library. Such papers are cited in the C++ documentation and can be found easily in the doxygen documentation. In case a citation is lacking, or is just incorrect, do not hesitate to contact us to inform us about this issue. All the citations can be found in doxygen format [here](https://github.com/LAL-project/linear-arrangement-library/blob/master/doxyconfig/bibliography.bib).

In order to cite the Linear Arrangement Library, please cite all papers presenting LAL (including, but not limited to the following list of papers):

- Lluı́s Alemany-Puig, Juan Luis Esteban, and Ramon Ferrer-i-Cancho. "The Linear Arrangement Library. A new tool for research on syntactic dependency structures". In: Proceedings of the Second Workshop on Quantitative Syntax (Quasy, SyntaxFest 2021). Sofia, Bulgaria: Association for Computational Linguistics, Dec. 2021, pp. 1–16. url: https://aclanthology.org/2021.quasy-1.1.

		@inproceedings{Alemany2021b,
			title = {{The Linear Arrangement Library. A new tool for research on syntactic dependency structures}},
			author = {{Alemany-Puig}, Llu\'{i}s and Esteban, {Juan Luis} and {Ferrer-i-Cancho}, Ramon},
			arxivId = {2112.02512},
			booktitle = {Proceedings of the Second Workshop on Quantitative Syntax (Quasy, SyntaxFest 2021)},
			url = {https://aclanthology.org/2021.quasy-1.1},
			month = {dec},
			year = {2021},
			address = {Sofia, Bulgaria},
			publisher = {Association for Computational Linguistics},
			pages = {1--16}
		}

In order to cite the corresponding Zenodo repository, please, refrain from using a non-stable release of LAL (`main` branch) for your research, and use a stable release instead. Contact us or visit the official webpage for help on how to proceed.

## Documentation

The C++ documentation can be found online in the [library's webpage](https://cqllab.upc.edu/lal/). Alternatively, it can be generated in _html_ format with [doxygen](http://doxygen.nl/). Simply issue the following command from the directory's root directory

	$ cd /path/to/the/linear-arrangement-library
	$ ./make_docs.sh

