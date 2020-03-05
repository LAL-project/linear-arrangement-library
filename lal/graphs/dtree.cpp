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

#include <lal/graphs/dtree.hpp>

// C++ includes
//#include <iostream>
#include <cassert>
using namespace std;

// lal includes
#include <lal/utils/bfs.hpp>
#include <lal/utils/cycles.hpp>

namespace lal {
namespace graphs {

dtree::dtree() : dgraph() { }
dtree::dtree(uint32_t n) : dgraph(n) { }
dtree::~dtree() { }

dtree& dtree::add_edge(node s, node t, bool norm) {
#if defined DEBUG
	assert(can_add_edge(s,t));
#endif

	dgraph::add_edge(s,t, norm);
	return *this;
}

dtree& dtree::add_edges(const vector<edge>& edges, bool norm) {
	dgraph::add_edges(edges, norm);

#if defined DEBUG
	assert(not utils::has_cycles(*this));
#endif
	return *this;
}

bool dtree::can_add_edge(node s, node t) const {
	// if the tree already has n-1 edges then
	// adding another edge will produce a cycle
	if (n_edges() + 1 > n_nodes() - 1) {
		return false;
	}

	// the edge cannot be in the graph twice:
	// this is not a multigraph
	if (has_edge(s,t)) {
		return false;
	}

	// copy the graph
	dgraph copy = *this;
	// add the edges
	copy.add_edge(s, t);
	// convert the directed graph to an undirected graph
	// and make sure that there are no loops in that
	return not utils::has_cycles(copy.to_undirected());
}

bool dtree::can_add_edges(const std::vector<edge>& edges) const {
	// in a tree we must have m <= n - 1
	const uint64_t more_m = edges.size();
	if (n_edges() + more_m > n_nodes() - 1) {
		return false;
	}

	// check that none of the edges exist
	for (auto e : edges) {
		if (has_edge(e.first, e.second)) {
			return false;
		}
	}

	// copy the graph
	dgraph copy = *this;
	// add the edges
	copy.add_edges(edges);
	// convert the directed graph to an undirected graph
	// and make sure that there are no loops in that
	return not utils::has_cycles(copy.to_undirected());
}

utree dtree::to_undirected() const {
	utree g(n_nodes());
	g.add_edges(edges());
	return g;
}

bool dtree::is_rooted() const { return false; }

/* PROTECTED */

void dtree::_init(uint32_t n) {
	//cout << "dtree::_init(uint32_t)" << endl;
	dgraph::_init(n);
}

void dtree::_clear() {
	//cout << "dtree::_clear()" << endl;
	dgraph::_clear();
}

} // -- namespace graphs
} // -- namespace lal
