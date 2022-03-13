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
#include <sstream>

// lal includes
#include <lal/graphs/conversions.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

/**
 * @brief User-defined literal to denote free trees.
 *
 * This is meant to be used only on strings that represent a head vector.
 * Such a string is valid when it contains only base 10 decimal values all
 * separated by spaces. The number of spaces that separate two decimal values
 * can be arbitrarily long.
 *
 @code
 const std::pair<lal::graphs::free_tree, lal::node> tree_and_root = "0 1 1 1 1 1"_pfree_tree;
 @endcode
 *
 * @param hv_str A pointer to a C-string encoding the head vector.
 * @param s Size of @e hv_str.
 * @returns Returns the result of @ref lal::graphs::from_head_vector_to_free_tree
 * when passed to it the head vector encoded in @e hv_str as parameter.
 */
[[nodiscard]] std::pair<lal::graphs::free_tree, lal::node>
operator""_pfree_tree (const char *hv_str, [[maybe_unused]] std::size_t s)
noexcept
{
	std::vector<uint64_t> hv;
	// parse the elements in the string
	{
	std::stringstream ss(hv_str);
	uint64_t v;
	while (ss >> v) { hv.push_back(v); }
	}
	return lal::graphs::from_head_vector_to_free_tree(hv);
}

/**
 * @brief User-defined literal to denote free trees.
 *
 * This is meant to be used only on strings that represent a head vector.
 * Such a string is valid when it contains only base 10 decimal values all
 * separated by spaces. The number of spaces that separate two decimal values
 * can be arbitrarily long.
 *
 @code
 const lal::graphs::free_tree tree = "0 1 1 1 1 1"_free_tree;
 @endcode
 *
 * @param hv_str A pointer to a C-string encoding the head vector.
 * @param s Size of @e hv_str.
 * @returns Returns the first element in the result of function
 * @ref lal::graphs::from_head_vector_to_free_tree when passed to it the head
 * vector encoded in @e hv_str as parameter.
 */
[[nodiscard]] lal::graphs::free_tree
operator""_free_tree (const char *hv_str, [[maybe_unused]] std::size_t s)
noexcept
{
	std::vector<uint64_t> hv;
	// parse the elements in the string
	{
	std::stringstream ss(hv_str);
	uint64_t v;
	while (ss >> v) { hv.push_back(v); }
	}
	return std::move(lal::graphs::from_head_vector_to_free_tree(hv).first);
}

/**
 * @brief User-defined literal to denote free trees.
 *
 * This is meant to be used only on strings that represent a head vector.
 * Such a string is valid when it contains only base 10 decimal values all
 * separated by spaces. The number of spaces that separate two decimal values
 * can be arbitrarily long.
 *
 @code
 const lal::graphs::rooted_tree tree = "0 1 1 1 1 1"_rooted_tree;
 @endcode
 *
 * @param hv_str A pointer to a C-string encoding the head vector.
 * @param s Size of @e hv_str.
 * @returns Returns the result of @ref lal::graphs::from_head_vector_to_free_tree
 * when passed as parameter the head vector encoded in @e hv_str.
 */
[[nodiscard]] lal::graphs::rooted_tree
operator""_rooted_tree (const char *hv_str, [[maybe_unused]] std::size_t s)
noexcept
{
	std::vector<uint64_t> hv;
	// parse the elements in the string
	{
	std::stringstream ss(hv_str);
	uint64_t v;
	while (ss >> v) { hv.push_back(v); }
	}
	return lal::graphs::from_head_vector_to_rooted_tree(hv);
}

void parse_tree_udls() noexcept {
	const std::pair<lal::graphs::free_tree, lal::node> tree_and_root = "0 1 1 1 1 "_pfree_tree;
	const lal::graphs::free_tree ftree = "0 1 1 1 1 "_free_tree;
	const lal::graphs::rooted_tree rtree = "0 1 1 1 1 "_rooted_tree;
}
