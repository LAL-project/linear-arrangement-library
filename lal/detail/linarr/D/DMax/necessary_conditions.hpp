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
#include <lal/detail/linarr/level_signature.hpp>

namespace lal {
namespace detail {

/**
 * @brief Returns true if the level sequence follows that of a maximum arrangement.
 *
 * Checks whether or not the level signature is non-increasing, that is, whether
 * the sequence of level values (in a per position distribution) is non-increasing.
 *
 * This is a necessary condition for an arrangement to be maximum, as shown in
 * \cite Nurse2018a \cite Nurse2019a and \cite Alemany2023a.
 *
 * When the level signature is defined on a 'per position' basis, the arrangement
 * is not needed.
 * @tparam t Level signature type.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param levels signature of the arrangement.
 * @returns Whether or not the level sequence satisfies the condition.
 */
template <class graph_t, level_signature_type t>
[[nodiscard]] inline bool is_level_signature_nonincreasing
(
	const graph_t& g,
	const level_signature<t>& levels,
	const linear_arrangement& arr
)
noexcept
{
	const auto n = g.get_num_nodes();
	if constexpr (t == level_signature_type::per_position) {
		for (position_t p = 0ull; p < n - 1ull; ++p) {
			if (levels[p] < levels[p + 1ull]) {
				return false;
			}
		}
	}
	else {
		for (position_t p = 0ull; p < n - 1ull; ++p) {
			const node_t u = (arr.size() == 0 ? *p : arr[p]);
			const node_t v = (arr.size() == 0 ? *p + 1 : arr[p + 1ull]);
			if (levels[u] < levels[v]) {
				return false;
			}
		}
	}
	return true;
}

/**
 * @brief Returns true if no two adjacent vertices (in the graph) have the same level value.
 *
 * Checks that no edge \f$uv\in E(G)\f$ is such that \f$l_{\pi}(u) = l_{\pi}(v)\f$.
 * This is a necessary condition for an arrangement to be maximum (in sum of edge
 * lengths), as shown in \cite Nurse2018a \cite Nurse2019a and \cite Alemany2023a.
 *
 * When the level signature is defined on a 'per vertex' basis, the arrangement
 * is not needed.
 * @tparam t Level signature type.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param levels signature of the arrangement.
 * @returns Whether or not the level sequence satisfies the condition.
 */
template <class graph_t, level_signature_type t>
[[nodiscard]] inline bool no_two_adjacent_vertices_have_same_level
(
	const graph_t& g,
	const level_signature<t>& levels,
	const linear_arrangement& arr
)
noexcept
{
	if constexpr (t == level_signature_type::per_position) {
		for (iterators::E_iterator it(g); not it.end(); it.next()) {
			const auto [u, v] = it.yield_edge_t();
			const position_t pu = (arr.size() == 0 ? *u : arr[u]);
			const position_t pv = (arr.size() == 0 ? *v : arr[v]);
			if (levels[pu] == levels[pv]) {
				return false;
			}
		}
	}
	else {
		for (iterators::E_iterator it(g); not it.end(); it.next()) {
			const auto [u, v] = it.yield_edge_t();
			if (levels[u] == levels[v]) {
				return false;
			}
		}
	}
	return true;
}

/**
 * @brief Returns true if none of the vertices in the antennas of the graph is a thistle.
 *
 * Checks that no vertex in any antenna of the tree is a thistle vertex in the
 * input arrangement. This is a necessary condition for an arrangement to be
 * maximum (in sum of edge lengths), as shown in \cite Alemany2023a.
 *
 * When the level signature is defined on a 'per vertex' basis, the arrangement
 * is not needed.
 * @tparam t Level signature type.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param levels signature of the arrangement.
 * @param bps All Branchless Paths of the tree.
 * @returns Whether or not the level sequence satisfies the condition.
 */
template <class graph_t, level_signature_type t>
[[nodiscard]] inline bool no_vertex_in_antenna_is_thistle
(
	const graph_t& g,
	const std::vector<properties::branchless_path>& bps,
	const level_signature<t>& levels,
	const linear_arrangement& arr
)
noexcept
{

	for (const properties::branchless_path& bp : bps) {
		if (not bp.is_antenna(g)) { continue; }

		const auto& seq = bp.get_vertex_sequence();
		for (std::size_t i = 1; i < seq.size() - 1; ++i) {
			const node_t u = seq[i];
#if defined DEBUG
			assert(g.get_degree(*u) == 2);
#endif

			if (is_thistle_vertex(g, levels, u, arr)) { return false; }
		}
	}
	return true;
}

/**
 * @brief Returns true if none of the vertices in the antennas of the graph is a thistle.
 *
 * Checks that no vertex in any antenna of the tree is a thistle vertex in the
 * input arrangement. This is a necessary condition for an arrangement to be
 * maximum (in sum of edge lengths), as shown in \cite Alemany2023a.
 *
 * When the level signature is defined on a 'per vertex' basis, the arrangement
 * is not needed.
 * @tparam t Level signature type.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param levels signature of the arrangement.
 * @param bps All Branchless Paths of the tree.
 * @returns Whether or not the level sequence satisfies the condition.
 */
template <class graph_t, level_signature_type t>
[[nodiscard]] inline bool at_most_one_thistle_in_bridges
(
	const graph_t& g,
	const std::vector<properties::branchless_path>& bps,
	const level_signature<t>& levels,
	const linear_arrangement& arr
)
noexcept
{

	for (const properties::branchless_path& bp : bps) {
		if (bp.is_antenna(g)) { continue; }

		uint64_t num_thistles = 0;
		const auto& seq = bp.get_vertex_sequence();
		for (std::size_t i = 1; i < seq.size() - 1; ++i) {
			const node_t u = seq[i];
#if defined DEBUG
			assert(g.get_degree(*u) == 2);
#endif

			num_thistles +=
				(num_thistles == 2 ? 0ull : is_thistle_vertex(g, levels, u, arr));
		}
		if (num_thistles > 1) {
			return false;
		}
	}
	return true;
}

} // -- namespace detail
} // -- namespace lal
