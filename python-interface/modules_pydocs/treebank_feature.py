r"""
The features that can be computed in automatic processing of treebanks.

Classes ``lal.io.treebank_collection_processor`` and ``lal.io.treebank_processor`` are designed to process treebanks in an automatic fashion, meaning that they process the trees and calculate a fixed set of features, the results of which are stored in files. In this enumeration users will find a complete list of all the features that can be calculated using those two classes.

Users will find the feature values after every bullet marker.

- ``num_nodes`` : Number of nodes.
- ``k2`` : Second moment of degree :math:`<k^2>`. See ``lal.properties.mmt_degree`` for details.
- ``k3`` : Third moment of degree :math:`<k^3>`. See ``lal.properties.mmt_degree`` for details.
- ``num_pairs_independent_edges`` : Size of the set :math:`Q(T)` of this tree :math:`T`. See ``lal.properties.size_Q`` for details.
- ``mean_hierarchical_distance`` : Mean hierarchical distance of the tree. See ``lal.properties.mean_hierarchical_distance`` for details.
- ``mean_dependency_distance`` : Mean dependency distance of the tree. See ``lal.linarr.mean_dependency_distance`` for details.
- ``headedness`` : Headedness of the tree. See ``lal.linarr.headedness`` for details.

C
-----

- ``C`` : Number of edge crossings :math:`C`. See ``lal.linarr.algorithms_C`` for details.
- ``C_expected`` : First moment of expectation of :math:`C`, :math:`E[C]`. See ``lal.properties.expectation_C`` for details.
- ``C_variance`` : Variance of :math:`C`, :math:`V[C]`. See ``lal.properties.variance_C_tree`` for details.
- ``C_z_score`` : z-score of :math:`C`, :math:`\frac{C - E[C]}{\sqrt{V[C]}}`. See ``lal.properties.variance_C_tree`` for details on how the variance of :math:`C`, :math:`V[C]`, is calculated.

D
-----

- ``D`` : Sum of length of edges :math:`D`. See ``lal.linarr.sum_length_edges`` for details.
- ``D_expected`` : First moment of expectation of :math:`D`, :math:`E[D]`. See ``lal.properties.expectation_D`` for details.
- ``D_variance`` : Variance of :math:`D`, :math:`V[D]`. See ``lal.properties.variance_D`` for details.
- ``D_z_score`` : z-score of :math:`D` , :math:`\frac{D - E[D]}{\sqrt{V[D]}}`. See ``lal.properties.variance_D`` for details on how the variance of :math:`D`, :math:`V[D]`, is calculated.

Minimum Linear arrangement
------

- ``Dmin`` : Unconstrained minimum sum of length of edges. See ``lal.linarr.algorithms_Dmin.Unconstrained_YS``, or ``lal.linarr.algorithms_Dmin.Unconstrained_FC`` for details.
- ``Dmin_Planar`` : Minimum sum of length of edges under the planary constraint. See ``lal.linarr.Dmin_Planar`` for details.
- ``Dmin_Projective`` : Minimum sum of length of edges under the planary constraint. See ``lal.linarr.Dmin_Projective`` for details.

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

- ``tree_type` The type of tree. See ``lal.graphs.tree_type`` for a complete list of tree types.
- ``syntactic_dependency_structure_type` The type of syntactic dependency structure. See ``lal.linarr.syntactic_dependency_structure_type`` for a complete list of types.

"""
