/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 *  Linear Arrangement Library is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU Affero General Public License
 *  as published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  Linear Arrangement Library is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *
 *      Lluís Alemany Puig (lalemany@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <cstdint>
#if defined DEBUG
#include <cassert>
#endif
#ifdef SWIG
#include <string>
#else
#include <string_view>
#endif

namespace lal {
namespace io {

/**
 * @brief The features that can be computed in automatic processing of treebanks.
 *
 * Classes @ref lal::io::treebank_collection_processor and
 * @ref lal::io::treebank_processor
 * are designed to process treebanks in an automatic fashion, meaning that they
 * process the trees and calculate a fixed set of features, the results of which
 * are stored in files. In this enumeration users will find a complete list of
 * all the features that can be calculated using those two classes.
 */
enum class treebank_feature {
	// STRUCTURAL PROPERTIES

	/// Number of nodes of the tree.
	num_nodes,
	/**
	 * @brief Second moment of degree \f$\langle k^2 \rangle\f$.
	 *
	 * See @ref lal::properties::moment_degree for details.
	 */
	second_moment_degree,
	/**
	 * @brief Second moment of out-degree \f$\langle k_{out}^2 \rangle\f$.
	 *
	 * See @ref lal::properties::moment_out_degree for details.
	 */
	second_moment_degree_out,
	/**
	 * @brief Third moment of degree \f$\langle k^3 \rangle\f$.
	 *
	 * See @ref lal::properties::moment_degree for details.
	 */
	third_moment_degree,
	/**
	 * @brief Third moment of out-degree \f$\langle k_{out}^3 \rangle\f$.
	 *
	 * See @ref lal::properties::moment_out_degree for details.
	 */
	third_moment_degree_out,

	/**
	 * @brief Sum of squared degrees.
	 *
	 * See @ref lal::properties::sum_powers_degrees for details.
	 */
	sum_squared_degrees,
	/**
	 * @brief Sum of squared out-degrees.
	 *
	 * See @ref lal::properties::sum_powers_out_degrees for details.
	 */
	sum_squared_out_degrees,
	/**
	 * @brief Sum of cube degrees.
	 *
	 * See @ref lal::properties::sum_powers_degrees for details.
	 */
	sum_cubed_degrees,
	/**
	 * @brief Sum of cube out-degrees.
	 *
	 * See @ref lal::properties::sum_powers_out_degrees for details.
	 */
	sum_cubed_out_degrees,

	/**
	 * @brief Size of the set \f$Q(T)\f$ of this tree \f$T\f$.
	 *
	 * See @ref lal::properties::num_pairs_independent_edges for details.
	 */
	num_pairs_independent_edges,
	/**
	 * @brief Headedness of the tree.
	 *
	 * In case the tree has only one vertex, outputs a NaN value.
	 *
	 * See @ref lal::linarr::head_initial for details.
	 */
	head_initial,
	/**
	 * @brief Hubiness of the tree.
	 *
	 * In case a tree has 3 or less vertices the output is a 'NaN' value.
	 *
	 * See @ref lal::properties::hubiness for details.
	 */
	hubiness,
	/**
	 * @brief Mean hierarchical distance of the tree.
	 *
	 * In case a tree has 1 vertex the output is a 'NaN' value.
	 *
	 * See @ref lal::properties::mean_hierarchical_distance for details.
	 */
	mean_hierarchical_distance,
	/**
	 * @brief Centre of the tree.
	 *
	 * This feature spans two columns, one for each possible central vertex.
	 * Each column contains an index: the first is always strictly less than the
	 * number of vertices, and the second is only valid when its value is strictly
	 * less than the number of vertices.
	 *
	 * See @ref lal::properties::tree_centre for details on the definition of
	 * centre of a tree.
	 */
	tree_centre,
	/**
	 * @brief Centroid of the tree.
	 *
	 * This feature spans two columns, one for each possible centroidal vertex.
	 * Each column contains an index: the first is always strictly less than the
	 * number of vertices, and the second is only valid when its value is strictly
	 * less than the number of vertices.
	 *
	 * See @ref lal::properties::tree_centroid for details on the definition of
	 * centroid of a tree.
	 */
	tree_centroid,
	/**
	 * @brief Diameter of the tree.
	 *
	 * See @ref lal::properties::tree_diameter for details.
	 */
	tree_diameter,

	// C

	/**
	 * @brief Number of edge crossings \f$C\f$.
	 *
	 * See @ref lal::linarr::num_crossings for details.
	 */
	num_crossings,
	/**
	 * @brief Prediction of the number of crossings \f$C\f$.
	 *
	 * See @ref lal::linarr::predicted_num_crossings for details.
	 */
	predicted_num_crossings,
	/**
	 * @brief First moment of expectation of \f$C\f$, \f$E[C]\f$.
	 *
	 * See @ref lal::properties::exp_num_crossings for details.
	 */
	exp_num_crossings,
	/**
	 * @brief Variance of \f$C\f$, \f$V[C]\f$.
	 *
	 * See @ref lal::properties::var_num_crossings for details.
	 */
	var_num_crossings,
	/**
	 * @brief z-score of \f$C\f$, \f$\frac{C - E[C]}{\sqrt{V[C]}}\f$.
	 *
	 * See @ref lal::properties::var_num_crossings_tree for details on how the
	 * variance of \f$C\f$, \f$V[C]\f$, is calculated.
	 */
	z_score_num_crossings,

	// D

	/**
	 * @brief Sum of length of edges \f$D\f$.
	 *
	 * See @ref lal::linarr::sum_edge_lengths for details.
	 */
	sum_edge_lengths,
	/**
	 * @brief Expectation of \f$D\f$, \f$E[D]\f$.
	 *
	 * See @ref lal::properties::exp_sum_edge_lengths for details.
	 */
	exp_sum_edge_lengths,
	/**
	 * @brief Expectation of \f$D\f$ constrained to projective arrangements, \f$E_{\mathrm{pr}}[D]\f$.
	 *
	 * See @ref lal::properties::exp_sum_edge_lengths_projective for details.
	 */
	exp_sum_edge_lengths_projective,
	/**
	 * @brief Expectation of \f$D\f$ constrained to planar arrangements, \f$E_{\mathrm{pl}}[D]\f$.
	 *
	 * See @ref lal::properties::exp_sum_edge_lengths_planar for details.
	 */
	exp_sum_edge_lengths_planar,
	/**
	 * @brief Variance of \f$D\f$, \f$V[D]\f$.
	 *
	 * See @ref lal::properties::var_sum_edge_lengths for details.
	 */
	var_sum_edge_lengths,
	/**
	 * @brief z-score of \f$D\f$, \f$\frac{D - E[D]}{\sqrt{V[D]}}\f$.
	 *
	 * See @ref lal::properties::var_sum_edge_lengths for details on how the
	 * variance of \f$D\f$, \f$V[D]\f$, is calculated.
	 */
	z_score_sum_edge_lengths,
	/**
	 * @brief Unconstrained minimum sum of length of edges.
	 *
	 * See @ref lal::linarr::algorithms_Dmin for details. The algorithm used
	 * is @ref lal::linarr::algorithms_Dmin::Shiloach.
	 */
	min_sum_edge_lengths,
	/**
	 * @brief Minimum sum of length of edges under the planary constraint.
	 *
	 * See @ref lal::linarr::min_sum_edge_lengths for details.
	 */
	min_sum_edge_lengths_planar,
	/**
	 * @brief Minimum sum of length of edges under the planary constraint.
	 *
	 * See @ref lal::linarr::min_sum_edge_lengths_projective for details.
	 */
	min_sum_edge_lengths_projective,
	/**
	 * @brief Mean dependency distance of the tree.
	 *
	 * In case a tree has 1 vertex the output is a 'NaN' value.
	 *
	 * See @ref lal::linarr::mean_dependency_distance for details.
	 */
	mean_dependency_distance,

	// FLUXES

	/**
	 * @brief Maximum flux weight.
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	flux_max_weight,
	/**
	 * @brief Mean flux weight.
	 *
	 * This is the sum of weights averaged by the number of fluxes (the number
	 * of vertices of the tree minus 1).
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details on the definition of weight.
	 */
	flux_mean_weight,
	/**
	 * @brief Minimum flux weight.
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	flux_min_weight,

	/**
	 * @brief Maximum left span.
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	flux_max_left_span,
	/**
	 * @brief Mean left span.
	 *
	 * This is the sum of left spans averaged by the number of fluxes (the
	 * number of vertices of the tree minus 1).
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details on the definition of left span.
	 */
	flux_mean_left_span,
	/**
	 * @brief Minimum left span.
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref linarr::dependency_flux for details.
	 */
	flux_min_left_span,

	/**
	 * @brief Maximum right span.
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	flux_max_right_span,
	/**
	 * @brief Mean right span.
	 *
	 * This is the sum of right spans averaged by the number of fluxes (the
	 * number of vertices of the tree minus 1).
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details on the definition of right span.
	 */
	flux_mean_right_span,
	/**
	 * @brief Minimum right span.
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	flux_min_right_span,

	/**
	 * @brief Maximum flux size.
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	flux_max_size,
	/**
	 * @brief Mean flux size.
	 *
	 * This is the sum of flux sizes averaged by the number of fluxes (the
	 * number of vertices of the tree minus 1).
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details on the definition of flux size.
	 */
	flux_mean_size,
	/**
	 * @brief Minimum flux size.
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	flux_min_size,

	/**
	 * @brief Maximum R/L ratio.
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	flux_max_RL_ratio,
	/**
	 * @brief Mean R/L ratio.
	 *
	 * This is the sum of R/L ratios averaged by the number of fluxes (the
	 * number of vertices of the tree minus 1).
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details on the definition of R/L ratio.
	 */
	flux_mean_RL_ratio,
	/**
	 * @brief Minimum R/L ratio.
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	flux_min_RL_ratio,

	/**
	 * @brief Maximum W/S ratio.
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	flux_max_WS_ratio,
	/**
	 * @brief Mean W/S ratio.
	 *
	 * This is the sum of W/S ratios averaged by the number of fluxes (the
	 * number of vertices of the tree minus 1).
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details on the definition of W/S ratio.
	 */
	flux_mean_WS_ratio,
	/**
	 * @brief Minimum W/S ratio.
	 *
	 * In case the tree has 1 vertex, the output is a 'nan' value.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	flux_min_WS_ratio,

	// TYPES OF TREES

	/**
	 * @brief The type of tree.
	 *
	 * This feature spans as many columns as types of trees are available in this
	 * library. Each column will contain either a 0 or a 1 depending on whether
	 * or not the tree can be classified into that type of tree.
	 *
	 * See @ref lal::graphs::tree_type for a complete list of tree types.
	 */
	tree_type,

	/**
	 * @brief The type of syntactic dependency structure.
	 *
	 * This feature spans as many columns as types of syntactic dependency
	 * structure are available in this library. Each column will contain either
	 * a 0 or a 1 depending on whether or not the tree can be classified into
	 * that syntactic dependency structure.
	 *
	 * See @ref lal::linarr::syntactic_dependency_structure for a complete
	 * list of types.
	 */
	syntactic_dependency_structure_class,

	/*
	 * @brief The last value of the enumeration.
	 *
	 * <b>This is used for internal purposes only.</b> Do not use this feature.
	 */
	__last_value
};

/**
 * @brief The total number of features available.
 *
 * This number equals the total amount of values within the enumeration
 * @ref lal::io::treebank_feature, except the last one (which should never be
 * used).
 */
static constexpr
std::size_t __treebank_feature_size =
	static_cast<std::size_t>(treebank_feature::__last_value);

/**
 * @brief Converts a @ref lal::io::treebank_feature value into a string.
 * @param tf A treebank feature.
 * @returns A string used to format output files.
 */
inline constexpr
#ifdef SWIG
std::string
#else
std::string_view
#endif
treebank_feature_string(const io::treebank_feature& tf)
noexcept
{
	switch (tf) {
	case treebank_feature::num_nodes: return "n";
	case treebank_feature::second_moment_degree: return "second_moment_degree";
	case treebank_feature::second_moment_degree_out: return "second_moment_degree_out";
	case treebank_feature::third_moment_degree: return "third_moment_degree";
	case treebank_feature::third_moment_degree_out: return "third_moment_degree_out";
	case treebank_feature::sum_squared_degrees: return "sum_squared_degrees";
	case treebank_feature::sum_squared_out_degrees: return "sum_squared_out_degrees";
	case treebank_feature::sum_cubed_degrees: return "sum_cubed_degrees";
	case treebank_feature::sum_cubed_out_degrees: return "sum_cubed_out_degrees";
	case treebank_feature::num_pairs_independent_edges: return "num_pairs_independent_edges";
	case treebank_feature::head_initial: return "head_initial";
	case treebank_feature::hubiness: return "hubiness";
	case treebank_feature::mean_hierarchical_distance: return "mean_hierarchical_distance";
	case treebank_feature::tree_centre: return "tree_centre";
	case treebank_feature::tree_centroid: return "tree_centroid";
	case treebank_feature::tree_diameter: return "tree_diameter";
	case treebank_feature::num_crossings: return "num_crossings";
	case treebank_feature::predicted_num_crossings: return "predicted_num_crossings";
	case treebank_feature::exp_num_crossings: return "exp_num_crossings";
	case treebank_feature::var_num_crossings: return "var_num_crossings";
	case treebank_feature::z_score_num_crossings: return "z_score_num_crossings";
	case treebank_feature::sum_edge_lengths: return "sum_edge_lengths";
	case treebank_feature::exp_sum_edge_lengths: return "exp_sum_edge_lengths";
	case treebank_feature::exp_sum_edge_lengths_projective: return "exp_sum_edge_lengths_projective";
	case treebank_feature::exp_sum_edge_lengths_planar: return "exp_sum_edge_lengths_planar";
	case treebank_feature::var_sum_edge_lengths: return "var_sum_edge_lengths";
	case treebank_feature::z_score_sum_edge_lengths: return "z_score_sum_edge_lengths";
	case treebank_feature::min_sum_edge_lengths: return "min_sum_edge_lengths";
	case treebank_feature::min_sum_edge_lengths_planar: return "min_sum_edge_lengths_planar";
	case treebank_feature::min_sum_edge_lengths_projective: return "min_sum_edge_lengths_projective";
	case treebank_feature::mean_dependency_distance: return "mean_dependency_distance";
	case treebank_feature::flux_max_weight: return "flux_max_weight";
	case treebank_feature::flux_mean_weight: return "flux_mean_weight";
	case treebank_feature::flux_min_weight: return "flux_min_weight";
	case treebank_feature::flux_max_left_span: return "flux_max_left_span";
	case treebank_feature::flux_mean_left_span: return "flux_mean_left_span";
	case treebank_feature::flux_min_left_span: return "flux_min_left_span";
	case treebank_feature::flux_max_right_span: return "flux_max_right_span";
	case treebank_feature::flux_mean_right_span: return "flux_mean_right_span";
	case treebank_feature::flux_min_right_span: return "flux_min_right_span";
	case treebank_feature::flux_max_RL_ratio: return "flux_max_RL_ratio";
	case treebank_feature::flux_mean_RL_ratio: return "flux_mean_RL_ratio";
	case treebank_feature::flux_min_RL_ratio: return "flux_min_RL_ratio";
	case treebank_feature::flux_max_WS_ratio: return "flux_max_WS_ratio";
	case treebank_feature::flux_mean_WS_ratio: return "flux_mean_WS_ratio";
	case treebank_feature::flux_min_WS_ratio: return "flux_min_WS_ratio";
	case treebank_feature::flux_max_size: return "flux_max_size";
	case treebank_feature::flux_mean_size: return "flux_mean_size";
	case treebank_feature::flux_min_size: return "flux_min_size";
	case treebank_feature::tree_type: return "tree_type";
	case treebank_feature::syntactic_dependency_structure_class:
		return "syntactic_dependency_structure_class";
	case treebank_feature::__last_value: return "__last_value";
	}
	// should never happen
	return "???";
}

/// Returns the index of the input treebank feature
inline constexpr
std::size_t treebank_feature_to_index(const io::treebank_feature& tf) noexcept {
	return static_cast<std::size_t>(tf);
}

/// Returns the treebank feature corresponding to the input index
inline constexpr
io::treebank_feature index_to_treebank_feature(std::size_t idx) noexcept {
#if defined DEBUG
	assert(idx < treebank_feature_to_index(treebank_feature::__last_value));
#endif
	return static_cast<io::treebank_feature>(idx);
}

/// Returns the treebank feature corresponding to the index as a string.
inline constexpr
#ifdef SWIG
std::string
#else
std::string_view
#endif
treebank_feature_index_to_string(std::size_t idx) noexcept {
	return treebank_feature_string(index_to_treebank_feature(idx));
}

} // -- namespace io
} // -- namespace lal
