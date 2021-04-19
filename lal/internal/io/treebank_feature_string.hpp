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
	case treebank_feature::k2: return "k2";
	case treebank_feature::k3: return "k3";
	case treebank_feature::num_pairs_independent_edges: return "size_Q";
	case treebank_feature::headedness: return "headedness";
	case treebank_feature::mean_hierarchical_distance: return "mean_hierarchical_distance";
	case treebank_feature::mean_dependency_distance: return "mean_dependency_distance";
	case treebank_feature::C: return "C";
	case treebank_feature::C_expected: return "C_expected";
	case treebank_feature::C_variance: return "C_variance";
	case treebank_feature::C_z_score: return "C_z_score";
	case treebank_feature::D: return "D";
	case treebank_feature::D_expected: return "D_expected";
	case treebank_feature::D_variance: return "D_variance";
	case treebank_feature::D_z_score: return "D_z_score";
	case treebank_feature::Dmin: return "Dmin";
	case treebank_feature::Dmin_Planar: return "Dmin_Planar";
	case treebank_feature::Dmin_Projective: return "Dmin_Projective";
	case treebank_feature::flux_max_weight: return "flux_max_weight";
	case treebank_feature::flux_mean_weight: return "flux_mean_weight";
	case treebank_feature::flux_min_weight: return "flux_min_weight";
	case treebank_feature::flux_max_left_span: return "max_left_span";
	case treebank_feature::flux_mean_left_span: return "mean_left_span";
	case treebank_feature::flux_min_left_span: return "min_left_span";
	case treebank_feature::flux_max_right_span: return "max_right_span";
	case treebank_feature::flux_mean_right_span: return "mean_right_span";
	case treebank_feature::flux_min_right_span: return "min_right_span";
	case treebank_feature::flux_max_RL_ratio: return "max_RL_ratio";
	case treebank_feature::flux_mean_RL_ratio: return "mean_RL_ratio";
	case treebank_feature::flux_min_RL_ratio: return "min_RL_ratio";
	case treebank_feature::flux_max_WS_ratio: return "max_WS_ratio";
	case treebank_feature::flux_mean_WS_ratio: return "mean_WS_ratio";
	case treebank_feature::flux_min_WS_ratio: return "min_WS_ratio";
	case treebank_feature::flux_max_size: return "max_size";
	case treebank_feature::flux_mean_size: return "mean_size";
	case treebank_feature::flux_min_size: return "min_size";
	case treebank_feature::tree_type: return "tree_type";
	case treebank_feature::syntactic_dependency_structure_type:
		return "syntactic_dependency_structure_type";
	case treebank_feature::__last_value: return "__last_value";
	}
	// should never happen
	return "???";
}

} // -- namespace internal
} // -- namespace lal
