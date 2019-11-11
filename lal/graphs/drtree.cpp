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
#include <queue>
using namespace std;

// lal includes
#include <lal/utils/bfs.hpp>
#include <lal/utils/is_tree.hpp>

namespace lal {
using namespace utils;

namespace graphs {

drtree::drtree() : dtree() { }
drtree::drtree(uint32_t n) : dtree(n) { }
drtree::drtree(const utree& t, node r, bool arb) : dtree() {
	// assert(utils::is_tree(t));
	// the assertion above is done in "init_rooted" method
	init_rooted(t, r, arb);
}
drtree::~drtree() { }

void drtree::init_rooted(const utree& _t, node r, bool arb) {
	assert(utils::is_tree(_t));

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
	set_root(r);
	m_drtree_type = (arb ? arborescence : anti_arborescence);
}

void drtree::find_drtree_type() {
	assert(has_root());

	// First case: the tree is NOT an anti-arborescence.
	// Do a BFS from the root. Make sure that all leaves have
	// been reached. If so, the tree is an arborescence.
	if (out_degree(get_root()) > 0) {
		BFS<drtree> bfs(*this);
		bfs.start_at(get_root());
		auto vis_nodes = bfs.get_visited();
		bool all_vis = find(vis_nodes.begin(), vis_nodes.end(), false) == vis_nodes.end();

		// if some node was not visited then the tree
		// will remain unclassified
		m_drtree_type = (all_vis ? arborescence : none);
		return;
	}

	// Second case: the tree is NOT an arborescence.
	// It might be an anti-arborescence. All vertices'
	// out-degree, excepting the root's, must be exactly 1
	bool all_one = true;
	for (node u = 0; u < n_nodes(); ++u) {
		if (u != get_root() and out_degree(u) != 1) {
			all_one = false;
		}
	}
	m_drtree_type = (all_one ? anti_arborescence : none);
}

/* GETTERS */

urtree drtree::to_undirected() const {
	return urtree(dtree::to_undirected(), get_root());
}

drtree::rooted_directed_tree_type drtree::get_drtree_type() const {
	return m_drtree_type;
}

bool drtree::is_rooted() const { return true; }

} // -- namespace graphs
} // -- namespace lal
