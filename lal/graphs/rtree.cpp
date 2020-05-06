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

#include <lal/graphs/rtree.hpp>

// C++ includes
#include <cassert>
#include <vector>
using namespace std;

// lal includes
#include <lal/utils/graphs/bfs.hpp>
#include <lal/utils/graphs/trees/is_tree.hpp>
#include <lal/utils/graphs/trees/size_subtrees.hpp>

namespace lal {
using namespace utils;

namespace graphs {

rtree::rtree() : dgraph() { }
rtree::rtree(uint32_t n) : dgraph(n) {
	rtree::_init(n);
}
rtree::rtree(const ftree& t, node r, rtree_type type) {
	init_rooted(t, r, type);
}
rtree::~rtree() { }

/* MODIFIERS */

rtree& rtree::add_edge(node s, node t, bool norm) {
#if defined DEBUG
	assert(can_add_edge(s,t));
#endif

	dgraph::add_edge(s,t, norm);
	return *this;
}

rtree& rtree::add_edges(const vector<edge>& edges, bool norm) {
#if defined DEBUG
	assert(can_add_edges(edges));
#endif

	dgraph::add_edges(edges, norm);
	return *this;
}

bool rtree::find_rtree_type() {
	//assert(is_tree());
	assert(has_root());

	m_rtree_type_valid = true;

	// First case: the tree is NOT an anti-arborescence.
	// Do a BFS from the root. Make sure that all leaves
	// can be reached. If so, the tree is an arborescence.
	if (out_degree(get_root()) > 0) {
		BFS<rtree> bfs(*this);
		bfs.start_at(get_root());

		// if some node was not visited then the tree
		// will remain unclassified
		m_rtree_type = (bfs.all_visited() ?
			rtree_type::arborescence : rtree_type::none);
	}
	else {
		// Second case: the tree is NOT an arborescence.
		// It might be an anti-arborescence. All nodes'
		// out-degree, excepting the root's, must be exactly 1
		bool all_one = true;
		for (node u = 0; u < n_nodes(); ++u) {
			if (u != get_root() and out_degree(u) != 1) {
				all_one = false;
			}
		}
		m_rtree_type = (all_one ?
			rtree_type::anti_arborescence : rtree_type::none);
	}

	return m_rtree_type != rtree_type::none;
}

void rtree::init_rooted(const ftree& _t, node r, rtree_type arb) {
	assert(_t.is_tree());
	assert(arb == rtree_type::arborescence or arb == rtree_type::anti_arborescence);

	if (_t.n_nodes() == 0) {
		_init(0);
		set_root(0);
		return;
	}

	// list of directed edges out of 'g'
	vector<edge> dir_edges(_t.n_edges());
	auto it_dir_edges = dir_edges.begin();

	// build list of directed edges using a breadth-first search
	BFS<ftree> bfs(_t);
	if (arb == rtree_type::arborescence) {
		bfs.set_process_neighbour(
		[&](const BFS<ftree>&, const node s, const node t, bool) -> void {
			// the tree is an arborescence, i.e., the
			// edges point away from the root
			*it_dir_edges = edge(s,t);
			++it_dir_edges;
		}
		);
	}
	else {
		bfs.set_process_neighbour(
		[&](const BFS<ftree>&, const node s, const node t, bool) -> void {
			// the tree is an anti-arborescence, i.e., the
			// edges point towards the root
			*it_dir_edges = edge(t,s);
			++it_dir_edges;
		}
		);
	}
	bfs.start_at(r);

	// construct rooted tree
	rtree::_init(_t.n_nodes());

	// set root and add edges
	set_root(r);
	add_edges(dir_edges);

	// set directed tree type
	m_rtree_type = arb;
}

void rtree::recalc_size_subtrees(bool rev) {
	assert(is_tree());
	assert(has_root());
	if (not rev) {
		assert(rtree_type_valid());
		assert(get_rtree_type() == rtree_type::arborescence);
	}

	m_recalc_size_subtrees = false;
	vector<bool> vis(n_nodes(), false);
	if (rev) {
		utils::get_size_subtrees_follow_reversed(*this, get_root(), vis, m_size_subtrees);
	}
	else {
		utils::get_size_subtrees(*this, get_root(), vis, m_size_subtrees);
	}
}

/* SETTERS */

void rtree::set_root(node r) {
	// if the tree is empty simply consider it has a root...
	// although it really doesn't

	if (n_nodes() > 0) {
		assert(has_node(r));
		m_root = r;
	}
	m_has_root = true;
	m_recalc_size_subtrees = true;
	m_rtree_type_valid = false;
}

void rtree::set_rtree_type(const rtree_type& type) {
	m_rtree_type = type;
	m_rtree_type_valid = true;
}

/* GETTERS */

bool rtree::can_add_edge(node s, node t) const {
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
	// add the edge
	copy.add_edge(s, t);
	// convert the directed graph to an undirected graph
	// and make sure that there are no loops in that
	return not utils::has_undirected_cycles(copy);
}

bool rtree::can_add_edges(const std::vector<edge>& edges) const {
	// in a tree we must have m <= n - 1
	const uint64_t more_m = edges.size();
	if (n_edges() + more_m > n_nodes() - 1) {
		return false;
	}

	// check that none of the edges exist
	for (const edge& e : edges) {
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
	return not utils::has_undirected_cycles(copy);
}

bool rtree::is_rooted() const { return true; }

rtree::rtree_type rtree::get_rtree_type() const {
	return m_rtree_type;
}
bool rtree::rtree_type_valid() const {
	return m_rtree_type_valid;
}

node rtree::get_root() const { return m_root; }

bool rtree::has_root() const {
	return m_has_root;
}

uint32_t rtree::n_nodes_subtree(node u) const {
	assert(has_node(u));
	return m_size_subtrees[u];
}

bool rtree::need_recalc_size_subtrees() const {
	return m_recalc_size_subtrees;
}

vector<edge> rtree::get_edges_subtree(node r, bool relab) const {
	// if the tree does not have edges, return an empty list.
	if (n_nodes() <= 1) { return vector<edge>(); }

	assert(has_node(r));

	const auto n = n_nodes();

	// parent of node 'r'
	bool r_parent_set = false;
	node r_parent = n;

	BFS<rtree> bfs(*this);

	// -----------------------
	// find parent of node r
	if (r != get_root()) {
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
	}

	// -----------------------------
	// retrieve edges of the subtree

	// reset the bfs
	bfs.reset();
	bfs.set_use_rev_edges(true);

	// stop the bfs from going further than 'r''s parent
	// in case such parent exists
	if (r_parent_set) {
		bfs.set_visited(r_parent);
	}

	// data structures for node relabelling
	vector<node> labels(n_nodes(), n);
	// we need node 'r' to be relabelled to 0.
	labels[r] = 0;
	node next_label = 1;

	// retrieve edges and relabel them at the same time
	vector<edge> es;
	// relabel nodes
	if (relab) {
		bfs.set_process_neighbour(
		[&](const auto&, node s, node t, bool ltr) -> void {
			// change the orientation of the edge whenever appropriate
			// ltr: true  ---> "s->t"
			// ltr: false ---> "t->s"
			if (not ltr) { std::swap(s,t); }

			edge e;
			// relabel first node
			if (labels[s] == n) {
				labels[s] = next_label;
				++next_label;
			}
			e.first = labels[s];
			// relabel second node
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
		[&](const auto&, node s, node t, bool dir) -> void {
			// change the orientation of the edge whenever appropriate
			// dir: true  ---> "s->t"
			// dir: false ---> "t->s"
			if (not dir) { std::swap(s,t); }
			es.push_back(edge(s,t));
		}
		);
	}
	// start the bfs again, this time at 'r'
	bfs.start_at(r);
	return es;
}

rtree rtree::get_subtree(node r) const {
	// if the tree does not have edges, return a copy.
	if (n_nodes() <= 1) { return *this; }

	assert(has_root());
	assert(is_tree());
	assert(has_node(r));

	// retrieve the list of edges with their nodes relabelled
	const vector<edge> es = get_edges_subtree(r, true);
	// number of nodes of subtree
	const uint32_t n_verts = static_cast<uint32_t>(es.size()) + 1;

	// make subtree
	rtree sub(n_verts);
	sub.set_root(0);
	sub.add_edges(es);
	return sub;
}

ftree rtree::to_undirected() const {
	return ftree(dgraph::to_undirected());
}

/* PROTECTED */

void rtree::_init(uint32_t n) {
	tree::tree_init(n);
	dgraph::_init(n);
	m_size_subtrees = vector<uint32_t>(n);
	m_rtree_type_valid = false;
	m_recalc_size_subtrees = true;
}

void rtree::_clear() {
	tree::tree_clear();
	dgraph::_clear();
	m_size_subtrees.clear();
	m_rtree_type_valid = false;
	m_recalc_size_subtrees = true;
}

} // -- namespace graphs
} // -- namespace lal
