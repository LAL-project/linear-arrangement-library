r"""
The different algorithms for computing the minimum sum of the length of the edges :math:`D` in planar arrangements of free trees.

Recall that a planar arrangement is one in which there are no edge crossings.

This enumeration's values are used to choose the algorithm which the functions ``lal.linarr.min_sum_edge_lengths_planar`` use to compute the minimum value of the sum of the length of the edges :math:`D`.

Algorithms for free trees
-------------------------

- Alemany-Puig, Esteban, Ferrer-i-Cancho's algorithm.

	Value: ``AlemanyEstebanFerrer``
	
	Interval-based approach to the calculation of minimum planar arrangements. Algorithm published in [1].

- Hochbger, Stallmann's algorithm.

	Value: ``HochbergStallmann``

	Displacement-based approach to the calculation of minimum planar arrangements. The algorithm was originally published in [2], however, the implementation uses the correction in [1].

References:
-----------

.. [1] Alemany-Puig, Lluís and Esteban, Juan Luis and Ferrer-i-Cancho, Ramon. Minimum projective linearization of trees in linear time. *Arxiv*, 2021. http://arxiv.org/abs/2102.03277.

.. [2] Robert A. Hochberg and Matthias F. Stallmann. Optimal one-page tree embeddings in linear time. Information Processing Letters, 87(2):59–66, 2003. 

"""
