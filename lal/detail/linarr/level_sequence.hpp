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

// lal includes
#include <lal/iterators/E_iterator.hpp>

namespace lal {
namespace detail {

/// Types of level signature.
enum level_signature_type : int8_t {
	/**
	 * @brief Given per vertex.
	 *
	 * The level value is queried via vertices. "L[u]".
	 */
	per_vertex,
	/**
	 * @brief Given per position.
	 *
	 * The level value is queried via positions. "L[p]".
	 */
	per_position
};


/**
 * @brief Calculates the level signature of an arrangement of a graph.
 *
 * The return type of this function (last parameter) must be that of an array-like
 * type, and has to have a constructor with size (std::size_t) and access operators '[]'.
 *
 * Level values are given per position or per vertex according to @e stype.
 * When levels are calculated per position, the result of this function is an
 * array, say @e A, and the level of node @e u is stored at @e A[ arr[u] ], where
 * @e arr[u] is the position of @e u in the arrangement. When levels are calculated
 * per vertex, the result of this function is an array, say @e A, and the level
 * of node @e u is stored at @e A[u].
 *
 * As defined by \cite Nurse2018a \cite Nurse2019a.
 * @tparam stype Type of level signature.
 * @tparam container_t The type of the container where to store the values. The
 * contained type should be a signed integer value.
 * @tparam graph_t Type of graph.
 * @tparam arrangement_t Type of arrangement.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param[out] Level sequence of the graph.
 * @returns The level sequence of an arrangement per vertex.
 * @pre Parameter @e L can only contain zeroes.
 */
template <
	level_signature_type stype,
	class container_t, class graph_t, class arrangement_t
>
void calculate_level_signature
(const graph_t& g, const arrangement_t& arr, container_t& L)
noexcept
{
	iterators::E_iterator it(g);
	while (not it.end()) {
		const auto [u,v] = it.yield_edge_t();
		const position pu = arr[u];
		const position pv = arr[v];
		if constexpr (stype == level_signature_type::per_position) {
			if (pu < pv) {
				++L[pu];
				--L[pv];
			}
			else {
				--L[pu];
				++L[pv];
			}
		}
		else {
			if (pu < pv) {
				++L[*u];
				--L[*v];
			}
			else {
				--L[*u];
				++L[*v];
			}
		}
	}
}

/**
 * @brief Calculates the level signature of an arrangement of a graph.
 *
 * The return type of this function must be that of an array-like type, and
 * has to have a constructor with size (std::size_t) and access operators '[]'.
 *
 * Level values are given per position or per vertex according to @e stype.
 * When levels are calculated per position, the result of this function is an
 * array, say @e A, and the level of node @e u is stored at @e A[ arr[u] ], where
 * @e arr[u] is the position of @e u in the arrangement. When levels are calculated
 * per vertex, the result of this function is an array, say @e A, and the level
 * of node @e u is stored at @e A[u].
 *
 * As defined by \cite Nurse2018a \cite Nurse2019a.
 * @tparam stype Type of level signature.
 * @tparam container_t The type of the container where to store the values. The
 * contained type should be a signed integer value.
 * @tparam graph_t Type of graph.
 * @tparam arrangement_t Type of arrangement.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @returns The level sequence of an arrangement per vertex.
 */
template <
	level_signature_type stype,
	class container_t, class graph_t, class arrangement_t
>
container_t calculate_level_signature
(const graph_t& g, const arrangement_t& arr)
noexcept
{
	const auto n = g.get_num_nodes();
	container_t L(n);
	for (position p = 0ull; p < n; ++p) { L[p] = 0; }
	calculate_level_signature<stype>(g, arr, L);
	return L;
}

/**
 * @brief Returns true if the level sequence follows that of a maximum arrangement.
 *
 * Checks the conditions defined by Nurse and De Vos \cite Nurse2018a \cite Nurse2019a,
 * defined as:
 * - The sequence of level values has to be non-decreasing.
 * - No two adjacent vertices in the graph can have equal level value.
 *
 * The level signature is to be implemented with an array that supports access
 * with operator '[]'. Level values are given per position when @e per_position
 * is true; they are given per vertex, when false.
 * @tparam stype Type of level signature.
 * @tparam graph_t Type of graph.
 * @tparam arrangement_t Type of arrangement.
 * @tparam container_t The type of the container where to store the values. The
 * contained type should be a signed integer value.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param Level signature of the arrangement.
 * @returns Whether or not the level sequence satisfies the two conditions.
 */
template <
	level_signature_type stype,
	class graph_t, class arrangement_t, class container_t
>
bool is_level_signature_appropriate_NDV
(const graph_t& g, const arrangement_t& arr, const container_t& levels)
noexcept
{
	const auto n = g.get_num_nodes();

	if constexpr (stype == level_signature_type::per_position) {
		for (position p = 0; p < n - 1; ++p) {
			if (levels[p] < levels[p + 1]) {
				return false;
			}
		}
	}
	else {
		for (position_t p = 0ull; p < n - 1ull; ++p) {
			const node u = arr[p];
			const node v = arr[p + 1ull];
			if (levels[u] < levels[v]) {
				return false;
			}
		}
	}

	// no two adjacent vertices can have equal level value
	for (iterators::E_iterator it(g); not it.end(); it.next()) {
		if constexpr (stype == level_signature_type::per_position) {
			const auto [u,v] = it.yield_edge_t();
			const position pu = arr[u];
			const position pv = arr[v];
			if (levels[pu] == levels[pv]) {
				return false;
			}
		}
		else {
			const auto [u,v] = it.yield_edge();
			if (levels[u] == levels[v]) {
				return false;
			}
		}
	}
	return true;
}

} // -- namespace detail
} // -- namespace lal
