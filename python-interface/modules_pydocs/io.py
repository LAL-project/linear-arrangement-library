r"""

Input/Output functions and processing of treebanks.
-----------------------

This module contains the functions for input/output operations.

The most basic operations read a graph from a file. The formats supported for reading are:

- Edge list. See ``read_edge_list`` functions.
- Head vector (see ``lal.head_vector`` for details). See ``read_head_vector`` functions.

Users can also process collection of trees (called treebank files), and collections of treebank files (obviously, a treebank collection). One can process automatically a treebank file (see ``lal.io.treebank_processor``) or a treebank collection (see ``lal.io.treebank_collection_processor``), and iterate through the trees of a treebank file (see ``lal.io.treebank_reader``) and through the treebank files within a treebank collection (see ``lal.io.treebank_collection_reader``).

A treebank file is a file containing only head vectors of trees. Such head vectors may not be formatted correctly (from the standpoint of this library), and so users of the library are also provided with function to check the correctess of such files (see ``lal.io.check_correctness_treebank`` and ``lal.io.check_correctness_treebank_collection``). Users are encouraged to read the documentation of these classes to know more about treebank files and collection of treebanks and how to process them with the library.

A head vector is a representation of (usually) a rooted tree. A head vector of an `n`-vertex tree is a list of non-negative integer numbers. The number at position `i` denotes the parent node of the vertex at said position. Value '0' denotes the root. In this case, the vertex corresponding to the value '0' is not labelled as a root. Each tree is formatted as a list of whole, positive numbers (including zero), each representing a node of the tree. The number 0 denotes the root of the tree, and a number at a certain position indicates its parent node. For example, when number 4 is at position 9 it means that node 9 has parent node 4. Therefore, if number 0 is at position 1 it means that node 1 is the root of the tree. A complete example of such a tree's representation is the following

	0 3 4 1 6 3
	

which should be interpreted as

	(a) predecessor:       0 3 4 1 6 3
	(b) node of the tree:  1 2 3 4 5 6

Note that lines like these are not valid:

	(1) 0 2 2 2 2 2
	(2) 2 0 0

Line (1) is not valid due to a self-reference in the second position, and (2) is not valid since it contains two '0' (i.e., two roots).
"""
