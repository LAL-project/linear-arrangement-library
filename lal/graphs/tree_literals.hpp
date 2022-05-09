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

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

/**
 * @brief String literal to easily describe free trees.
 *
 * This is meant to be used only on strings that represent:
 * - an edge list, or
 * - a head vector.
 * See the code example for the format of both.
 *
 * Numbers in valid strings are always in base 10. The number of spaces that
 * separate characters can be arbitrarily long or short.
 *
 @code
 const std::pair<lal::graphs::free_tree, lal::node> tree_and_root =
	"0 1 1 1 1 1"_root_free_tree;
 const std::pair<lal::graphs::free_tree, lal::node> tree_and_root =
	"{0 1} {0 2} {0 3} {0 4} {0 5} {0 6}"_root_free_tree;
 @endcode
 *
 * @param str A pointer to a C-string encoding the head vector or the edge list.
 * @param s Size of @e str.
 * @returns Returns a std::pair<@ref lal::graphs::free_tree,@ref lal::node>.
 * In case the input string is a head vector the node is the root of the head
 * vector. In case the input is an edge list, the node is invalid, i.e., a value
 * larger than the number of vertices.
 * @post The tree is normalized.
 */
[[nodiscard]] std::pair<lal::graphs::free_tree, lal::node>
operator""_root_free_tree (const char *str, std::size_t s)
noexcept;

/**
 * @brief User-defined literal to denote free trees.
 *
 * This is meant to be used only on strings that represent:
 * - an edge list, or
 * - a head vector.
 * See the code example for the format of both.
 *
 * Numbers in valid strings are always in base 10. The number of spaces that
 * separate characters can be arbitrarily long or short.
 *
 @code
 const lal::graphs::free_tree tree = "0 1 1 1 1 1"_free_tree;
 const lal::graphs::free_tree tree =
	"{0 1} {0 2} {0 3} {0 4} {0 5} {0 6}"_free_tree;
 @endcode
 *
 * @param str A pointer to a C-string encoding the head vector or the edge list.
 * @param s Size of @e str.
 * @returns Returns a @ref lal::graphs::free_tree.
 * @post The tree is normalized.
 */
[[nodiscard]] lal::graphs::free_tree
operator""_free_tree (const char *str, std::size_t s)
noexcept;

/**
 * @brief User-defined literal to denote free trees.
 *
 * This is meant to be used only on strings that represent:
 * - an edge list, or
 * - a head vector.
 * See the code example for the format of both.
 *
 * Numbers in valid strings are always in base 10. The number of spaces that
 * separate characters can be arbitrarily long or short.
 *
 @code
 const lal::graphs::rooted_tree tree = "0 1 1 1 1 1"_rooted_tree;
 const lal::graphs::rooted_tree tree =
	"{0 1} {0 2} {0 3} {0 4} {0 5} {0 6}"_rooted_tree;
 @endcode
 * @param str A pointer to a C-string encoding the head vector or the edge list.
 * @param s Size of @e str.
 * @returns Returns a @ref lal::graphs::free_tree.
 * @post The tree is normalized.
 */
[[nodiscard]] lal::graphs::rooted_tree
operator""_rooted_tree (const char *str, std::size_t s)
noexcept;
