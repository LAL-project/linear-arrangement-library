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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/arrangement_wrapper.hpp>

namespace lal {
namespace detail {

/**
 * @brief Number of right branching edges in a directed graph
 * @tparam arrangement_t Type of arrangement.
 * @param g Input directed graph.
 * @param arr Input linear arrangement.
 */
template <Arrangement arrangement_t>
[[nodiscard]] uint64_t right_branching_edges(
	const graphs::directed_graph& g, const arrangement_t& arr
) noexcept
{
	uint64_t edges_to_right = 0;
	for (iterators::E_iterator e_it(g); not e_it.end(); e_it.next()) {
		const auto [u, v] = e_it.get_edge_t();
		edges_to_right += arr[u] < arr[v];
	}
	return edges_to_right;
}

/**
 * @brief Proposition of right branching edges in a directed graph
 * @tparam result_t Type of return value.
 * @tparam arrangement_t Type of arrangement.
 * @param g Input directed graph.
 * @param arr Input linear arrangement.
 */
template <typename result_t, Arrangement arrangement_t>
[[nodiscard]] result_t
head_initial(const graphs::directed_graph& g, const arrangement_t& arr) noexcept
{
	static_assert(std::is_same_v<result_t, numeric::rational> or std::is_same_v<result_t, double>);

#if defined DEBUG
	assert(g.get_num_edges() > 0);
#endif

	const uint64_t rbe = right_branching_edges(g, arr);

	if constexpr (std::is_same_v<result_t, numeric::rational>) {
		return numeric::rational(rbe, g.get_num_edges());
	}
	else {
		return to_double(rbe) / to_double(g.get_num_edges());
	}
}

} // namespace detail
} // namespace lal
