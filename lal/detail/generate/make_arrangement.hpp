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

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace detail {

// -- for rooted trees

/**
 * @brief Make an arrangement using permutations.
 * @tparam container Object containing the permutations.
 * @param T Input rooted tree.
 * @param r Root of the current subtree.
 * @param data The permutations used to construct the arrangement.
 * @param[out] pos Current position in the arrangement.
 * @param[out] arr Arrangement constructed.
 */
template <class container>
void make_arrangement_permutations(
	const graphs::rooted_tree& T,
	const node r,
	const container& data,
	position& pos,
	linear_arrangement& arr
) noexcept
{
	// number of children of 'r' with respect to the tree's root
	const uint64_t d_out = T.get_out_degree(r);

	// vertex 'r' is a leaf
	if (d_out == 0) {
		arr.assign(r, pos++);
		return;
	}
	const auto& interval = data[r];
	for (std::size_t i = 0; i < interval.size(); ++i) {
		const node vi = interval[i];
		if (vi == r) {
			arr.assign(vi, pos++);
		}
		else {
			make_arrangement_permutations(T, vi, data, pos, arr);
		}
	}
}

/**
 * @brief Make an arrangement using permutations.
 * @tparam container Object containing the permutations.
 * @param T Input rooted tree.
 * @param data The permutations to construct the arrangement from.
 * @returns The arrangement constructed with the permutations.
 */
template <class container>
[[nodiscard]] linear_arrangement make_arrangement_permutations(
	const graphs::rooted_tree& T, const container& data
) noexcept
{
	linear_arrangement arr(T.get_num_nodes());
	position pos = 0;
	make_arrangement_permutations(T, T.get_root(), data, pos, arr);
	return arr;
}

// -----------------------------------------------------------------------------
// -- for free trees

/**
 * @brief Make an arrangement using permutations.
 * @tparam container Object containing the permutations.
 * @param T Input rooted tree.
 * @param parent Parent of node @e u.
 * @param u Root of the current subtree.
 * @param data The permutations used to construct the arrangement.
 * @param[out] pos Current position in the arrangement.
 * @param[out] arr Arrangement constructed.
 */
template <class container>
void make_arrangement_permutations(
	const graphs::free_tree& T,
	node parent,
	node u,
	const container& data,
	position& pos,
	linear_arrangement& arr
) noexcept
{
	// number of children of 'u' with respect to the tree's root
	const uint64_t d_out = T.get_degree(u) - (u == parent ? 0 : 1);

	// vertex 'u' is a leaf in the rooted version of T
	if (d_out == 0) {
		arr.assign(u, pos++);
		return;
	}
	const auto& interval = data[u];
	for (std::size_t i = 0; i < interval.size(); ++i) {
		const node vi = interval[i];
		if (vi == u) {
			arr.assign(vi, pos++);
		}
		else {
			make_arrangement_permutations(T, u, vi, data, pos, arr);
		}
	}
}

/**
 * @brief Make an arrangement using permutations.
 * @tparam container Object containing the permutations.
 * @param T Input rooted tree.
 * @param root Node used as root.
 * @param data The permutations to construct the arrangement from.
 * @returns The arrangement constructed with the permutations.
 */
template <class container>
[[nodiscard]] linear_arrangement make_arrangement_permutations(
	const graphs::free_tree& T, node root, const container& data
) noexcept
{
	linear_arrangement arr(T.get_num_nodes());
	position pos = 0;
	make_arrangement_permutations(T, root, root, data, pos, arr);
	return arr;
}

} // namespace detail
} // namespace lal
