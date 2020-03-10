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
using namespace std;

// lal includes
#include <lal/utils/is_tree.hpp>
#include <lal/utils/bfs.hpp>
#include <lal/utils/size_subtrees.hpp>

namespace lal {
using namespace utils;

namespace graphs {

drtree::drtree() : dtree() { }
drtree::drtree(uint32_t n) {
	drtree::_init(n);
	rtree::tree_init(n);
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
	assert(_t.is_tree());
	assert(arb == drtree_type::arborescence or arb == drtree_type::anti_arborescence);

	if (_t.n_nodes() == 0) {
		_init(0);
		return;
	}

	// list of directed edges out of 'g'
	vector<edge> dir_edges(_t.n_edges());
	auto it_dir_edges = dir_edges.begin();

	// build list of directed edges using a breadth-first search
	BFS<utree> bfs(_t);
	if (arb == drtree_type::arborescence) {
		bfs.set_process_neighbour(
		[&](const BFS<utree>&, const node s, const node t, bool) -> void {
			// the tree is an arborescence, i.e., the
			// edges point away from the root
			*it_dir_edges = edge(s,t);
			++it_dir_edges;
		}
		);
	}
	else {
		bfs.set_process_neighbour(
		[&](const BFS<utree>&, const node s, const node t, bool) -> void {
			// the tree is an anti-arborescence, i.e., the
			// edges point towards the root
			*it_dir_edges = edge(t,s);
			++it_dir_edges;
		}
		);
	}
	bfs.start_at(r);

	// construct rooted directed tree
	_init(_t.n_nodes());

	// set root and add edges
	set_root(r);
	add_edges(dir_edges);

	// set directed tree type
	m_drtree_type = arb;
	m_drtree_type_valid = true;
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

void drtree::recalc_size_subtrees(bool rev) {
	assert(is_tree());
	assert(has_root());
	if (not rev) {
		assert(is_tree_type_valid());
		assert(get_drtree_type() == drtree_type::arborescence);
	}

	m_recalc_size_subtrees = false;
	vector<bool> vis(n_nodes(), false);
	if (rev) {
		utils::get_directed_size_subtrees(*this, get_root(), vis, m_size_subtrees);
	}
	else {
		utils::get_undirected_size_subtrees(*this, get_root(), vis, m_size_subtrees);
	}
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

vector<edge> drtree::get_edges_subtree(node r, bool relab) const {
	// if the tree does not have edges, return an empty list.
	if (n_nodes() <= 1) { return vector<edge>(); }

	assert(is_tree());
	assert(has_root());
	assert(has_node(r));

	const auto n = n_nodes();

	bool r_parent_set = false;
	node r_parent = n;

	// -----------------------
	// find parent of vertex r
	BFS<drtree> bfs(*this);
	bfs.set_use_rev_edges(true);
	bfs.set_terminate( [&](const auto&, node) { return r_parent_set; } );
	bfs.set_process_neighbour(
	[&](const auto&, node s, node t, bool) -> void {
		if (t == r) {
			r_parent = s;
			r_parent_set = true;
		}
	}
	);
	bfs.start_at(get_root());

	// -----------------------------
	// retrieve edges of the subtree

	// reset the bfs
	bfs.reset();

	// stop the bfs from going further than 'r''s parent
	// in case such parent exists
	if (r_parent_set) {
		bfs.set_visited(r_parent);
	}

	// data structures for vertex relabelling
	vector<node> labels(n_nodes(), n);
	// we need vertex 'r' to be relabelled to 0.
	labels[r] = 0;
	node next_label = 1;

	// retrieve edges and relabel them at the same time
	vector<edge> es;
	bfs.set_process_neighbour(
	[&](const auto&, node s, node t, bool dir) -> void {
		// change the orientation of the edge whenever appropriate
		// dir: true  ---> "s->t"
		// dir: false ---> "t->s"
		if (not dir) { std::swap(s,t); }

		edge e;
		// relabel vertices
		if (relab) {
			// relabel first vertex
			if (labels[s] == n) {
				labels[s] = next_label;
				++next_label;
			}
			e.first = labels[s];
			// relabel second vertex
			if (labels[t] == n) {
				labels[t] = next_label;
				++next_label;
			}
			e.second = labels[t];
		}
		else {
			e = edge(s,t);
		}
		es.push_back(e);
	}
	);
	// start the bfs again, this time at 'r'
	bfs.start_at(r);
	return es;
}

drtree drtree::get_subtree(node r) const {
	// if the tree does not have edges, return a copy.
	if (n_nodes() <= 1) { return *this; }

	assert(has_root());
	assert(is_tree());
	assert(has_node(r));

	// retrieve the list of edges with their vertices relabelled
	const vector<edge> es = get_edges_subtree(r, true);
	// number of vertices of subtree
	const uint32_t n_verts = static_cast<uint32_t>(es.size()) + 1;

	// make subtree
	drtree sub(n_verts);
	sub.set_root(0);
	sub.add_edges(es);
	return sub;
}

/* PROTECTED */

void drtree::_init(uint32_t n) {
	rtree::tree_init(n);
	dtree::_init(n);
	m_drtree_type_valid = false;
}

void drtree::_clear() {
	rtree::tree_clear();
	dtree::_clear();
	m_drtree_type_valid = false;
}

} // -- namespace graphs
} // -- namespace lal
