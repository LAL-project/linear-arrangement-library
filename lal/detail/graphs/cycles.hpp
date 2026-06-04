/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2026
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

#include <iostream>

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/detail/graphs/traversal.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace detail {

static constexpr char STATE_PENDING = 0;
static constexpr char STATE_IN_STACK = 1;
static constexpr char STATE_PROCESSED = 2;

/**
 * @brief Returns true if, and only if, the graph has cycles.
 * @param g Input graph.
 * @param u Node of the directed graph
 * @param visited For each node, has it been visited?
 * @param in_stack For each node, is it in the recursion stack?
 */
[[nodiscard]] inline bool find_cycle(
	const graphs::directed_graph& g,
	const node u,
	array<char>& state
) noexcept
{
	if (state[u] == STATE_PROCESSED) {
		return false;
	}
	if (state[u] == STATE_IN_STACK) {
		return true;
	}
	
	state[u] = STATE_IN_STACK;
	for (const node v : g.get_out_neighbors(u)) {
		if (find_cycle(g, v, state)) {
			return true;
		}
	}
	state[u] = STATE_PROCESSED;
	
	return false;
}

/**
 * @brief Returns true if, and only if, the graph has *DIRECTED* cycles.
 * @param g Input graph.
 * @param vis Array of size 'n', where 'n' is the number of vertices of 'g'.
 * @param in_stack Array of size 'n', where 'n' is the number of vertices of 'g'.
 * @returns Whether the graph has cycles or not.
 */
[[nodiscard]] inline bool has_directed_cycles(
	const graphs::directed_graph& g,
	array<char>& state
) noexcept
{
	const uint64_t n = g.get_num_nodes();
	std::fill(state.begin(), state.end(), STATE_PENDING);
	
	bool has_cycle = false;
	node u = 0;
	while (u < n and not has_cycle) {
		if (state[u] == STATE_PENDING) {
			has_cycle = find_cycle(g, u, state);
		}
		++u;
	}
	return has_cycle;
}

/**
 * @brief Returns true if, and only if, the graph has DIRECTED cycles.
 * @param g Input graph.
 * @returns Whether the graph has cycles or not.
 */
[[nodiscard]] inline bool has_directed_cycles(const graphs::directed_graph& g
) noexcept
{
	const uint64_t n = g.get_num_nodes();
	array<char> state(n);
	return has_directed_cycles(g, state);
}

/**
 * @brief Returns true if, and only if, the graph has UNDIRECTED cycles.
 *
 * In case the input graph is a directed graph, reverse edges are considered.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @param bfs Breadth-First Search object.
 * @returns Whether the graph has cycles or not.
 */
template <graphs::Graph graph_t>
[[nodiscard]] bool
has_undirected_cycles(const graph_t& g, BFS<graph_t>& bfs) noexcept
{
	const auto n = g.get_num_nodes();

	// parent[s] = t <->
	// (in the traversal) s was reached from t (NOTE THE DIFFERENT ORDER).
	// Note that read operations "if (parent[s] != t)" always come after
	// the first write "parent[t] = s".
	array<node> parent(n, n + 1);
	// a cycle was found
	bool cycle_found = false;

	// we need to traverse "reversed edges" in directed graphs
	bfs.set_use_rev_edges(BFS<graph_t>::is_graph_directed);
	// we need this to detect cycles
	bfs.set_process_visited_neighbors(true);
	// -- functions for the traversal
	bfs.set_terminate(
		[&](const node) -> bool
		{
			return cycle_found;
		}
	);
	bfs.set_process_neighbour(
		[&](const node s, const node t, const bool) -> void
		{
			// Since we want to do the traversal on the directed graphs likewise on
			// the undirected graphs, the direction is ignored. We do not want to
			// treat the nodes 's' and 't' as in the edge "t->s" but as in the
			// edge "s->t" so as to mimic an "undirected traversal" on directed
			// graphs.

			if (bfs.node_was_visited(t)) {
				// if t was visted before then
				//     "s -> t" and later "t -> s"
				// or
				//     "s -> ..." and later "... -> s"
				//     where '...' does not contain 't'
				if (parent[s] != t) {
					// node 't' was reached from some node
					// other than 's' in previous iterations
					cycle_found = true;
				}
			}
			parent[t] = s;
		}
	);

	// find cycles
	for (node u = 0; u < n and not cycle_found; ++u) {
		if (not bfs.node_was_visited(u)) {
			bfs.clear_queue();
			bfs.start_at(u);
		}
	}
	return cycle_found;
}

/**
 * @brief Returns true if, and only if, the graph has UNDIRECTED cycles.
 *
 * In case the input graph is a directed graph, reverse edges are considered.
 * @tparam graph_t Type of graph.
 * @param g Input graph.
 * @returns Whether the graph has cycles or not.
 */
template <graphs::Graph graph_t>
[[nodiscard]] bool has_undirected_cycles(const graph_t& g) noexcept
{
	// BFS traversal object
	BFS<graph_t> bfs(g);
	return has_undirected_cycles(g, bfs);
}

} // namespace detail
} // namespace lal
