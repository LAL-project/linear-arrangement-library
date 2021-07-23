r"""
The features that can be computed in automatic processing of treebanks.

Classes ``lal.io.treebank_collection_processor`` and ``lal.io.treebank_processor`` are designed to process treebanks in an automatic fashion, meaning that they process the trees and calculate a fixed set of features, the results of which are stored in files. In this enumeration users will find a complete list of all the features that can be calculated using those two classes.

Users will find the feature values after every bullet marker.

Structural properties
----------

- ``num_nodes`` : Number of nodes.
- ``second_moment_degree`` : Second moment of degree :math:`<k^2>`. See ``lal.properties.moment_degree`` for details.
- ``second_moment_degree_in`` : Second moment of degree :math:`<k_{in}^2>`. See ``lal.properties.moment_degree_in`` for details.
- ``second_moment_degree_out`` : Second moment of degree :math:`<k_{out}^2>`. See ``lal.properties.moment_degree_out`` for details.
- ``third_moment_degree`` : Third moment of degree :math:`<k^3>`. See ``lal.properties.moment_degree`` for details.
- ``third_moment_degree_in`` : Third moment of degree :math:`<k_{in}^3>`. See ``lal.properties.moment_degree_in`` for details.
- ``third_moment_degree_out`` : Third moment of degree :math:`<k_{out}^3>`. See ``lal.properties.moment_degree_out`` for details.
- ``num_pairs_independent_edges`` : Size of the set :math:`Q(T)` of this tree :math:`T`. See ``lal.properties.num_pairs_independent_edges`` for details.
- ``mean_hierarchical_distance`` : Mean hierarchical distance of the tree. See ``lal.properties.mean_hierarchical_distance`` for details.
- ``head_initial`` : Headedness of the tree. See ``lal.linarr.head_initial`` for details.
- ``tree_centre`` : Centre of the tree. This feature spans two columns, one for each possible central vertex. Each column contains an index: the first is always strictly less than the number of vertices, and the second is only valid when its value is strictly less than the number of vertices. See ``lal.properties.tree_centre`` for details on the definition of centre of a tree.
- ``tree_centroid`` : Centroid of the tree. This feature spans two columns, one for each possible centroidal vertex. Each column contains an index: the first is always strictly less than the number of vertices, and the second is only valid when its value is strictly less than the number of vertices. See ``lal.properties.tree_centroid`` for details on the definition of centroid of a tree.
- ``tree_diameter`` : Diameter of the tree. See ``lal.properties.tree_diameter`` for details.

C
-----

- ``num_crossings`` : Number of edge crossings :math:`C`. See ``lal.linarr.algorithms_C`` for details.
- ``predicted_num_crossings`` : Prediction of the number of crossings :math:`C`. See ``lal.linarr.predicted_num_crossings`` for details.
- ``exp_num_crossings`` : First moment of expectation of :math:`C`, :math:`E[C]`. See ``lal.properties.exp_num_crossings`` for details.
- ``var_num_crossings`` : Variance of :math:`C`, :math:`V[C]`. See ``lal.properties.var_num_crossings`` for details.
- ``z_score_num_crossings`` : z-score of :math:`C`, :math:`\frac{C - E[C]}{\sqrt{V[C]}}`. See ``lal.properties.var_num_crossings_tree`` for details on how the variance of :math:`C`, :math:`V[C]`, is calculated.

D
-----

- ``sum_edge_lengths`` : Sum of length of edges :math:`D`. See ``lal.linarr.sum_length_edges`` for details.
- ``exp_sum_edge_lengths`` : Expectation of :math:`D`, :math:`E[D]`. See ``lal.properties.exp_sum_edge_lengths`` for details.
- ``exp_sum_edge_lengths_projective`` : Expectation of :math:`D` constrained to projective arrangements, :math:`E_{pr}[D]`. See ``lal.properties.exp_sum_edge_lengths_projective`` for details.
- ``exp_sum_edge_lengths_planar`` : Expectation of :math:`D` constrained to planar arrangements, :math:`E_{pl}[D]`. See ``lal.properties.exp_sum_edge_lengths_planar`` for details.
- ``var_sum_edge_lengths`` : Variance of :math:`D`, :math:`V[D]`. See ``lal.properties.var_sum_edge_lengths`` for details.
- ``z_score_sum_edge_lengths`` : z-score of :math:`D` , :math:`\frac{D - E[D]}{\sqrt{V[D]}}`. See ``lal.properties.var_sum_edge_lengths`` for details on how the variance of :math:`D`, :math:`V[D]`, is calculated.
- ``min_sum_edge_lengths`` : Unconstrained minimum sum of length of edges. See ``lal.linarr.algorithms_Dmin`` for details. The default algorithm used is ``lal.linarr.algorithms_Dmin.Shiloach``.
- ``min_sum_edge_lengths_planar`` : Minimum sum of length of edges under the planary constraint. See ``lal.linarr.min_sum_edge_lengths_planar`` for details.
- ``min_sum_edge_lengths_projective`` : Minimum sum of length of edges under the planary constraint. See ``lal.linarr.min_sum_edge_lengths_projective`` for details.
- ``mean_dependency_distance`` : Mean dependency distance of the tree. See ``lal.linarr.mean_dependency_distance`` for details.

Dependency Flux
----------

All the following features are defined in class ``lal.linarr.dependency_flux``; read its documentation for further details.

- ``flux_max_weight`` : Maximum flux weight.
- ``flux_mean_weight`` : Mean flux weight. This is the sum of weights averaged by the number of fluxes (the number of vertices of the tree minus 1).
- ``flux_min_weight`` : Minimum flux weight.
- ``flux_max_left_span`` : Maximum left span.
- ``flux_mean_left_span`` : Mean left span. This is the sum of left spans averaged by the number of fluxes (the number of vertices of the tree minus 1). See ``lal.linarr.dependency_flux`` for details on the definition of left span.
- ``flux_min_left_span`` : Minimum left span.
- ``flux_max_right_span`` : Maximum right span.
- ``flux_mean_right_span`` : Mean right span. This is the sum of right spans averaged by the number of fluxes (the number of vertices of the tree minus 1).
- ``flux_min_right_span`` : Minimum right span.
- ``flux_max_size`` : Maximum flux size.
- ``flux_mean_size`` : Mean flux size. This is the sum of flux sizes averaged by the number of fluxes (the number of vertices of the tree minus 1).
- ``flux_min_size`` : Minimum flux size.
- ``flux_max_RL_ratio`` : Maximum R/L ratio.
- ``flux_mean_RL_ratio`` : Mean R/L ratio. This is the sum of R/L ratios averaged by the number of fluxes (the number of vertices of the tree minus 1).
- ``flux_min_RL_ratio`` : Minimum R/L ratio.
- ``flux_max_WS_ratio`` : Maximum W/S ratio.
- ``flux_mean_WS_ratio`` : Mean W/S ratio. This is the sum of W/S ratios averaged by the number of fluxes (the number of vertices of the tree minus 1).
- ``flux_min_WS_ratio`` : Minimum W/S ratio.

Types of structures
-----------

- ``tree_type` The type of tree. This feature spans as many columns as types of trees are available in this library. Each column will contain either a 0 or a 1 depending on whether or not the tree can be classified into that type of tree. See ``lal.graphs.tree_type`` for a complete list of tree types.
- ``syntactic_dependency_structure_class` The type of syntactic dependency structure. This feature spans as many columns as types of syntactic dependency structure are available in this library. Each column will contain either a 0 or a 1 depending on whether or not the tree can be classified into that syntactic dependency structure. See ``lal.linarr.syntactic_dependency_structure_class`` for a complete list of types.

"""
