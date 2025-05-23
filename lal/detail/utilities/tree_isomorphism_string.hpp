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
 ********************************************************************/

#pragma once

// C++ includes
#include <algorithm>

// lal includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace detail {
namespace isomorphism {

/**
 * @brief Assigns a name to node 'u', root of the current subtree.
 *
 * For further details on the algorithm, see \cite Aho1974a for further details.
 * @param t Input rooted tree.
 * @param u Root of the subtree whose name we want to calculate.
 * @param names An array of strings where the names are stored (as in a dynamic
 * programming algorithm). The size of this array must be at least the number of
 * vertices in the subtree of 't' rooted at 'u'. Actually, less memory suffices,
 * but I don't know how much less: better be safe than sorry.
 * @param idx A pointer to the position within @e names that will contain the
 * name of the first child of 'u'. The position @e names[idx+1] will contain the
 * name of the second child of 'u'.
 * @returns The code for the subtree rooted at 'u'.
 */
[[nodiscard]] inline std::string assign_name(
	const graphs::rooted_tree& t,
	const node u,
	array<std::string>& names,
	std::size_t idx
) noexcept
{
	if (t.get_out_degree(u) == 0) {
		return std::string("10");
	}

	// make childrens' names
	const std::size_t begin_idx = idx;
	for (const node v : t.get_out_neighbors(u)) {
		// make the name for v
		names[idx] = assign_name(t, v, names, idx + 1);
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

/**
 * @brief Assigns a name to node 'u', root of the current subtree.
 *
 * For further details on the algorithm, see \cite Aho1974a for further details.
 * @param t Input rooted tree.
 * @param p Parent of the root of the subtree whose name we want to calculate.
 * @param u Root of the subtree whose name we want to calculate.
 * @param names An array of strings where the names are stored (as in a dynamic
 * programming algorithm). The size of this array must be at least the number of
 * vertices in the subtree of 't' rooted at 'u'. Actually, less memory suffices,
 * but I don't know how much less: better be safe than sorry.
 * @param idx A pointer to the position within @e names that will contain the
 * name of the first child of 'u'. The position @e names[idx+1] will contain the
 * name of the second child of 'u'.
 * @returns The code for the subtree rooted at 'u'.
 */
[[nodiscard]] inline std::string assign_name(
	const graphs::free_tree& t,
	const node p,
	const node u,
	array<std::string>& names,
	std::size_t idx
) noexcept
{
	if (t.get_degree(u) == 1) {
		return std::string("10");
	}

	// make childrens' names
	const std::size_t begin_idx = idx;
	for (const node v : t.get_neighbors(u)) {
		if (v == p) {
			continue;
		}

		// make the name for v
		names[idx] = assign_name(t, u, v, names, idx + 1);
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

/**
 * @brief Assigns a name to node 'u', root of the current subtree.
 *
 * For further details on the algorithm, see \cite Aho1974a for further details.
 * @param t Input rooted tree, as a @ref lal::graphs::free_tree object.
 * @param r Root of the input tree.
 * @param names An array of strings where the names are stored (as in a dynamic
 * programming algorithm). The size of this array must be at least the number of
 * vertices in the subtree of 't' rooted at 'u'. Actually, less memory suffices,
 * but I don't know how much less: better be safe than sorry.
 * @param idx A pointer to the position within @e names that will contain the
 * name of the first child of 'u'. The position @e names[idx+1] will contain the
 * name of the second child of 'u'.
 * @returns The code for the subtree rooted at 'u'.
 */
[[nodiscard]] inline std::string assign_name(
	const graphs::free_tree& t,
	const node r,
	array<std::string>& names,
	std::size_t idx
) noexcept
{
	if (t.get_degree(r) == 0) {
		return std::string("10");
	}

	// make childrens' names
	const std::size_t begin_idx = idx;
	for (const node v : t.get_neighbors(r)) {
		// make the name for v
		names[idx] = assign_name(t, r, v, names, idx + 1);
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

} // namespace isomorphism

/**
 * @brief Test whether two rooted trees are isomorphic or not.
 * @param t1 First tree.
 * @param r1 Root of the first tree.
 * @param t2 Second tree.
 * @param r2 Root of the second tree.
 * @returns Whether the two rooted trees are isomorphic or not.
 */
[[nodiscard]] inline bool are_rooted_trees_isomorphic_string(
	const graphs::free_tree& t1,
	const node r1,
	const graphs::free_tree& t2,
	const node r2
) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Aho1974a);
#endif

	array<std::string> names(t1.get_num_nodes());
	const std::string name_r1 = isomorphism::assign_name(t1, r1, names, 0);
	const std::string name_r2 = isomorphism::assign_name(t2, r2, names, 0);
	return name_r1 == name_r2;
}

/**
 * @brief Test whether two rooted trees are isomorphic or not.
 * @param t1 First tree.
 * @param t2 Second tree.
 * @returns Whether the two rooted trees are isomorphic or not.
 */
[[nodiscard]] inline bool are_rooted_trees_isomorphic_string(
	const graphs::rooted_tree& t1, const graphs::rooted_tree& t2
) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Aho1974a);
#endif

	array<std::string> names(t1.get_num_nodes());
	const std::string name_r1 =
		isomorphism::assign_name(t1, t1.get_root(), names, 0);
	const std::string name_r2 =
		isomorphism::assign_name(t2, t2.get_root(), names, 0);
	return name_r1 == name_r2;
}

} // namespace detail
} // namespace lal
