r"""
The different algorithms for computing the minimum sum of the length of the edges :math:`D` 

This enumeration's values are used to choose the algorithm which the functions ``lal.linarr.Dmin`` use to compute the minimum value of the sum of the length the edges :math:`D` 

Algorithms for free trees
-------------------------

- Yossi Shiloach's algorithm to calculate unconstrained optimal linearization of free trees.

	Value: ``Unconstrained_YS``
	
	This value makes the ``lal.linarr.Dmin`` function choose the implementation
	of Yossi Shiloach's algorithm. This algorithm was published in [1].
	The implementation of this algorithm applies the corrections published
	in [2].

- Fan Chung's algorithm to calculate unconstrained optimal linearization of free trees.

	Value: ``Unconstrained_FC``

	This value makes the ``lal.linarr.Dmin`` function choose the implementation
	of Fan Chung's algorithm. This algorithm was published in \cite [3].
	In particular, this implements Fan Chung's quadratic algorithm (Section 3).

References:
-----------

.. [1] Yossi Shiloach. A minimum linear arrangement algorithm for undirected trees. *Society for Industrial and Applied Mathematics*, 8(1):15-32, 1979. https://doi.org/10.1137/0208002.

.. [2] Juan Luis Esteban and Ramon Ferrer-i Cancho. A correction on Shiloach's algorithm for minimum linear arrangements of trees. *Society for Industrial and Applied Mathematics*, 46(3):1146-1151, 2017. https://doi.org/10.1137/15M1046289.

.. [3] F. R. K. Chung. On optimal linear arrangements of trees. *Computers and
   Mathematics with applications*, 10(1):43-60, 1984. https://doi.org/10.1016/0898-1221(84)90085-3.

"""
