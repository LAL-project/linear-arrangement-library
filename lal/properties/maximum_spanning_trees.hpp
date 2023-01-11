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
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>

namespace lal {
namespace properties {

/**
 * @brief Caterpillar distance of a tree.
 *
 * The caterpillar distance of a given tree is defined as the minimum amount
 * of vertices to be removed from the tree so that the result is a caterpillar
 * tree. See page @ref LAL_concepts__tree_types for a definition of caterpillar
 * tree.
 * @param t Input free tree.
 * @returns A positive integer value, the caterpillar distance of the input tree.
 */
uint64_t tree_caterpillar_distance(const graphs::free_tree& t) noexcept;

/**
 * @brief Caterpillar distance of a tree.
 *
 * The caterpillar distance of a given tree is defined as the minimum amount
 * of vertices to be removed from the tree so that the result is a caterpillar
 * tree. See page @ref LAL_concepts__tree_types for a definition of caterpillar
 * tree.
 * @param t Input free tree.
 * @returns A positive integer value, the caterpillar distance of the input tree.
 */
uint64_t tree_caterpillar_distance(const graphs::rooted_tree& t) noexcept;

} // -- namespace properties
} // -- namespace lal
