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

#include <lal/graphs/rooted_tree.hpp>

// C++ includes
#include <cassert>
#include <vector>
using namespace std;

// lal includes
#include <lal/internal/graphs/traversal.hpp>
#include <lal/internal/graphs/trees/is_tree.hpp>
#include <lal/internal/graphs/trees/size_subtrees.hpp>

namespace lal {
using namespace internal;

namespace graphs {

//rtree::rtree() { }
rooted_tree::rooted_tree(uint32_t n) : directed_graph(n) {
	rooted_tree::_init(n);
}
rooted_tree::rooted_tree(const free_tree& t, node r, rooted_tree_type type) {
	init_rooted(t, r, type);
}
//rtree::~rtree() { }

/* MODIFIERS */

rooted_tree& rooted_tree::add_edge(node s, node t, bool norm, bool check_norm) {
#if defined DEBUG
	assert(can_add_edge(s,t));
#endif

	directed_graph::add_edge(s,t, norm, check_norm);
	return *this;
}

rooted_tree& rooted_tree::add_edges(const vector<edge>& edges, bool norm, bool check_norm) {
#if defined DEBUG
	assert(can_add_edges(edges));
#endif

	directed_graph::add_edges(edges, norm, check_norm);
	return *this;
}

rooted_tree& rooted_tree::remove_edge(node s, node t, bool norm, bool check_norm) {
	directed_graph::remove_edge(s,t, norm, check_norm);
	m_rtree_type_valid = false;
	m_need_recalc_size_subtrees = true;
	return *this;
}

rooted_tree& rooted_tree::remove_edges(const std::vector<edge>& edges, bool norm, bool check_norm) {
	directed_graph::remove_edges(edges, norm, check_norm);
	m_rtree_type_valid = false;
	m_need_recalc_size_subtrees = true;
	return *this;
}

void rooted_tree::disjoint_union(const rooted_tree& t, bool connect_roots) {
	const uint32_t prev_n = n_nodes();
	if (prev_n == 0) {
		*this = t;
		return;
	}

	// join trees
	directed_graph::disjoint_union(t);

	// connect the roots if necessary
	if (connect_roots) {
		const node this_r = get_root();
		const node t_r = prev_n + t.get_root();
		add_edge(this_r, t_r);
	}
	// If connect_roots is false then the graph is going to
	// lack an edge (until inserted by someone). Nothing to do.

	// - keep the tree's root (if any)
	// - size of subtrees need recalculating
	m_need_recalc_size_subtrees = true;
	// - do not change the type of rooted tree
}

bool rooted_tree::find_rooted_tree_type() {
	assert(is_tree());
	assert(has_root());

	// assign arborescence type to trees of 1 vertex
	if (n_nodes() == 1) {
		// the out-degree of the root is equal to and so it
		// would be assumed that it is not an arborescence
		set_rooted_tree_type(rooted_tree_type::arborescence);
		return true;
	}

	// First case: the tree is NOT an anti-arborescence.
	// Do a BFS from the root. Make sure that all leaves
	// can be reached. If so, the tree is an arborescence.
	if (out_degree(get_root()) > 0) {
		BFS<rooted_tree> bfs(*this);
		bfs.start_at(get_root());

		// if some node was not visited then the tree
		// will remain unclassified
		set_rooted_tree_type(
			bfs.all_visited() ?
			rooted_tree_type::arborescence : rooted_tree_type::none);
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
		set_rooted_tree_type(
			all_one ?
			rooted_tree_type::anti_arborescence : rooted_tree_type::none);
	}

	return m_rtree_type != rooted_tree_type::none;
}

void rooted_tree::init_rooted(const free_tree& _t, node r, rooted_tree_type arb) {
	const uint32_t n = _t.n_nodes();

	assert(_t.is_tree());
	assert(arb == rooted_tree_type::arborescence or arb == rooted_tree_type::anti_arborescence);

	if (n == 0) {
		rooted_tree::_init(0);
		set_root(0);
		set_rooted_tree_type(arb);
		return;
	}

	assert(_t.has_node(r));

	// list of directed edges out of 'g'
	vector<edge> dir_edges(_t.n_edges());
	auto it_dir_edges = dir_edges.begin();

	// Build list of directed edges using a breadth-first search.
	// This is needed to make the edges point in the direction
	// indicated by the rooted tree type.
	BFS<free_tree> bfs(_t);
	if (arb == rooted_tree_type::arborescence) {
		bfs.set_process_neighbour(
		[&](const auto&, const node s, const node t, bool) -> void {
			// the tree is an arborescence, i.e., the
			// edges point away from the root
			*it_dir_edges = edge(s,t);
			++it_dir_edges;
		}
		);
	}
	else {
		bfs.set_process_neighbour(
		[&](const auto&, const node s, const node t, bool) -> void {
			// the tree is an anti-arborescence, i.e., the
			// edges point towards the root
			*it_dir_edges = edge(t,s);
			++it_dir_edges;
		}
		);
	}
	bfs.start_at(r);

	// construct rooted tree
	rooted_tree::_init(n);

	// set root and add edges
	set_root(r);
	add_edges(dir_edges);

	// set directed tree type
	set_rooted_tree_type(arb);
}

void rooted_tree::calculate_size_subtrees() {
	assert(is_rooted_tree());

	m_need_recalc_size_subtrees = false;
	m_size_subtrees.resize(n_nodes(), 0);

	internal::get_size_subtrees(*this, get_root(), &m_size_subtrees[0]);
}

/* SETTERS */

void rooted_tree::set_root(node r) {
	// if the tree is empty simply consider it has a root...
	// although it really doesn't

	if (n_nodes() > 0) {
		assert(has_node(r));
		m_root = r;
	}
	m_has_root = true;
	m_need_recalc_size_subtrees = true;
	m_rtree_type_valid = false;
}

void rooted_tree::set_rooted_tree_type(const rooted_tree_type& type) {
	m_rtree_type = type;
	m_rtree_type_valid = true;
}

/* GETTERS */

bool rooted_tree::can_add_edge(node s, node t) const {
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
	directed_graph copy = *this;
	// add the edge
	copy.add_edge(s, t);
	// convert the directed graph to an undirected graph
	// and make sure that there are no loops in that
	return not internal::has_undirected_cycles(copy);
}

bool rooted_tree::can_add_edges(const std::vector<edge>& edges) const {
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
	directed_graph copy = *this;
	// add the edges
	copy.add_edges(edges);
	// convert the directed graph to an undirected graph
	// and make sure that there are no loops in that
	return not internal::has_undirected_cycles(copy);
}

bool rooted_tree::is_rooted() const { return true; }

bool rooted_tree::is_rooted_tree() const {
	return is_tree() and has_root() and rooted_tree_type_valid() and
	(get_rooted_tree_type() == rooted_tree_type::arborescence or
	 get_rooted_tree_type() == rooted_tree_type::anti_arborescence);
}

rooted_tree::rooted_tree_type rooted_tree::get_rooted_tree_type() const {
	assert(rooted_tree_type_valid());
	return m_rtree_type;
}
bool rooted_tree::rooted_tree_type_valid() const {
	return m_rtree_type_valid;
}

node rooted_tree::get_root() const {
	assert(has_root());
	return m_root;
}

bool rooted_tree::has_root() const {
	return m_has_root;
}

uint32_t rooted_tree::n_nodes_subtree(node u) const {
	assert(has_node(u));
	return m_size_subtrees[u];
}

bool rooted_tree::size_subtrees_valid() const {
	return m_need_recalc_size_subtrees;
}

vector<edge> rooted_tree::get_edges_subtree(node u, bool relab) const {
	// if the tree does not have edges, return an empty list.
	if (n_nodes() <= 1) { return vector<edge>(); }

	assert(is_rooted_tree());
	assert(has_node(u));

	const uint32_t n = n_nodes();
	const bool is_anti = get_rooted_tree_type() == rooted_tree_type::anti_arborescence;

	// parent of node 'u'
	bool u_parent_set = false;
	node u_parent = n;

	BFS<rooted_tree> bfs(*this);

	// -----------------------
	// find parent of node u

	if (u != get_root()) {
		bfs.set_use_rev_edges(is_anti);
		bfs.set_terminate( [&](const auto&, node) { return u_parent_set; } );
		bfs.set_process_neighbour(
		[&](const auto&, node s, node t, bool) -> void {
			if (t == u) {
				u_parent = s;
				u_parent_set = true;
			}
		}
		);
		bfs.start_at(get_root());
	}

	// -----------------------------
	// retrieve edges of the subtree

	// reset the bfs
	bfs.reset();
	bfs.set_use_rev_edges(is_anti);

	// stop the bfs from going further than 'r''s parent
	// in case such parent exists
	if (u_parent_set) {
		bfs.set_visited(u_parent);
	}

	// data structures for node relabelling
	vector<node> labels(n_nodes(), n);
	// we need node 'r' to be relabelled to 0.
	labels[u] = 0;
	node next_label = 1;

	// retrieve edges and relabel them at the same time
	vector<edge> es;
	// relabel nodes
	if (relab) {
		bfs.set_process_neighbour(
		[&](const auto&, node s, node t, bool dir) -> void {
			// change the orientation of the edge whenever appropriate
			// dir: true  ---> "s->t"
			// dir: false ---> "t->s"
			if (not dir) { std::swap(s,t); }

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
	// start the bfs again, this time at 'u'
	bfs.start_at(u);
	return es;
}

rooted_tree rooted_tree::get_subtree(node u) const {
	// if the tree does not have edges, return a copy.
	if (n_nodes() <= 1) { return *this; }

	assert(is_rooted_tree());
	assert(has_node(u));

	// retrieve the list of edges with their nodes relabelled
	const vector<edge> es = get_edges_subtree(u, true);
	// number of nodes of subtree
	const uint32_t n_verts = static_cast<uint32_t>(es.size()) + 1;

	// make subtree
	rooted_tree sub(n_verts);
	sub.set_root(0);
	sub.set_rooted_tree_type(rooted_tree_type::arborescence);
	sub.add_edges(es);
	return sub;
}

free_tree rooted_tree::to_undirected() const {
	return free_tree(directed_graph::to_undirected());
}

/* PROTECTED */

void rooted_tree::_init(uint32_t n) {
	tree::tree_init(n);
	directed_graph::_init(n);
	m_rtree_type_valid = false;
	m_need_recalc_size_subtrees = true;
}

void rooted_tree::_clear() {
	tree::tree_clear();
	directed_graph::_clear();
	m_size_subtrees.clear();
	m_rtree_type_valid = false;
	m_need_recalc_size_subtrees = true;
}

} // -- namespace graphs
} // -- namespace lal
