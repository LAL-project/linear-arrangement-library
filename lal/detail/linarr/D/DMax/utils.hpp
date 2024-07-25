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

/// Utilities for the various maximum linear arrangement algorithms.
namespace DMax_utils {

/* ************************************************************************** */
/* ---------------------- INTERVAL-based methods ---------------------------- */

/* The following namespace contains functions for the interval-based algorithms
 * to calculate the planar and projective maximum sum of edge lengths.
 */

/**
 * @brief Make a maximum projective arrangement using the sorted, rooted
 * adjacency list @e L.
 *
 * @tparam r_place Position of node @e r with respect to its parent.
 * @tparam make_arrangement Whether or not the arrangement is to be constructed.
 * @param L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @param r The vertex root of the subtree whose interval is to be made
 * @param ini Left limit of the positions of the arrangement in which the tree
 * has to be arranged. Note that the limits are included [@e ini,@e fin].
 * @param fin Right limit of the positions of the arrangement in which
 * the tree has to be arranged. Note that the limits are included [@e ini,@e fin].
 * @param[out] arr the arrangement of the tree.
 *
 * @returns The sum of the length of the outgoing edges from vertex 'r' plus
 * the length of the anchor of the edge from 'r' to its parent. Such length
 * is defined as the number of vertices to the left of 'r' if 'r_place'
 * is RIGHT_PLACE, or as the number of vertices to the right of 'r' if
 * 'r_place' is LEFT_PLACE.
 * @pre @e L is sorted decreasingly.
 */
template <Dopt_utils::place r_place, bool make_arrangement>
[[nodiscard]] uint64_t arrange
(
	const std::vector<std::vector<node_size>>& L,
	const node r,
	position ini, position fin,
	linear_arrangement& arr
)
noexcept
{
#if defined DEBUG
	assert(ini <= fin);
#endif

	if constexpr (make_arrangement) {
		if constexpr (r_place == Dopt_utils::PLACE_LEFT_OF) {
			arr.assign(r, ini);
		}
		else {
			// It is clear that for the case 'r_place == PLACE_RIGHT_OF', we
			// need the code below. For the case 'r_place == PLACE_NONE_OF', the
			// code below is an arbitrary choice, but it is in accordance with
			// the steps of this algorithm.
			arr.assign(r, fin);
		}
	}

	// sizes of the subtrees
	const auto& children = L[r];

	// accumulated size of the subtrees
	uint64_t acc_size = 0;

	// sum of the optimal D for every subtree +
	// the length of the edge from 'r' to its parent (if any)
	uint64_t D = 0;

	// Auxiliary variables that contain the next
	// starting position and the next ending position.
	// Initialized so the compiler does not cry
	position next_ini = 0, next_fin = 0;

	constexpr Dopt_utils::place next_place =
		r_place == Dopt_utils::PLACE_LEFT_OF ?
			Dopt_utils::PLACE_RIGHT_OF :
			Dopt_utils::PLACE_LEFT_OF;

	// while placing the children, calculate the
	// length of the edge from 'r' to vertex 'vi'
	for (const auto& [vi, ni] : children) {

		if constexpr (make_arrangement) {
			if constexpr (r_place == Dopt_utils::PLACE_LEFT_OF) {
				next_ini = ini + acc_size + 1;
				next_fin = next_ini + ni - 1;
			}
			else {
				// It is clear that for the case 'r_place == PLACE_RIGHT_OF', we
				// need the code below. For the case 'r_place == PLACE_NONE_OF', the
				// code below is an arbitrary choice, but it is in accordance with
				// the steps of this algorithm.
				next_fin = fin - acc_size - 1;
				next_ini = next_fin - ni + 1;
			}
		}

		// recursive call: make the interval of 'vi'
		D += arrange<next_place, make_arrangement>(L, vi, next_ini, next_fin, arr);

		D += 1 + acc_size;
		acc_size += ni;
	}

	if constexpr (r_place != Dopt_utils::PLACE_NONE_OF) {
		// accumulate this subtree's anchor
		D += acc_size;
	}
	return D;
}

/**
 * @brief Wrapper method for the recursive method @ref arrange.
 *
 * A call to this function is done when the goal is to construct a linear arrangement.
 * @param n Number of vertices.
 * @param L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @param r Root of the tree represented by @e L.
 * @param[out] arr maximum projective linear arrangement.
 * @returns The cost of a maximum projective linear arrangement.
 * @pre @e L is sorted decreasingly.
 */
[[nodiscard]] inline uint64_t arrange_projective
(
	uint64_t n, const std::vector<std::vector<node_size>>& L,
	node r, linear_arrangement& arr
)
noexcept
{
	return arrange<Dopt_utils::PLACE_NONE_OF, true>(L, r, 0, n-1, arr);
}

/**
 * @brief Wrapper method for the recursive method @ref arrange.
 *
 * A call to this function is done when the goal is not to construct a linear
 * arrangement, only to calculate its cost.
 * @param n Number of vertices.
 * @param L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @param r Root of the tree represented by @e L.
 * @returns The cost of a maximum projective linear arrangement.
 * @pre @e L is sorted decreasingly.
 */
[[nodiscard]] inline uint64_t arrange_projective
(uint64_t n, const std::vector<std::vector<node_size>>& L, node r)
noexcept
{
	linear_arrangement arr;
	return arrange<Dopt_utils::PLACE_NONE_OF, false>(L, r, 0, n-1, arr);
}

} // -- namespcae DMax_utils
} // -- namespace detail
} // -- namespace lal
