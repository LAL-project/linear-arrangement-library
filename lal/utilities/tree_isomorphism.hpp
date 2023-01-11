/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2023
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace utilities {

/**
 * @brief Isomorphism test for unlabelled rooted trees.
 *
 * See @ref LAL_concepts__tree_isomorphism for a short definition of what tree
 * isomorphism is.
 *
 * The algorithm implemented in this function can be found in \cite Aho1974a.
 * @param t1 Input rooted tree.
 * @param t2 Input rooted tree.
 * @returns Whether or not the input trees are isomorphic or not.
 * @pre Both input trees are valid rooted trees (see
 * @ref lal::graphs::rooted_tree::is_rooted_tree).
 */
bool are_trees_isomorphic
(const graphs::rooted_tree& t1, const graphs::rooted_tree& t2) noexcept;

/**
 * @brief Isomorphism test for unlabelled free trees.
 *
 * See @ref LAL_concepts__tree_isomorphism for a short definition of what tree
 * isomorphism is.
 *
 * The algorithm implemented in this function can be found in \cite Aho1974a.
 * @param t1 Input free tree.
 * @param t2 Input free tree.
 * @returns Whether or not the input trees are isomorphic.
 * @pre Both input trees are valid free trees (see
 * @ref lal::graphs::free_tree::is_tree).
 */
bool are_trees_isomorphic
(const graphs::free_tree& t1, const graphs::free_tree& t2) noexcept;

} // -- namespace utilities
} // -- namespace lal
