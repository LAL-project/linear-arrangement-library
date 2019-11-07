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

#include <lal/graphs/drtree.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <vector>
using namespace std;

// lal includes
#include <lal/utils/bfs.hpp>

namespace lal {
using namespace utils;

namespace graphs {

drtree::drtree() : dtree() { }
drtree::drtree(uint32_t n) : dtree(n) { }
drtree::drtree(const utree& g, node r, bool arb) : dtree() {
	init_rooted(g, r, arb);
}
drtree::~drtree() { }

void drtree::init_rooted(const utree& _t, node r, bool arb) {
	// assert(is_tree(t));
	if (_t.n_nodes() == 0) {
		dgraph::init(0);
		m_r = 0;
		return;
	}

	// build list of directed edges out of 'g' ...
	vector<edge> dir_edges(_t.n_edges());
	auto it_dir_edges = dir_edges.begin();

	BFS<ugraph> bfs(_t);
	bfs.set_process_neighbour(
	[&](const BFS<ugraph>&, node s, node t) -> void {
		if (arb) {
			// the tree is an arborescence, i.e., the
			// edges point away from the root
			*it_dir_edges = edge(s,t);
		}
		else {
			// the tree is an anti-arborescence, i.e., the
			// edges point towards the root
			*it_dir_edges = edge(t,s);
		}
		++it_dir_edges;
	}
	);
	bfs.start_at(r);

	// construct rooted directed tree
	dgraph::init(_t.n_nodes());
	add_edges(dir_edges);
	m_r = r;
}

} // -- namespace graphs
} // -- namespace lal
