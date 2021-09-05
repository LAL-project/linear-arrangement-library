r"""

Module for all linear-arrangement-dependent algorithms.
-----------------------------------

This modules contains functions to calculate properties of graphs that depend on a linear arrangement. Said arrangement van be given explicitly, i.e., by constructing a ``linear_arrangement`` object, or by omitting it in the functions to let these use the labelling of the graph (to read more about the concept of linear arrangement see the "Linear arrangement" page -- only available on html documentation). For example, given a graph

>>> g = lal.graphs.undirected_graph()

we can calculate the sum of length of the edges using function ``lal.linarr.sum_edge_lengths`` in two different ways. The first is by omitting the arrangement:

>>> lal.linarr.sum_edge_lengths(g)

or by giving one explicitly

>>> lal.linarr.sum_edge_lengths(g, [...])
"""
