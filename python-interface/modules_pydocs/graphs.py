r"""

The different classes of graphs
----------------------------------------

This module contains the data structures that implement graphs. Note that, although we do not support labelled graphs, the nodes of these graphs carry a label, a number between :math:`0` and :math:`n-1`, where :math:`n` is the number of nodes of the graph.

The functions (in other namespaces) that return a ``lal.node`` will always use a value greater than or equal to :math:`n` to denote that the node index is invalid.

See the concepts in "Graph Theory" page of this documentation for further details (available only on html documentation).
"""
