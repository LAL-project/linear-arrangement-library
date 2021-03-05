r"""

LAL, the Linear Arrangement Library
---------------------------------------

This library offers a variety of algorithms related to linear arrangements of graphs so as to provide to researchers in the field of Quantitative Linguistics a toolset with which they can perform statistical analyses on different corpora of languages efficiently and effectively. Therefore, this library implements several state-of-the-art algorithms and offers a variety of functionalities. While most of the functions have been generalised to be applicable to graphs, we also provide specialised functions for trees, which are more efficient than their more general counterparts.

The main goal of this library is to provide algorithms with which the library's users can use to do statistical studies. One of the most attractive features offered in this library is that of treebank dataset processing. We offer a class that automatically processes a dataset and computes several metrics based on the capabilities of the library. See class ``lal.io.treebank_dataset_processor`` for details. We also provide classes for custom processing of treebanks (see ``lal.io.treebank_dataset_reader`` and ``lal.io.treebank_reader``).

All the features of syntactic dependency trees that can be calculated with the algorithms in this library are gathered in the modules lal.linarr and in lal.properties. These features include, but are not limited to,

- the sum of edge lengths :math:`D` (see ``lal.linarr.sum_length_edges``), and the expectation and variance of the sum of edge lengths (see ``lal.properties.expectation_D`` and ``lal.properties.variance_D``),
- calculation of optimal arrangements of free and rooted trees (see lal.linarr.Dmin); the enumeration ``lal.linarr.algorithms_Dmin`` lists all algorithms available,
- the number of crossings (see ``lal.linarr.n_crossings``), and the expectation and variance of the number of crossings (see ``lal.properties.expectation_C`` and ``lal.properties.variance_C``),
- any moment of the degree of the vertices of a graph (see ``lal.properties.mmt_degree``),
- the mean dependency distance (see ``lal.linarr.mean_dependency_distance``),
- the mean dependency distance over ensembles of graphs (see  ``lal.linarr.mean_dependency_distance_1level`` and ``lal.linarr.mean_dependency_distance_2level``),
- the mean hierarchical distance (see ``lal.properties.mean_hierarchical_distance``),
- the headedness of a tree (see ``lal.linarr.headedness``),
- the type of syntactic dependency trees according the their projectivity (see  ``lal.linarr.classify_tree_structure``).

Other algorithms, also gathered in the same modules, offer the computation of optimal arrangements. For example, it is offered
- the computation of the minimal arrangements with respect to the sum of edge lengths (see  ``lal.linarr.Dmin``), with free choice on the algorithm to be used (see ``lal.linarr.algorithms_Dmin``).
 
As extra features, useful for experimentation, are the generation of different types of trees, all of which are available in the  lal.generate module. We have implemented existing techniques (cited accordingly) or made or own to enumerate

- all labelled/unlabelled free trees,
- all labelled/unlabelled rooted trees,
- all projective arrangements of rooted trees,
 
and to generate uniformly at random

- labelled/unlabelled free/rooted trees,
- projective arrangements of rooted trees.
 
The documentation of each class include usage examples.
 
The basic data structures in this library
-----------------------------------------

In order to be able to use the library comfortably, its users must take good note of the different data structures that are the library's core.
 
	Exact integer and rational arithmetic
	

	Most operations in this library are done using exact integer and rational arithmetic. Such arithmetic is powered by the GMP library. We have wrapped their C structures into the classes  ``lal.numeric.integer`` and ``lal.numeric.rational``.

	The different types of graphs

	As it should be expected, this library offers a number of different graph abstractions: undirected graphs (see lal.graphs.undirected_graph), directed graphs (see lal.graphs.directed_graph), free trees (see lal.graphs.free_tree) and roted trees (see lal.graphs.rooted_tree), all of which can be found within the lal.graphs module.

	The internal structure of graphs
	
	Although all graphs should be regarded as unlabelled, each node carries an implicit labelling. Such labelling has a most trivial nature since each node is labelled with a number between 0 and the total number of vertices minus one.
	 
	Due to most graphs being sparse, the data structure of choice are adjacency lists where each vertex has a list of neighbouring nodes, or simply neighbours, associated to it. The user can affect the order of appearance of neighbours in multiple ways. One of them is, evidently, the order in which edges are added. Another way is via the lal.graphs.graph.normalise function, which sorts every list of neighbours increasingly. By default, the addition of edges is normalising, namely the following code
	
	>>> t = lal.graphs.free_tree(4)
	>>> t.add_edge(0,1,false,false).add_edge(0,3,false,false)
	  ...  .add_edge(0,2,false,false)
	>>> t.normalise()
	>>> print(t)
	0: 1 2 3
	1: 0
	2: 0
	3: 0
	
	produces the same output as
	
	>>> t = lal.graphs.free_tree(4)
	>>> t.add_edge(0,1).add_edge(0,3).add_edge(0,2)
	>>> print(t)
	0: 1 2 3
	1: 0
	2: 0
	3: 0
	
	The output is easy to interpret: the first line indicates the nodes are incident to vertex 0, the second line indicates the nodes incident to vertex 1, and so on. Without normalisation, the output is
	
	>>> print(t)
	0: 1 3 2
	1: 0
	2: 0
	3: 0
	
	where only the first line changes.
	
	Such normalisation is required by some of the algorithms in this library. Without proper normalisation, the algorithms are not likely to compute correct values. The parameter that governs the graphs' normalisation is called the normalisation parameter.
	
	The adjacency list structure has been extended to directed graphs in a way that the user can query them for in-degree (see ``lal.graphs.directed_graph.in_degree``) and in-neighbours (see ``lal.graphs.directed_graph.get_in_neighbours``).

Basic terminology and notation
------------------------------

Users will note, after browsing through the capabilities of the library, that several concepts are quite ubiquitous. The ``lal.node`` type is simply an unsigned integer type, and the ``lal.edge`` type is simply a pair of nodes.

A more advanced concept is that of linear arrangement (see ``lal.linear_arrangement``). In this library, a linear arrangement is viewed as a function that relates each node to a position in a linear sequence. Due to the properties of such functions, a linear arrangement is implemented with a list. Note that the concept of linear arrangement has been detached from that of trees, and the pair of a linear arrangement and a tree forms, in the context of the library, a syntactic dependency tree (this is why this class is not implemented). The symbol of choice for representing a linear arrangement in the library is the greek letter for the number pi :math:`\pi`.

Now, many functions (see those fuctions within the ``lal.linarr`` module) admit a linear arrangement which can be empty. Whenever it is empty, i.e., the value of the parameter is an empty vector, the positions of the nodes of the graphs in question are given by their implicit label. Such empty arrangement is called, in the context of the library, the identity arrangement symbolised with :math:`\pi_I`. Therefore, the following measurement of the sum of the lengths of the edges are equivalent

>>> t = lal.graphs.free_tree(4);
>>> t.add_edges([(0,1), (1,2), (2,3)])
>>> D1 = linarr.sum_length_edges(t)
>>> D2 = linarr.sum_length_edges(t, [0,1,2,3])

The possibility of expliciting a linear arrangement increases the flexibility of the library. For example, for the purposes of illustration, one can calculate the expected sum of the length of the edges as follows

>>> from itertools import permutations
>>> t = lal.graphs.free_tree(4)
>>> t.add_edges([(0,1), (1,2), (2,3)])
>>> Dt = lal.numeric.rational(0)
>>> perms = permutations([0,1,2,3])
>>> for p in list(perms):
>>> 		Dt += lal.linarr.sum_length_edges(t, p)
>>> Dt /= 24

However, we can calculate such expected value using the ``properties`` module:

>>> print(lal.properties.expectation_D_rational(t))

or, more simply,

>>> print(lal.properties.expectation_D(t))

Throughout the library we refer to the sum of length of edges with a capital D, :math:`D`, and we refer to the number of crossings with a capital C, :math:`C`.

Using the library effectively
-----------------------------

As a rule of the thumb, the user is encouraged not to change the default value of the parameters whenever they are given. However, certain operations can be less efficient than others, and sometimes it is even desirable to use values different from the default ones.

One the one hand, the wrong choice of operation can affect the library's performance gravely. For example, the addition/deletion of edges to/from graphs is slower when it is done edge by edge than when it is done in bulk. Users are highly encouraged to add/delete them in bulk using the appropriate functions (see, for example, ``lal.graphs.undirected_graph.add_edges`` and ``lal.graphs.undirected_graph.remove_edges``). The following code is discouraged

>>> t = lal.graphs.free_tree(10)
>>> for i in range(0,9):
>>>     u = input()
>>>     v = input()
>>>     t.add_edge(u,v)

while the next piece of code is strongly encouraged whenever possible

>>> t = lal.graphs.free_tree(10)
>>> e = []
>>> for i in range(0,9):
>>>     u = input()
>>>     v = input()
>>> t.add_edges(e)

A similar reasoning should be applied to the deletion of edges.

On the other hand, graphs are seldom required to be normalised. For example, when calculating the variance of :math:`C` (see ``lal.properties.variance_C``), it is mandatory that the graph be normalised, namely, the function has a precondition that requires the graph to be normalised. If such a function is to be called eventually then add all edges in bulk and with normalisation, or read the graph from disk also with normalisation. However, if such functions will never be called then the users are encouraged to set the normalisation parameter to false. For example, if the variance of :math:`C` is to be calculated,

>>> t = lal.graphs.free_tree()
>>> lal.io.read_edge_list(t)
>>> var_C = lal.properties.variance_C(t)

but if not

>>> t = lal.graphs.free_tree()
>>> lal.io.read_edge_list(t, false)
>>> ...



License notice
-------

Linear Arrangement Library - A library that implements a collection
algorithms for linear arrangments of graphs.

Copyright (C) 2019 - 2021

This file is part of Linear Arrangement Library. To see the full code
visit the webpage:
	https://github.com/lluisalemanypuig/linear-arrangement-library.git

Linear Arrangement Library is free software: you can redistribute it
and/or modify it under the terms of the GNU Affero General Public License
as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

Linear Arrangement Library is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.

Contact:

    Lluís Alemany-Puig (lalemany@cs.upc.edu)
        LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
        CQL (Complexity and Quantitative Linguistics Lab)
        Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
        Webpages:
            https://cqllab.upc.edu/people/lalemany/
            https://www.researchgate.net/profile/Lluis_Alemany-Puig
            
    Juan Luis Esteban (esteban@cs.upc.edu)
        Office 110, Omega building
        Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
        Webpages:
            https://www.cs.upc.edu/~esteban/
            https://www.researchgate.net/profile/Juan_Esteban13
    
    Ramon Ferrer-i-Cancho (rferrericancho@cs.upc.edu)
        LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
        CQL (Complexity and Quantitative Linguistics Lab)
        Office S124, Omega building
        Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
        Webpages:
            https://cqllab.upc.edu/people/rferrericancho/
            https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
"""
