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

#include <lal/graphs/drtree.hpp>

// C++ includes
#include <cassert>
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
drtree::drtree(uint32_t n) : dtree(n) {
	rtree::rtree_init();
}
drtree::drtree(const utree& t, node r, drtree_type arb) : dtree() {
	// assert(utils::is_tree(t));
	// the assertion above is done in "init_rooted" method
	init_rooted(t, r, arb);
}
drtree::~drtree() { }

void drtree::set_root(node r) {
	rtree::set_root(r);
	m_drtree_type_valid = false;
}

void drtree::init_rooted(const utree& _t, node r, drtree_type arb) {
	assert(utils::is_tree(_t));

	if (_t.n_nodes() == 0) {
		dgraph::init(0);
		m_r = 0;
		rtree::rtree_init();
		return;
	}

	// build list of directed edges out of 'g' ...
	vector<edge> dir_edges(_t.n_edges());
	auto it_dir_edges = dir_edges.begin();

	BFS<ugraph> bfs(_t);
	bfs.set_process_neighbour(
	[&](const BFS<ugraph>&, const node s, const node t) -> void {
		if (arb == drtree_type::arborescence) {
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
	m_drtree_type = arb;

	m_drtree_type_valid = true;

	rtree::rtree_init();
}

void drtree::find_drtree_type() {
	assert(is_tree());
	assert(has_root());

	m_drtree_type_valid = true;

	// First case: the tree is NOT an anti-arborescence.
	// Do a BFS from the root. Make sure that all leaves
	// can be reached. If so, the tree is an arborescence.
	if (out_degree(get_root()) > 0) {
		BFS<drtree> bfs(*this);
		bfs.start_at(get_root());

		// if some node was not visited then the tree
		// will remain unclassified
		m_drtree_type = (bfs.all_visited() ? drtree_type::arborescence :
											 drtree_type::none);
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
	m_drtree_type = (all_one ? drtree_type::anti_arborescence :
							   drtree_type::none);
}

/* GETTERS */

urtree drtree::to_undirected() const {
	return urtree(dtree::to_undirected(), get_root());
}

drtree::drtree_type drtree::get_drtree_type() const {
	return m_drtree_type;
}

bool drtree::is_tree_type_valid() const {
	return m_drtree_type_valid;
}

bool drtree::is_rooted() const { return true; }

void drtree::calculate_nodes_subtrees() {
	assert(is_tree());
	m_num_verts_subtree_valid = true;
	calc_nodes_subtree(get_root());
}

/* PRIVATE */

void drtree::calc_nodes_subtree(node r) {
	m_num_verts_subtree[r] = 1;
	for (const node u : get_neighbours(r)) {
		calc_nodes_subtree(u);
		m_num_verts_subtree[r] += m_num_verts_subtree[u];
	}
}

} // -- namespace graphs
} // -- namespace lal
