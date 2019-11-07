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

#include <lal/graphs/dgraph.hpp>

namespace lal {
namespace utils {

inline bool __find_cycle
(
	const graphs::graph& g, node u,
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

/*
 * @brief Returns true if, and only if, the graph has cycles.
 * @param g Input graph.
 */
bool graph_has_cycles(const graphs::dgraph& g) {

	const uint32_t n = g.n_nodes();
	std::vector<bool> vis(n, false);
	std::vector<bool> in_stack(n, false);

	bool has_cycle = false;
	for (node u = 0; u < n and not has_cycle; ++u) {
		if (not vis[u]) {
			has_cycle = __find_cycle(g, u, vis, in_stack);
		}
	}

	return has_cycle;
}

} // -- namespace utils
} // -- namespace lal
