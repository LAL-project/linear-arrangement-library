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

// C++ includes
#include <algorithm>

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>

namespace lal {
namespace detail {

/**
 * @brief Retrieves the neighbors of a node in an undirected graph as a
 * list of 0-1 values.
 *
 * Sets to 1 the positions in @e neighs that correspond to the nodes
 * neighours of @e u.
 * @param g Input graph.
 * @param u Input node.
 * @param neighs 0-1 list of neighbors of @e u in @e g.
 * @pre The contents of @e neighs must be all 0 (or false).
 */
template <
	class graph_t,
	typename char_type,
	std::enable_if_t<
		std::is_base_of_v<graphs::graph, graph_t>
		and std::is_integral_v<char_type>,
	bool> = true
>
inline void get_bool_neighbors(
	const graph_t& g,
	const node u,
	char_type * const neighs
)
noexcept
{
	if constexpr (std::is_base_of_v<graphs::directed_graph, graph_t>) {
		const auto& in_u = g.get_in_neighbors(u);
		std::for_each(in_u.begin(), in_u.end(), [&](node v) { neighs[v] = 1; });
		const auto& out_u = g.get_out_neighbors(u);
		std::for_each(out_u.begin(), out_u.end(), [&](node v) { neighs[v] = 1; });
	}
	else {
		const auto& neighs_u = g.get_neighbors(u);
		std::for_each(neighs_u.begin(), neighs_u.end(), [&](node v) { neighs[v] = 1; });
	}
}

/**
 * @brief Append adjacency list 'source' to list 'target'
 * @param target List into which @e source will be appended to.
 * @param source List to append to @e target.
 */
inline void append_adjacency_lists
(std::vector<neighbourhood>& target, const std::vector<neighbourhood>& source)
noexcept
{
	const uint64_t n_target = target.size();
	for (std::size_t u = 0; u < source.size(); ++u) {
		// add new edges by appending all the neighbors of 'u' in 'g'
		target.push_back( source[u] );
		// relabel the nodes
		for (node& v : target.back()) {
			v += n_target;
		}
	}
}

} // -- namespace detail
} // -- namespace lal
