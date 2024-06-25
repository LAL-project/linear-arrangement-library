/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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
#include <algorithm>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace detail {

/**
 * @brief Fast tree non-isomorphism test.
 *
 * @tparam tree_t Tree type.
 * @param t1 One tree.
 * @param t2 Another tree.
 * @returns Whether the input trees are, might be, or are not isomorphic.
 * @returns 0 if the trees ARE isomorphic
 * @returns 1 if the trees ARE NOT isomorphic:
 * - number of vertices do not coincide,
 * - number of leaves do not coincide,
 * - second moment of degree do not coincide,
 * - maximum vertex degrees do not coincide,
 * @returns 2 if the trees MIGHT BE isomorphic
 */
template <
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool> = true
>
char fast_non_iso(const tree_t& t1, const tree_t& t2) noexcept
{
	// check number of nodes
	if (t1.get_num_nodes() != t2.get_num_nodes()) { return 1; }

	const uint64_t n = t1.get_num_nodes();

	// trees ARE isomorphic
	if (n <= 2) { return 0; }

	uint64_t nL_t1 = 0; // number of leaves of t1
	uint64_t nL_t2 = 0; // number of leaves of t2
	uint64_t k2_t1 = 0; // sum of squared degrees of t1
	uint64_t k2_t2 = 0; // sum of squared degrees of t2
	uint64_t maxdeg_t1 = 0; // max degree of t1
	uint64_t maxdeg_t2 = 0; // max degree of t2
	for (node u = 0; u < n; ++u) {
		const uint64_t ku1 = to_uint64(t1.get_degree(u));
		const uint64_t ku2 = to_uint64(t2.get_degree(u));

		nL_t1 += t1.get_degree(u) == 1;
		nL_t2 += t2.get_degree(u) == 1;
		k2_t1 += ku1*ku1;
		k2_t2 += ku2*ku2;
		maxdeg_t1 = (maxdeg_t1 < ku1 ? ku1 : maxdeg_t1);
		maxdeg_t2 = (maxdeg_t2 < ku2 ? ku2 : maxdeg_t2);
	}

	// check number of leaves
	if (nL_t1 != nL_t2) { return 1; }
	// check maximum degree
	if (maxdeg_t1 != maxdeg_t2) { return 1; }
	// check sum of squared degrees
	if (k2_t1 != k2_t2) { return 1; }

	// trees MIGHT BE isomorphic
	return 2;
}

/**
 * @brief Assigns a name to node 'u', root of the current subtree.
 *
 * This function stores the names of every node in the subtree rooted at 'u'.
 * This is useful if we want to make lots of comparisons between subtrees
 *
 * For further details on the algorithm, see \cite Aho1974a.
 * @param t Input rooted tree
 * @param u Root of the subtree whose name we want to calculate
 * @param idx A pointer to the position within @e names that will contain the
 * name of the first child of 'u'. The position @e names[idx+1] will contain the
 * name of the second child of 'u'.
 * @param aux_memory_for_names Auxiliary memory used to sort names of subtrees.
 * @param keep_name_of An array of strings where the names are stored
 * (as in a dynamic programming algorithm). The size of this array must be at
 * least the number of vertices in the subtree of 't' rooted at 'u'. Actually,
 * less memory suffices, but I don't know how much less: better be safe than
 * sorry.
 */
inline void assign_name_and_keep(
	const graphs::rooted_tree& t,
	const node u,
	std::size_t idx,
	std::string * const aux_memory_for_names,
	std::string * const keep_name_of
)
noexcept
{
	if (t.get_out_degree(u) == 0) {
		keep_name_of[u] = "10";
		return;
	}

	// make childrens' names
	const std::size_t begin_idx = idx;
	for (node v : t.get_out_neighbors(u)) {
		// make the name for v
		assign_name_and_keep(t,v, idx+1, aux_memory_for_names, keep_name_of);

		aux_memory_for_names[idx] = keep_name_of[v];
		++idx;
	}
	std::sort(&aux_memory_for_names[begin_idx], &aux_memory_for_names[idx]);

	// join the names in a single string to make the name of vertex 'v'
	std::string name = "1";
	for (std::size_t j = begin_idx; j < idx; ++j) {
		name += aux_memory_for_names[j];
	}
	name += "0";
	keep_name_of[u] = name;
}

/**
 * @brief Assigns a name to node 'u', root of the current subtree.
 *
 * For further details on the algorithm, see \cite Aho1974a for further details.
 * @param t Input rooted tree
 * @param u Root of the subtree whose name we want to calculate
 * @param names An array of strings where the names are stored (as in a dynamic
 * programming algorithm). The size of this array must be at least the number of
 * vertices in the subtree of 't' rooted at 'u'. Actually, less memory suffices,
 * but I don't know how much less: better be safe than sorry.
 * @param idx A pointer to the position within @e names that will contain the
 * name of the first child of 'u'. The position @e names[idx+1] will contain the
 * name of the second child of 'u'.
 * @returns The code for the subtree rooted at 'u'.
 */
inline std::string assign_name
(const graphs::rooted_tree& t, node u, std::string * const names, std::size_t idx)
noexcept
{
	if (t.get_out_degree(u) == 0) {
		return std::string("10");
	}

	// make childrens' names
	const std::size_t begin_idx = idx;
	for (node v : t.get_out_neighbors(u)) {
		// make the name for v
		names[idx] = assign_name(t,v, names, idx+1);
		++idx;
	}
	std::sort(&names[begin_idx], &names[idx]);

	// join the names in a single string to make the name of vertex 'v'
	std::string name = "1";
	for (std::size_t j = begin_idx; j < idx; ++j) {
		name += names[j];
	}
	name += "0";

	return name;
}

/// Test whether two rooted trees are isomorphic or not.
inline bool are_rooted_trees_isomorphic
(const graphs::rooted_tree& t1, const graphs::rooted_tree& t2)
noexcept
{
	const auto discard = detail::fast_non_iso(t1,t2);
	if (discard == 0) { return true; }
	if (discard == 1) { return false; }

	array<std::string> names(t1.get_num_nodes());
	const std::string name_r1 = assign_name(t1, t1.get_root(), names.begin(), 0);
	const std::string name_r2 = assign_name(t2, t2.get_root(), names.begin(), 0);
	return name_r1 == name_r2;
}

} // -- namespace detail
} // -- namespace lal
