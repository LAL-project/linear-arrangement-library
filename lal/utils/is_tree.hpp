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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

#pragma once

#include <lal/utils/bfs.hpp>
#include <lal/graphs/ugraph.hpp>

namespace lal {
namespace utils {

/*
 * @brief Returns true if, and only if, the graph is a tree.
 *
 * By definition, an undirected graph is a tree if it does not contain
 * cycles and has one single connected component.
 * @param g Input graph.
 */
inline
bool is_tree(const graphs::ugraph& g) {
	typedef graphs::ugraph G;
	const auto n = g.n_nodes();

	// simple cases
	if (n <= 1) { return true; }
	if (n == 2 and g.n_edges() == 1) { return true; }
	if (n == 3 and g.n_edges() == 2) { return true; }

	// parent[s] = t <->
	// (in the traversal) s was reached from t (NOTE THE DIFFERENT ORDER)
	// After 't' comes 's'
	std::vector<node> parent(n);
	// a cycle was found
	bool cycle_found = false;

	// BFS traversal object
	BFS<G> bfs_trav(g);
	bfs_trav.process_visited_neighbours(true);
	// functions for the traversal
	bfs_trav.set_terminate(
	[&cycle_found](const BFS<G>&, const node) -> bool {
		return cycle_found;
	}
	);
	bfs_trav.set_process_neighbour(
	[&](const BFS<G>& bfs, const node s, const node t) -> void {
		if (bfs.node_was_visited(t)) {
			// if t was visted before then
			//     "s -> t" and later "t -> s"
			// or
			//     "s -> ..." and later "... -> s"
			//     where '...' does not contain 't'
			if (parent[s] != t) {
				// node t was reached from some node
				// other than 's' in previous iterations
				cycle_found = true;
			}
		}
		parent[t] = s;
	}
	);

	// find cycles
	bfs_trav.start_at(0);
	return not cycle_found and bfs_trav.all_visited();
}

} // -- namespace utils
} // -- namespace lal
