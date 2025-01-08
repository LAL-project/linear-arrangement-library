/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <array>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/syntactic_dependency_tree/type.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Computes the type of syntactic dependency tree.
 *
 * Given an undirected rooted tree and a linear arrangement of its nodes,
 * computes the class of projective structure the tree belongs to.
 *
 * This function admits the precomputed number of edge crossings in the same
 * linear arrangement passed as parameter to this function.
 *
 * @param t Input tree.
 * @param C Number of crossings (see @ref lal::linarr::num_crossings).
 * @param pi Linear arrangement of the nodes. If \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @returns The class of projective structure. If the class could not
 * be determined the method sets the corresponding position for
 * @ref lal::linarr::syntactic_dependency_tree_type::unknown.
 */
[[nodiscard]] std::array<bool, __syntactic_dependency_tree_size>
syntactic_dependency_tree_classify
(
	const graphs::rooted_tree& t,
	const uint64_t C,
	const linear_arrangement& pi = {}
)
noexcept;

/**
 * @brief Computes the type of syntactic dependency tree.
 *
 * Given an undirected rooted tree and a linear arrangement of its nodes,
 * computes the class of projective structure the tree belongs to.
 *
 * @param t Input tree.
 * @param pi Linear arrangement of the nodes. If \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @returns The class of projective structure. If the class could not
 * be determined the method sets the corresponding position for
 * @ref lal::linarr::syntactic_dependency_tree_type::unknown.
 */
[[nodiscard]] std::array<bool, __syntactic_dependency_tree_size>
syntactic_dependency_tree_classify
(const graphs::rooted_tree& t, const linear_arrangement& pi = {})
noexcept;

} // -- namespace linarr
} // -- namespace lal
