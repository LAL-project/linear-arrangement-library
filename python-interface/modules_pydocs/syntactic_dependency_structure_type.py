r"""
The different types of syntactic dependency tree structures.

Any tree with its nodes linearly arranged can be classified into several different classes.

We can currently identify the following structures:

- Projective structures.

	Value: ``projective``

	A structure is projective if it is ``planar`` and the root is not covered by any dependency.
	
- Planar structures.

	Value: ``planar``.

	A structure is planar if none of its dependencies cross. Two dependencies :math:`(s,t), (u,v)` cross if, and only if, their positions in the arrangement are interleaved, i.e., if :math:`\pi(s) < \pi(u) < \pi(t) < \pi(v)`, assuming that :math:`s` precedes :math:`t` and :math:`u` precedes :math:`v` in the arrangement.

- Well nested trees with maximum gap-degree 1.

	Value: ``WG1``
	
	For further details and a thorough discussion, see [2].

- 1-Endpoint Crossing.

	Value: ``EC1``

	A structure is 1-endpoint crossing if, given any dependency, all other dependencies crossing it are incident to a common node. See [1] for further details.
	
	EC1,

- Unknown class of syntantic dependency structure:

	Value: ``unknown``

	This value is used when the tree could not be classified into a particular type of dependency structure.

References
----------

.. [1] Giorgio Satta, Emily Pitler, Sampath Kannan, and Mitchell Marcus. Finding optimal 1-endpoint-crossing trees. In *Transactions of the Association for Computational Linguistics*, 2013, pages: 13-24.

.. [2] Carlos Gómez-Rodríguez, John Carroll, and David Weir. Dependency parsing schemata and mildly non-projective dependency parsing. *Computational Linguistics*, 37:541–586, 2011. 

"""
