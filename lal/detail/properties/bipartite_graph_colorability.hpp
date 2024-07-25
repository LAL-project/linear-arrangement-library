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
#include <lal/detail/graphs/traversal.hpp>
#include <lal/properties/bipartite_graph_coloring.hpp>

namespace lal {
namespace detail {

/**
 * @brief Colors the vertices of a bipartite graph.
 * @param g Input graph to be colored.
 * @returns A coloring of a bipartite graph @ref lal::properties::bipartite_graph_coloring.
 * @pre The input graph must be a bipartite graph (not necessarily connected),
 * for the coloring to be correct.
 */
template <class graph_t>
[[nodiscard]] properties::bipartite_graph_coloring color_vertices_graph
(const graph_t& g)
noexcept
{
	const auto n = g.get_num_nodes();
	properties::bipartite_graph_coloring colors(n);

	BFS<graph_t> bfs(g);

	bfs.set_use_rev_edges( g.is_directed() );

	bfs.set_process_neighbour(
	[&](const auto&, lal::node u, lal::node v, bool) {
		if (colors[u] == properties::bipartite_graph_coloring::blue) {
			colors[v] = properties::bipartite_graph_coloring::red;
		}
		else {
			colors[v] = properties::bipartite_graph_coloring::blue;
		}
	});

	for (node u = 0; u < n; ++u) {
		if (not bfs.node_was_visited(u)) {
			colors[u] = properties::bipartite_graph_coloring::blue;
			bfs.start_at(u);
		}
	}
	return colors;
}

} // -- namespace detail
} // -- namespace lal
