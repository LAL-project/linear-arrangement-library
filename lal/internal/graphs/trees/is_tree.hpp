/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Linear Arrangement Library. To see the full code
 *  visit the webpage:
 *      https://github.com/lluisalemanypuig/linear-arrangement-library.git
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

// lal includes
#include <lal/internal/graphs/traversal.hpp>
#include <lal/internal/graphs/cycles.hpp>

namespace lal {
namespace internal {

/*
 * @brief Returns true if, and only if, the graph is a tree.
 *
 * By definition, an undirected graph is a tree if it does not contain
 * cycles and has one single connected component. Note that isloated nodes
 * count as single connected components.
 *
 * In an attempt to extend the usage of this method, directed graphs are
 * also allowed. In this case, since the data structure allows it, the
 * algorithm looks for undirected cycles in the directed graph.
 * @param g Input graph.
 */
template<class G>
bool is_graph_a_tree(const G& t) {
	const auto n = t.n_nodes();

	// simple cases
	if (n <= 1) { return true; }
	if (n == 2) { return t.n_edges() == 1; }
	if (n == 3) { return t.n_edges() == 2; }

	BFS<G> bfs(t);
	const bool cycle_found = has_undirected_cycles(t, bfs);
	return not cycle_found and bfs.all_visited();
}

} // -- namespace internal
} // -- namespace lal
