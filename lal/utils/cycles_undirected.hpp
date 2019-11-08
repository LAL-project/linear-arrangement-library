/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
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
 * @brief Returns true if, and only if, the graph has cycles.
 * @param g Input graph.
 */
inline bool has_cycles(const graphs::ugraph& g) {
	typedef graphs::ugraph G;
	const auto n = g.n_nodes();

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
	[&cycle_found](const BFS<G>&, node) -> bool {
		return cycle_found;
	}
	);
	bfs_trav.set_process_neighbour(
	[&](const BFS<G>& bfs, node s, node t) -> void {
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
	for (node u = 0; u < n and not cycle_found; ++u) {
		if (not bfs_trav.node_was_visited(u)) {
			bfs_trav.clear_queue();
			bfs_trav.start_at(u);
		}
	}
	return cycle_found;
}

} // -- namespace utils
} // -- namespace lal
