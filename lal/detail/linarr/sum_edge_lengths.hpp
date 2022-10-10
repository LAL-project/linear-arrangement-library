/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2022
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
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/iterators/E_iterator.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/detail/macros/basic_convert.hpp>

namespace lal {
namespace detail {

/**
 * @brief Sum of edge lengths in a graph.
 * @tparam graph_t Type of graph.
 * @tparam arrangement_t Type of arrangement.
 * @param g Input directed graph.
 * @param arr Input linear arrangement.
 */
template <class graph_t, class arrangement_t>
uint64_t sum_edge_lengths(
	const graph_t& g,
	const arrangement_t& arr
)
noexcept
{
	// sum of lengths
	uint64_t l = 0;

	iterators::E_iterator<graph_t> e_it(g);
	for (; not e_it.end(); e_it.next()) {
		const auto [u,v] = e_it.get_edge_t();

		const auto pu = arr[u];
		const auto pv = arr[v];

		// accumulate length of edge
		l += (pu < pv ? pv - pu : pu - pv);
	}
	return l;
}

/**
 * @brief Average sum of edge lengths in a graph.
 * @tparam result_t Type of return value.
 * @tparam graph_t Type of graph.
 * @tparam arrangement_t Type of arrangement.
 * @param g Input directed graph.
 * @param arr Input linear arrangement.
 */
template <typename result_t, class graph_t, class arrangement_t>
result_t mean_sum_edge_lengths(
	const graph_t& g,
	const arrangement_t& arr
)
noexcept
{
	static_assert(
		std::is_same_v<result_t, numeric::rational> or
		std::is_same_v<result_t, double>
	);

#if defined DEBUG
	assert(g.get_num_edges() > 0);
#endif

	const uint64_t D = sum_edge_lengths(g, arr);

	if constexpr (std::is_same_v<result_t, numeric::rational>) {
		return numeric::rational(D, g.get_num_edges());
	}
	else {
		return detail::to_double(D)/detail::to_double(g.get_num_edges());
	}
}

} // -- namespace detail
} // -- namespace lal
