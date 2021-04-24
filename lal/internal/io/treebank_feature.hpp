/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. To see the full code
 *  visit the webpage:
 *      https://github.com/lluisalemanypuig/linear-arrangement-library.git
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
#include <string_view>

// lal includes
#include <lal/io/treebank_feature.hpp>

namespace lal {
namespace internal {

/**
 * @brief Converts a @ref lal::io::treebank_feature value into a string.
 * @param tf A treebank feature.
 * @returns A string used to format output files.
 */
inline constexpr std::string_view
treebank_feature_string(const io::treebank_feature& tf) noexcept {
	using treebank_feature = io::treebank_feature;

	switch (tf) {
	case treebank_feature::num_nodes: return "n";
	case treebank_feature::second_moment_degree: return "second_moment_degree";
	case treebank_feature::second_moment_degree_in: return "second_moment_degree_in";
	case treebank_feature::second_moment_degree_out: return "second_moment_degree_out";
	case treebank_feature::third_moment_degree: return "third_moment_degree";
	case treebank_feature::third_moment_degree_in: return "third_moment_degree_in";
	case treebank_feature::third_moment_degree_out: return "third_moment_degree_out";
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

// return the index of the treebank feature
inline constexpr
std::size_t treebank_feature_to_index(const io::treebank_feature& tf) noexcept {
	return static_cast<std::size_t>(tf);
}

// return the treebank feature corresponding to the index
inline constexpr
io::treebank_feature index_to_treebank_feature(std::size_t idx) noexcept {
	return static_cast<io::treebank_feature>(idx);
}

} // -- namespace internal
} // -- namespace lal
