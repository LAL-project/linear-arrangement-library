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
#include <cstdint>
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
 * Classes @ref lal::io::treebank_dataset_processor and
 * @ref lal::io::treebank_processor
 * are designed to process treebanks in an automatic fashion, meaning that they
 * process the trees and calculate a fixed set of features, the results of which
 * are stored in files. In this enumeration users will find a complete list of
 * all the features that can be calculated using those two classes.
 */
enum class treebank_feature {
	/// Number of nodes of the tree.
	num_nodes = 0,
	/**
	 * @brief Second moment of degree \f$\langle k^2 \rangle\f$.
	 *
	 * See @ref lal::properties::mmt_degree for details.
	 */
	k2,
	/**
	 * @brief Third moment of degree \f$\langle k^3 \rangle\f$.
	 *
	 * See @ref lal::properties::mmt_degree for details.
	 */
	k3,
	/**
	 * @brief Size of the set \f$Q(T)\f$ of this tree \f$T\f$.
	 *
	 * See @ref lal::properties::size_Q for details.
	 */
	num_pairs_independent_edges,
	/**
	 * @brief Headedness of the tree.
	 *
	 * See @ref lal::linarr::headedness for details.
	 */
	headedness,
	/**
	 * @brief Mean hierarchical distance of the tree.
	 *
	 * See @ref lal::properties::mean_hierarchical_distance for details.
	 */
	mean_hierarchical_distance,
	/**
	 * @brief Mean dependency distance of the tree.
	 *
	 * See @ref lal::linarr::mean_dependency_distance for details.
	 */
	mean_dependency_distance,
	/**
	 * @brief Number of edge crossings \f$C\f$.
	 *
	 * See @ref lal::linarr::algorithms_C for details.
	 */

	// C

	C,
	/**
	 * @brief First moment of expectation of \f$C\f$, \f$E[C]\f$.
	 *
	 * See @ref lal::properties::expectation_C for details.
	 */
	C_exp_1,
	/**
	 * @brief Second moment of expectation of \f$C\f$, \f$E[C^2]\f$.
	 *
	 * This is calculated as \f$E[C^2]=V[C] + E[C]^2\f$. See
	 * @ref lal::properties::variance_C_tree for details on how the variance
	 * of \f$C\f$, \f$V[C]\f$, is calculated.
	 */
	C_exp_2,
	/**
	 * @brief Variance of \f$C\f$, \f$V[C]\f$.
	 *
	 * @ref lal::properties::variance_C_tree for details.
	 */
	C_var,
	/**
	 * @brief z-score of \f$C\f$, \f$\frac{C - E[C]}{\sqrt{V[C]}}\f$.
	 *
	 * See @ref lal::properties::variance_C_tree for details on how the
	 * variance of \f$C\f$, \f$V[C]\f$, is calculated.
	 */
	C_z,
	/**
	 * @brief Sum of length of edges \f$D\f$.
	 *
	 * See @ref lal::linarr::sum_length_edges for details.
	 */

	// D

	D,
	/**
	 * @brief First moment of expectation of \f$D\f$, \f$E[D]\f$.
	 *
	 * See @ref lal::properties::expectation_D for details.
	 */
	D_exp_1,
	/**
	 * @brief Second moment of expectation of \f$D\f$, \f$E[D^2]\f$.
	 *
	 * This is calculated as \f$E[D^2]=V[D] + E[D]^2\f$. See
	 * @ref lal::properties::variance_D for details on how the variance
	 * of \f$D\f$, \f$V[D]\f$, is calculated.
	 */
	D_exp_2,
	/**
	 * @brief Variance of \f$D\f$, \f$V[D]\f$.
	 *
	 * @ref lal::properties::variance_D for details.
	 */
	D_var,
	/**
	 * @brief z-score of \f$D\f$, \f$\frac{D - E[D]}{\sqrt{V[D]}}\f$.
	 *
	 * See @ref lal::properties::variance_D for details on how the
	 * variance of \f$D\f$, \f$V[D]\f$, is calculated.
	 */
	D_z,
	/**
	 * @brief Unconstrained minimum sum of length of edges.
	 *
	 * See @ref lal::linarr::algorithms_Dmin::Unconstrained_YS, or
	 * @ref lal::linarr::algorithms_Dmin::Unconstrained_FC for details.
	 */
	Dmin_Unconstrained,
	/**
	 * @brief Minimum sum of length of edges under the planary constraint.
	 *
	 * See @ref lal::linarr::algorithms_Dmin::Planar for details.
	 */
	Dmin_Planar,
	/**
	 * @brief Minimum sum of length of edges under the planary constraint.
	 *
	 * See @ref lal::linarr::algorithms_Dmin::Projective for details.
	 */
	Dmin_Projective,

	// FLUXES

	/**
	 * @brief Maximum flux weight.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	max_flux_weight,
	/**
	 * @brief Mean flux weight.
	 *
	 * This is the sum of weights averaged by the number of fluxes (the number
	 * of vertices of the tree minus 1). See @ref lal::linarr::dependency_flux
	 * for details on the definition of weight.
	 */
	mean_flux_weight,
	/**
	 * @brief Minimum flux weight.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	min_flux_weight,

	/**
	 * @brief Maximum left span.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	max_left_span,
	/**
	 * @brief Mean left span.
	 *
	 * This is the sum of left spans averaged by the number of fluxes (the
	 * number of vertices of the tree minus 1). See @ref lal::linarr::dependency_flux
	 * for details on the definition of left span.
	 */
	mean_left_span,
	/**
	 * @brief Minimum left span.
	 *
	 * See @ref linarr::dependency_flux for details.
	 */
	min_left_span,

	/**
	 * @brief Maximum right span.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	max_right_span,
	/**
	 * @brief Mean right span.
	 *
	 * This is the sum of right spans averaged by the number of fluxes (the
	 * number of vertices of the tree minus 1). See @ref lal::linarr::dependency_flux
	 * for details on the definition of right span.
	 */
	mean_right_span,
	/**
	 * @brief Minimum right span.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	min_right_span,

	/**
	 * @brief Maximum flux size.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	max_size,
	/**
	 * @brief Mean flux size.
	 *
	 * This is the sum of flux sizes averaged by the number of fluxes (the
	 * number of vertices of the tree minus 1). See @ref lal::linarr::dependency_flux
	 * for details on the definition of flux size.
	 */
	mean_size,
	/**
	 * @brief Minimum flux size.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	min_size,

	/**
	 * @brief Maximum R/L ratio.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	max_RL_ratio,
	/**
	 * @brief Mean R/L ratio.
	 *
	 * This is the sum of R/L ratios averaged by the number of fluxes (the
	 * number of vertices of the tree minus 1). See @ref lal::linarr::dependency_flux
	 * for details on the definition of R/L ratio.
	 */
	mean_RL_ratio,
	/**
	 * @brief Minimum R/L ratio.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	min_RL_ratio,

	/**
	 * @brief Maximum W/S ratio.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	max_WS_ratio,
	/**
	 * @brief Mean W/S ratio.
	 *
	 * This is the sum of W/S ratios averaged by the number of fluxes (the
	 * number of vertices of the tree minus 1). See @ref lal::linarr::dependency_flux
	 * for details on the definition of W/S ratio.
	 */
	mean_WS_ratio,
	/**
	 * @brief Minimum W/S ratio.
	 *
	 * See @ref lal::linarr::dependency_flux for details.
	 */
	min_WS_ratio,


	/**
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
static constexpr std::size_t __treebank_feature_size =
	static_cast<std::size_t>(treebank_feature::__last_value);

/**
 * @brief Converts a @ref lal::io::treebank_feature value into a string.
 * @param tf A treebank feature.
 * @returns A string used to format output files.
 */
inline
#ifdef SWIG
std::string
#else
constexpr std::string_view
#endif
treebank_feature_to_string(const treebank_feature& tf) {
	switch (tf) {
	case treebank_feature::num_nodes: return "n";
	case treebank_feature::k2: return "k2";
	case treebank_feature::k3: return "k3";
	case treebank_feature::num_pairs_independent_edges: return "size_Q";
	case treebank_feature::headedness: return "headedness";
	case treebank_feature::mean_hierarchical_distance: return "mean_hierarchical_distance";
	case treebank_feature::mean_dependency_distance: return "mean_dependency_distance";
	case treebank_feature::C: return "C";
	case treebank_feature::C_exp_1: return "C_exp_1";
	case treebank_feature::C_exp_2: return "C_exp_2";
	case treebank_feature::C_var: return "C_var";
	case treebank_feature::C_z: return "C_z";
	case treebank_feature::D: return "D";
	case treebank_feature::D_exp_1: return "D_exp_1";
	case treebank_feature::D_exp_2: return "D_exp_2";
	case treebank_feature::D_var: return "D_var";
	case treebank_feature::D_z: return "D_z";
	case treebank_feature::Dmin_Unconstrained: return "Dmin_Unconstrained";
	case treebank_feature::Dmin_Planar: return "Dmin_Planar";
	case treebank_feature::Dmin_Projective: return "Dmin_Projective";
	case treebank_feature::max_flux_weight: return "max_flux_weight";
	case treebank_feature::mean_flux_weight: return "mean_flux_weight";
	case treebank_feature::min_flux_weight: return "min_flux_weight";
	case treebank_feature::max_left_span: return "max_left_span";
	case treebank_feature::mean_left_span: return "mean_left_span";
	case treebank_feature::min_left_span: return "min_left_span";
	case treebank_feature::max_right_span: return "max_right_span";
	case treebank_feature::mean_right_span: return "mean_right_span";
	case treebank_feature::min_right_span: return "min_right_span";
	case treebank_feature::max_RL_ratio: return "max_RL_ratio";
	case treebank_feature::mean_RL_ratio: return "mean_RL_ratio";
	case treebank_feature::min_RL_ratio: return "min_RL_ratio";
	case treebank_feature::max_WS_ratio: return "max_WS_ratio";
	case treebank_feature::mean_WS_ratio: return "mean_WS_ratio";
	case treebank_feature::min_WS_ratio: return "min_WS_ratio";
	case treebank_feature::max_size: return "max_size";
	case treebank_feature::mean_size: return "mean_size";
	case treebank_feature::min_size: return "min_size";
	case treebank_feature::__last_value: return "__last_value";
	}
	// should never happen
	return "???";
}

} // -- namespace io
} // -- namespace lal