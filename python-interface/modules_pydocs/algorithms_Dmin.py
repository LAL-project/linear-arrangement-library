r"""
The different algorithms for computing the minimum sum of the length of the edges :math:`D` 

This enumeration's values are used to choose the algorithm which the functions ``lal.linarr.Dmin`` use to compute the minimum value of the sum of the length the edges :math:`D` 

Algorithms for rooted trees
---------------------------

- Algorithm to calculate optimal projective arrangements of rooted trees.

	Value: ``Projective``

	Computes an optimal projective linear arrangement for rooted trees. A projective linear arrangement is an arrangement in which there are no edge crossings and the root is not covered by any edge.

	This option implements the algorithm in [1]. A non-linear time algorithm to solve this problem was oulined in [2].

	Preconditions:
	
	* The input rooted tree has to have the size of its subtrees calculated (see ``lal.graphs.rooted_tree.calculate_size_subtrees`` and ``lal.graphs.rooted_tree.size_subtrees_valid``).


Algorithms for free trees
-------------------------

- Algorithm to calculate optimal planar arrangements of free trees.

	Value: ``Planar``

	Computes an optimal planar linear arrangement for free trees. A planar linear arrangement is an arrangement in which there are no edge crossings.

	This implements the algorithm published by Hochberg and Stallmann [3]. A previous algorithm solving the same problem was published by Iordanskii [4]. See [1] for a review.

- Algorithm to calculate unconstrained an optimal linearization of free trees.

	Value: ``Unconstrained_YS``

	Computes an unconstrained optimal linear arrangement of a free tree and the value of its cost, i.e., the sum of the lengths of the edges.

	This implements Yossi Shiloach's algorithm published in [5]. The implementation of this algorithm applies the corrections published in [6].

- Algorithm to calculate unconstrained an optimal linearization of free trees.

	Value: ``Unconstrained_FC``

	Computes an unconstrained optimal linear arrangement of a free tree and the value of its cost, i.e., the sum of the lengths of the edges.

	This implements Fan Chung's quadratic algorithm (Section 3) published in [7].

References:
-----------

.. [1] Lluís Alemany-Puig, Juan Luis Esteban, and Ramon Ferrer-i-Cancho. Minimum projective linearizations of trees in linear time. Arxiv 2102.03277, 2021.

.. [2] Daniel Gildea and David Temperley. Optimizing grammars for minimum dependency length. *ACL 2007 - Proceedings of the 45th Annual Meeting of the Association for Computational Linguistics*, pages 184-191, 2007.

.. [3] Robert A. Hochberg and Matthias F. Stallmann. Optimal one-page tree embeddings in linear time. In *Information Processing Letters*, 87(2):59 - 66, 2003. https://doi.org/10.1016/S0020-0190(03)00261-8.

.. [4] M. A. Iordanskii. Minimal numberings of the vertices of trees - Approximate
   approach. In Lothar Budach, Rais Gatic Bukharajev, and Oleg Borisovic Lupanov, editors, *Fundamentals of Computation Theory*, pages 214-217, Berlin, Heidelberg, 1987. Springer Berlin Heidelberg.

.. [5] Yossi Shiloach. A minimum linear arrangement algorithm for undirected trees. *Society for Industrial and Applied Mathematics*, 8(1):15-32, 1979. https://doi.org/10.1137/0208002.

.. [6] Juan Luis Esteban and Ramon Ferrer-i Cancho. A correction on Shiloach's algorithm for minimum linear arrangements of trees. *Society for Industrial and Applied Mathematics*, 46(3):1146-1151, 2017. https://doi.org/10.1137/15M1046289.

.. [7] F. R. K. Chung. On optimal linear arrangements of trees. *Computers and
   Mathematics with applications*, 10(1):43-60, 1984. https://doi.org/10.1016/0898-1221(84)90085-3.

"""
