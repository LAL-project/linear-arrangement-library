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

#include <lal/graphs/utree.hpp>

// C includes
#include <assert.h>

// C++ includes
using namespace std;

// lal includes
#include <lal/utils/bfs.hpp>
#include <lal/utils/cycles_undirected.hpp>

namespace lal {
namespace graphs {

utree::utree() : ugraph() { }
utree::utree(uint32_t n) : ugraph(n) { }
utree::utree(const ugraph& t) : ugraph(t.n_nodes()) {
	// Note: add_edges already checks that 't' is a tree or not. It does
	// so implicitly: after the addition of the edges, the method looks
	// for cycles in the graph. If there are, some assertion will fail
	add_edges(t.edges());
}

utree& utree::add_edge(node s, node t, bool norm) {
#if defined DEBUG
	assert(can_add_edge(s,t));
#endif

	ugraph::add_edge(s,t, norm);
	return *this;
}

utree& utree::add_edges(const vector<edge>& edges, bool norm) {
	ugraph::add_edges(edges, norm);
	assert(not utils::graph_has_cycles(*this));
	return *this;
}

bool utree::can_add_edge(node s, node t) const {
	// if the tree already has n-1 edges then
	// adding another edge will produce a cycle
	if (n_edges() + 1 > n_nodes() - 1) {
		return false;
	}

	// check that adding this edge does not produce cyces
	return not utils::is_node_reachable_from(*this, s, t);
}

bool utree::can_add_edges(const vector<edge>& edges) const {
	// in a tree we must have m <= n - 1
	const uint64_t more_m = edges.size();
	if (n_edges() + more_m > n_nodes() - 1) {
		return false;
	}

	// Copy the current graph
	ugraph copy = *this;
	// add the edges to the copy
	copy.add_edges(edges, false);

	// check that there are no cycles in the copy
	return not utils::graph_has_cycles(copy);
}

} // -- namespace graphs
} // -- namespace lal
