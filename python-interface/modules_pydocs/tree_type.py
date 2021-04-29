r"""
The different types of trees.

- ``bistar`` Bi-star trees

	These trees are made of two star trees joined by an edge at their centers.

- ``caterpillar`` Caterpillar trees

	These are the trees such that a linear tree is produced when its leaves are removed [2].

- ``linear`` Linear trees

	A linear tree has only two leaves, and the rest of the vertices have degree exactly two. This is, precisely, a path graph.

- ``quasistar`` Quasi star trees

	Also quasi star graphs, trees where all vertices but two have degree 1. One of these two vertices has degree exactly two, the other has degree at least two.

- ``spider`` Spider trees

	A spider tree has a unique vertex of degree greater than or equal to 3. The other vertices have degree 2 or 1 [1].

- ``star`` Star trees

	Also star graphs, trees where all vertices but one have degree 1.

- ``unknown``

	The tree could not be classified

References
----------

.. [1] Bennett, Patrick and English, Sean and Talanda-Fisher, Maria. Weighted turán problems with applications. *Discrete Mathematics*, 342(8):2165–2172, 2019. https://doi.org/10.1016/j.disc.2019.04.007.

.. [2] Frank Harary and Allen J. Schwenk. The number of caterpillars. *Discrete Mathematics*, 6:359–365, 1973. https://doi.org/10.1016/0012-365X(73)90067-8.

"""
