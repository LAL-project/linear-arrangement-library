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
 *     Juan Luis Esteban (esteban@cs.upc.edu)
 *         LOGPROG: Logics and Programming Research Group
 *         Office 110, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://www.cs.upc.edu/~esteban/
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
#if defined DEBUG
#include <optional>
#include <cassert>
#endif
#include <vector>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/array.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/detail/graphs/size_subtrees.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/properties/tree_centroid.hpp>
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/linarr/D/Dopt_utils.hpp>

namespace lal {
namespace detail {

/// Utilities for the various minimum linear arrangement algorithms.
namespace Dmin_utils {

/* ************************************************************************** */
/* ---------------------- INTERVAL-based methods ---------------------------- */

/* The following namespace contains functions for the interval-based algorithms
 * to calculate the planar and projective minimum sum of edge lengths.
 */

/**
 * @brief Make a minimum projective arrangement using the sorted, rooted
 * adjacency list @e L.
 *
 * The full details of this algorithm can be found in \cite Alemany2022a.
 *
 * @tparam make_arrangement Whether or not the arrangement is to be constructed.
 * @param L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @param r The vertex root of the subtree whose interval is to be made
 * @param r_place Where, respect to its parent, node 'r' has been placed in the
 * arrangement. Possible values: PLACE_LEFT_OF, PLACE_RIGHT_OF, PLACE_NONE_OF.
 * The latter value is only valid for the root of the whole tree.
 * @param ini Left limit of the positions of the arrangement in which the tree
 * has to be arranged. Note that the limits are included [@e ini,@e fin].
 * @param fin Right limit of the positions of the arrangement in which
 * the tree has to be arranged. Note that the limits are included [@e ini,@e fin].
 * @param[out] arr the arrangement of the tree
 *
 * @returns The sum of the length of the outgoing edges from vertex 'r' plus
 * the length of the anchor of the edge from 'r' to its parent. Such length
 * is defined as the number of vertices to the left of 'r' if 'r_place'
 * is RIGHT_PLACE, or as the number of vertices to the right of 'r' if
 * 'r_place' is LEFT_PLACE.
 *
 * @pre @e L is sorted decreasingly.
 */
template <bool make_arrangement>
[[nodiscard]] uint64_t arrange(
	const std::vector<std::vector<node_size>>& L,
	const node r,
	const Dopt_utils::place r_place,
	position ini,
	position fin,
	linear_arrangement& arr
) noexcept
{
#if defined DEBUG
	assert(ini <= fin);
#endif

	// the children and the sizes of the subtrees of 'r'
	const auto& children_size_of_r = L[r];

	// -- place the children --

	// work out the starting side of the first-largest subtree
	Dopt_utils::side roots_side = r_place == Dopt_utils::PLACE_RIGHT_OF
									  ? Dopt_utils::RIGHT_SIDE
									  : Dopt_utils::LEFT_SIDE;

	// size of the intervals from the root to the left end
	uint64_t acc_size_left = 0;
	// size of the intervals from the root to the right end
	uint64_t acc_size_right = 0;

	// number of intervals to the left of the root
	uint64_t n_intervals_left = 0;
	// number of intervals to the right of the root
	uint64_t n_intervals_right = 0;

	// sum of the optimal D for every subtree +
	// the length of the edge from 'r' to its parent (if any)
	uint64_t D = 0;
	// total sum of lengths of edges from 'r' to 'vi' without the anchor
	uint64_t d = 0;

#if defined DEBUG
	// store the size of the previous subtree (previous in the lists's sorted)
	std::optional<uint64_t> previous_size;
#endif

	// while placing the children calculate the
	// length of the edge from 'r' to vertex 'vi'
	// LARGEST to SMALLEST
	for (const auto& [vi, ni] : children_size_of_r) {

#if defined DEBUG
		// ensure that the list is sorted non-increasingly
		if (previous_size) {
			assert(*previous_size >= ni);
		}
		previous_size = {ni};
#endif

		// recursive call: make the interval of 'vi'
		D += arrange<make_arrangement>(
			L,
			vi,
			(roots_side == Dopt_utils::LEFT_SIDE ? Dopt_utils::PLACE_LEFT_OF
												 : Dopt_utils::PLACE_RIGHT_OF),
			(make_arrangement
				 ? (roots_side == Dopt_utils::LEFT_SIDE ? ini : fin - ni + 1)
				 : 0),
			(make_arrangement
				 ? (roots_side == Dopt_utils::LEFT_SIDE ? ini + ni - 1 : fin)
				 : 0),
			arr
		);

		// accumulate size of interval
		d += ni * (roots_side == Dopt_utils::LEFT_SIDE ? n_intervals_left
													   : n_intervals_right);
		// add length of edge over root 'r'
		d += 1;

		// number of intervals to the left and right of the root
		n_intervals_left += roots_side;
		n_intervals_right += Dopt_utils::other_side(roots_side);

		// accumulate size of subtree rooted at vi
		acc_size_left += roots_side * ni;
		acc_size_right += Dopt_utils::other_side(roots_side) * ni;

		// update limits of the embedding
		if constexpr (make_arrangement) {
			ini += roots_side * ni;
			fin -= Dopt_utils::other_side(roots_side) * ni;
		}

		// change side
		roots_side = Dopt_utils::other_side(roots_side);
	}

#if defined DEBUG
	if constexpr (make_arrangement) {
		assert(ini == fin);
	}
#endif
	if constexpr (make_arrangement) {
		arr.assign(r, ini);
	}

	// accumulate the length of the edge from 'r' to its parent (if any)
	D +=
		(r_place == Dopt_utils::PLACE_NONE_OF	? 0
		 : r_place == Dopt_utils::PLACE_LEFT_OF ? acc_size_right
												: acc_size_left);

	return D + d;
}

/**
 * @brief Wrapper method for the recursive method @ref arrange.
 *
 * A call to this function is done when the goal is to construct a linear arrangement.
 * @tparam make_arrangement Should the arrangement be constructed?
 * @param n Number of vertices.
 * @param L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @param r Root of the tree represented by @e L.
 * @param[out] arr Minimum projective linear arrangement.
 * @returns The cost of a minimum projective linear arrangement.
 * @pre @e L is sorted decreasingly.
 */
template <bool make_arrangement>
[[nodiscard]] inline uint64_t arrange_projective(
	const uint64_t n,
	const std::vector<std::vector<node_size>>& L,
	const node r,
	linear_arrangement& arr
) noexcept
{
	return arrange<make_arrangement>(
		L, r, Dopt_utils::PLACE_NONE_OF, 0, n - 1, arr
	);
}

/* ************************************************************************** */
/* ----------------- DISPLACEMENT-based methods namespace ------------------- */

/* The following namespace contains functions for the interval-based algorithms
 * to calculate the planar and projective minimum sum of edge lengths.
 */

/**
 * @brief Embed a tree's branch.
 *
 * Implementation of procedure 'embed' as defined by Hochberg and Stallmann in
 * \cite Hochberg2003a and with the correction in \cite Alemany2022a.
 *
 * @tparam make_arrangement Whether or not the arrangement is to be constructed.
 * @param L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @param v The current branch of the tree to be arranged.
 * @param base The displacement for the starting position of the subtree
 * arrangement
 * @param dir Whether or not @e v is to the left or to the right of its parent
 * @param[out] rel_pos the displacement from the root of all nodes of the subtree
 * @returns The cost of a minimum linear arrangement of the corresponding branch.
 * @pre @e L is sorted decreasingly.
 */
template <bool make_arrangement>
[[nodiscard]] uint64_t embed_branch(
	const std::vector<std::vector<node_size>>& L,
	const node v,
	int64_t base,
	const int64_t dir,
	array<int64_t>& rel_pos
) noexcept
{
	const auto& Cv = L[v];
	uint64_t cost_branch = 0;

	uint64_t before = 0;
	uint64_t after = 0;
	uint64_t under_anchor = 0;

	// LARGEST to SMALLEST
	// (only at even positions of the list, which are indexed at odd values...)
	for (std::size_t i = 1; i < Cv.size(); i += 2) {
		under_anchor += Cv[i].size;
	}

	if constexpr (make_arrangement) {
		base += dir * (to_int64(under_anchor) + 1);
	}

	cost_branch += under_anchor;

	// Index of the positions of Cv. The interval it iterates is [Cv.size(), 1]
	// (and not the usual [Cv.size()-1, 0]) to ensure we are using the right
	// parities (even/odd).
	uint64_t i = Cv.size();

	// SMALLEST to LARGEST
	for (auto it = Cv.rbegin(); it != Cv.rend(); ++it, --i) {
#if defined DEBUG
		assert(i <= Cv.size());
#endif

		const auto& [vi, ni] = *it;

		cost_branch += embed_branch<make_arrangement>(
			L,
			vi,
			make_arrangement ? Dopt_utils::is_even(i)
								   ? base - dir * to_int64(before)
								   : base + dir * to_int64(after)
							 : 0,
			make_arrangement ? Dopt_utils::is_even(i) ? -dir : dir : 0,
			rel_pos
		);
		cost_branch += (Dopt_utils::is_even(i) ? before : after);

		before += (Dopt_utils::is_even(i) ? ni : 0);
		after += (Dopt_utils::is_even(i) ? 0 : ni);

		cost_branch += 1;
	}

	if constexpr (make_arrangement) {
		rel_pos[v] = base;
	}
	return cost_branch;
}

/**
 * @brief Embed a tree's branch.
 *
 * Implementation of procedure 'embed' as defined by Hochberg and Stallmann in
 * \cite Hochberg2003a and with the correction in \cite Alemany2022a.
 *
 * @tparam make_arrangement Whether or not the arrangement is to be constructed.
 * @param L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @param r The vertex root used to construct L.
 * @param[out] arr The optimal arrangement.
 * @returns The cost of a minimum linear arrangement of the corresponding branch.
 * @pre @e L is sorted decreasingly.
 */
template <bool make_arrangement>
[[nodiscard]] uint64_t embed(
	const std::vector<std::vector<node_size>>& L,
	const node r,
	linear_arrangement& arr
) noexcept
{
	const uint64_t n = L.size();
	uint64_t D = 0;

	array<int64_t> rel_pos(n, 0);
	uint64_t left_sum = 0;
	uint64_t right_sum = 0;

	uint64_t i = L[r].size();

	// SMALLEST to LARGEST
	for (auto it = L[r].rbegin(); it != L[r].rend(); ++it, --i) {
		const auto& [vi, ni] = *it;
#if defined DEBUG
		assert(i <= L[r].size());
#endif

		D += embed_branch<make_arrangement>(
			L,
			vi,
			make_arrangement ? Dopt_utils::is_even(i) ? to_int64(right_sum)
													  : -to_int64(left_sum)
							 : 0,
			make_arrangement
				? Dopt_utils::is_even(i) ? to_int64(1) : to_int64(-1)
				: 0,
			rel_pos
		);
		D += (Dopt_utils::is_even(i) ? right_sum : left_sum);

		right_sum += (Dopt_utils::is_even(i) ? ni : 0);
		left_sum += (Dopt_utils::is_even(i) ? 0 : ni);

		D += 1;
	}

	// the '-1' is used to offset the positions from [1,n] to [0,n-1]
	if constexpr (make_arrangement) {
		arr.assign(r, left_sum + 1 - 1);
		rel_pos[r] = 0;
		for (node v = 0; v < n; ++v) {
			const int64_t pos = to_int64(arr[node_t{r}]) + rel_pos[v];
#if defined DEBUG
			assert(pos >= 0);
#endif
			arr.assign(v, to_uint64(pos));
		}
	}

	return D;
}

} // namespace Dmin_utils
} // namespace detail
} // namespace lal
