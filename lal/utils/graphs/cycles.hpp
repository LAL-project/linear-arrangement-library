/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Linear Arrangement Library.
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

#include <lal/graphs/dgraph.hpp>
#include <lal/utils/graphs/traversal.hpp>

namespace lal {
namespace utils {

namespace __lal {

/*
 * @brief Returns true if, and only if, the graph has cycles.
 * @param g Input graph.
 * @param u Node of the directed graph
 * @param visited For each node, has it been visited?
 * @param in_stack For each node, is it in the recursion stack?
 */
inline bool __find_cycle
(
	const graphs::dgraph& g, node u,
	std::vector<bool>& visited, std::vector<bool>& in_stack
)
{
	if (visited[u]) { return false; }
	visited[u] = true;

	in_stack[u] = true;
	for (node v : g.get_neighbours(u)) {
		if (in_stack[v]) {
			return true;
		}
		if (not visited[v] and __find_cycle(g,v,visited,in_stack)) {
			return true;
		}
	}

	in_stack[u] = false;
	return false;
}
} // -- namespace __lal

/*
 * @brief Returns true if, and only if, the graph has DIRECTED cycles.
 * @param g Input graph.
 * @returns Returns whether the graph has cycles or not.
 */
inline bool has_directed_cycles(const graphs::dgraph& g) {
	const uint64_t n = g.n_nodes();
	std::vector<bool> vis(n, false);
	std::vector<bool> in_stack(n, false);

	bool has_cycle = false;
	for (node u = 0; u < n and not has_cycle; ++u) {
		if (not vis[u]) {
			has_cycle = __lal::__find_cycle(g, u, vis, in_stack);
		}
	}
	return has_cycle;
}

/*
 * @brief Returns true if, and only if, the graph has UNDIRECTED cycles.
 *
 * In case the input graph is a directed graph, reverse edges are considered.
 * @param g Input graph.
 * @returns Returns whether the graph has cycles or not.
 */
template<class G>
inline bool has_undirected_cycles(const G& g) {
	// BFS traversal object
	BFS<G> bfs(g);
	return has_undirected_cycles(g, bfs);
}

/*
 * @brief Returns true if, and only if, the graph has UNDIRECTED cycles.
 *
 * In case the input graph is a directed graph, reverse edges are considered.
 * @param g Input graph.
 * @returns Returns whether the graph has cycles or not.
 */
template<class G>
inline bool has_undirected_cycles(const G& g, BFS<G>& bfs) {
	const auto n = g.n_nodes();

	// parent[s] = t <->
	// (in the traversal) s was reached from t (NOTE THE DIFFERENT ORDER).
	// Note that read operations "if (parent[s] != t)" always come after
	// the first write "parent[t] = s".
	std::vector<node> parent(n);
	// a cycle was found
	bool cycle_found = false;

	// we need to traverse "reversed edges" in directed graphs
	bfs.set_use_rev_edges(g.is_directed());
	// we need this to detect cycles
	bfs.set_process_visited_neighbours(true);
	// -- functions for the traversal
	bfs.set_terminate(
	[&cycle_found](const auto&, const node) -> bool { return cycle_found; }
	);
	bfs.set_process_neighbour(
	[&](const auto& _bfs, node s, node t, bool) -> void {
		// Since we want to do the traversal on the directed graphs likewise on
		// the undirected graphs, the direction is ignored. We do not want to
		// treat the nodes 's' and 't' as in the edge "t->s" but as in the
		// edge "s->t" so as to mimic an "undirected traversal" on directed
		// graphs.

		if (_bfs.node_was_visited(t)) {
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
			bfs.clear_structure();
			bfs.start_at(u);
		}
	}
	return cycle_found;
}

} // -- namespace utils
} // -- namespace lal
