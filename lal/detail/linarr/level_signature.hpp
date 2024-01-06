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
#include <lal/linear_arrangement.hpp>
#include <lal/linarr/level_signature.hpp>

namespace lal {
namespace detail {

/**
 * @brief Returns true if the level sequence follows that of a maximum arrangement.
 *
 * Checks the conditions defined by Nurse and De Vos \cite Nurse2018a \cite Nurse2019a,
 * defined as:
 * - The sequence of level values has to be non-decreasing.
 * - No two adjacent vertices in the graph can have equal level value.
 *
 * @tparam t Level signature type.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param Level signature of the arrangement.
 * @returns Whether or not the level sequence satisfies the two conditions.
 */
template <class graph_t, linarr::level_signature_type t>
bool is_level_signature_maximum(
	const graph_t& g,
	const linear_arrangement& arr,
	const linarr::level_signature<t>& levels
)
noexcept
{
	const auto n = g.get_num_nodes();

	if constexpr (t == linarr::level_signature_type::per_position) {
		// (1) the sequence of level values must be non-increasing
		for (position_t p = 0ull; p < n - 1ull; ++p) {
			if (levels[p] < levels[p + 1ull]) {
				return false;
			}
		}

		// (2) no two adjacent vertices can have equal level value
		for (iterators::E_iterator it(g); not it.end(); it.next()) {
			const auto [u,v] = it.yield_edge_t();
			const position_t pu = (arr.size() == 0 ? *u : arr[u]);
			const position_t pv = (arr.size() == 0 ? *v : arr[v]);
			if (levels[pu] == levels[pv]) {
				return false;
			}
		}
	}
	else {
		// (1) the sequence of level values must be non-increasing
		for (position_t p = 0ull; p < n - 1ull; ++p) {
			const node_t u = (arr.size() == 0 ? *p : arr[p]);
			const node_t v = (arr.size() == 0 ? *p + 1 : arr[p + 1ull]);
			if (levels[u] < levels[v]) {
				return false;
			}
		}

		// (2) no two adjacent vertices can have equal level value
		for (iterators::E_iterator it(g); not it.end(); it.next()) {
			const auto [u,v] = it.yield_edge_t();
			if (levels[u] == levels[v]) {
				return false;
			}
		}
	}

	return true;
}

} // -- namespace detail
} // -- namespace lal
