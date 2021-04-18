r"""

Input/Output operations
-----------------------

This module contains the functions for input/output operations.

This includes reading a graph (or collection of graphs) from a file. The formats supported for reading are:

- Edge list. See ``read_edge_list`` function.
- Head vector. See ``read_head_vector`` function.

Users can also process collection of trees (called treebank files), and collections of treebank files (obviously, a treebank collection). One can process automatically a treebank file (see ``lal.io.treebank_processor``) or a treebank collection (see ``lal.io.treebank_collection_processor``), and iterate through the trees of a treebank file (see ``lal.io.treebank_reader``) and through the treebank files within a treebank collection (see ``lal.io.treebank_collection_reader``).

Furthermore, since treebanks may contain errors (from the standpoint of this library), users are also provided with function to check the correctess of such files (see ``lal.io.check_correctness_treebank`` and ``lal.io.check_correctness_treebank_collection``).
"""
