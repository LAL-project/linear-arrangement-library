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
#include <lal/detail/graphs/traversal.hpp>

namespace lal {
namespace detail {

/**
 * @brief Is the input graph a tree?
 *
 * By definition, an undirected graph is a tree if it does not contain
 * cycles and has one single connected component. Note that isloated nodes
 * count as single connected components. Directed graphs are allowed.
 * @param g Input graph.
 * @returns True if, and only if, the graph is a tree.
 */
template <graphs::Graph graph_t>
[[nodiscard]] inline bool is_graph_a_tree(const graph_t& g) noexcept
{
	const auto n = g.get_num_nodes();

	// simplest case
	if (n <= 1) {
		return true;
	}

	// check exact amount of edges
	if (g.get_num_edges() != n - 1) {
		return false;
	}

	// visit all vertices, if all vertices
	// were visited then the graph is a tree
	BFS<graph_t> bfs(g);
	bfs.set_use_rev_edges(BFS<graph_t>::is_graph_directed);
	bfs.start_at(0);
	return bfs.all_visited();
}

} // namespace detail
} // namespace lal
