r"""
The different algorithms for computing the number of crossings.

Two edges :math:`\{s,t\},\{u,v\}` of a graph :math:`G` cross in a linear arrangement :math:`\pi` of its nodes if, and only if, their positions interleave in the linear arrangement. More formally, given an arrangement :math:`\pi` of a graph :math:`G`, the edges :math:`\{s,t\},\{u,v\}` cros iff :math:`\pi(s) < \pi(u) < \pi(t) < \pi(v)`.

The algorithms available in this library are:

Bruteforce algorithm
--------------------

Value: ``brute_force``.

Complexity: time :math:`O(m^2)`, space :math:`O(1)`.

Dynamic programming algorithm (1)
---------------------------------

Value: ``dynamic_programming``. 

Complexity: time :math:`O(n^2)`, space :math:`O(n^2)`.

Dynamic programming algorithm (2)
---------------------------------

Value: ``ladder``.

Complexity: time :math:`O(n^2)`, space :math:`O(n)`.

Sort-based algorithm
--------------------

Value: ``stack_based``.

Complexity: time :math:`O(m\log n)`, space :math:`O(m)`.
"""
