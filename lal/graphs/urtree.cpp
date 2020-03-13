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

#include <lal/graphs/urtree.hpp>

// C++ includes
#include <cassert>
#include <vector>
using namespace std;

// lal includes
#include <lal/utils/bfs.hpp>
#include <lal/utils/size_subtrees.hpp>

namespace lal {
using namespace utils;

namespace graphs {

urtree::urtree() : utree() { }
urtree::urtree(uint32_t n) {
	utree::_init(n);
	rtree::tree_init(n);
}
urtree::urtree(const utree& t, node r) : utree() {
	init_rooted(t, r);
}
urtree::~urtree() { }

void urtree::init_rooted(const utree& t, node r) {
	_clear();
	_init(t.n_nodes());

	*static_cast<ugraph *>(this) = t;
	set_root(r);
}

bool urtree::is_rooted() const { return true; }

vector<edge> urtree::get_edges_subtree(node r, bool relab) const {
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
	BFS<urtree> bfs(*this);
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
	if (relab) {
		bfs.set_process_neighbour(
		[&](const auto&, node s, node t, bool) -> void {
			edge e;
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
			es.push_back(e);
		}
		);
	}
	else {
		bfs.set_process_neighbour(
		[&](const auto&, node s, node t, bool) -> void {
			es.push_back(edge(s,t));
		}
		);
	}

	// start the bfs again, this time at 'r'
	bfs.start_at(r);
	return es;
}

urtree urtree::get_subtree(node r) const {
	// if the tree does not have edges, return a copy.
	if (n_nodes() <= 1) { return *this; }

	assert(is_tree());
	assert(has_root());
	assert(has_node(r));

	// retrieve the list of edges with their vertices relabelled
	const vector<edge> es = get_edges_subtree(r, true);
	// number of vertices of subtree
	const uint32_t n_verts = static_cast<uint32_t>(es.size()) + 1;

	// make subtree
	urtree sub(n_verts);
	sub.set_root(0);
	sub.add_edges(es);
	return sub;
}

void urtree::recalc_size_subtrees() {
	assert(is_tree());
	assert(has_root());

	m_recalc_size_subtrees = false;
	vector<bool> vis(n_nodes(), false);
	utils::get_undirected_size_subtrees(*this, get_root(), vis, m_size_subtrees);
}

/* PROTECTED */

void urtree::_init(uint32_t n) {
	rtree::tree_init(n);
	utree::_init(n);
}

void urtree::_clear() {
	rtree::_clear();
	utree::_clear();
}

} // -- namespace graphs
} // -- namespace lal
