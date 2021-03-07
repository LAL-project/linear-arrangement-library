r"""

Calculations on linear arrangements
-----------------------------------

This modules contains functions to calculate properties of graphs that depend on a linear arrangement. Said arrangement van be given explicitly, i.e., by constructing a @ref linear_arrangement object, or by omitting it in the functions to let these use the labelling of the graph. For example, given a graph

>>> g = lal.graphs.undirected_graph()

we can calculate the sum of length of the edges using function ``sum_length_edges`` in two different ways. The first is by omitting the arrangement:

>>> lal.linarr.sum_length_edges(g)

or by giving one explicitly

>>> lal.linarr.sum_length_edges(g, [...])

A linear arrangement object is a vector whose size is equal to the number of nodes of its corresponding graph. Note, however, that a linear arrangement has no associated graph; it is just a vector. Throughout this namespace we use the symbol for the number pi, :math:`\pi`, to denote a linear arrangement, as is usually done in the scientific literature.

The contents of a @ref linear_arrangement object are as follows: if :math:`\pi` is a linear arrangement then the `u`-th position of :math:`\pi` contains the position of that node in the arrangement. Formally, :math:`\pi[u] = p` if, and only if, node `u` is at position `p` in the linear arrangement.

The identity arrangement :math:`\pi_I` is a special case of linear arrangement used in many functions. Such an arrangement is one that maps each node into the position corresponding to their label, i.e., :math:`\pi_I(u) = u`. When omitting the arrangement in the functions of this namespace, the user should consider that the arrangement used is the identity arrangement.

"""
