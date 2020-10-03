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

#include <lal/graphs/tree.hpp>

// C++ includes
#include <cassert>
using namespace std;

// lal includes
#include <lal/internal/graphs/traversal.hpp>

namespace lal {
namespace graphs {

/* NON-MEMBER FUNCTIONS */

void __update_roots_add(
	const tree& t, node u, node v,
	vector<node>& root_of, vector<uint32_t>& root_size
)
{
	// 'u' and 'v' are not connected, so they belong to
	// (different) connected components of the tree.

	// 'parent' and 'child' determine a direction to be used later.
	// 'new_root' is the new root for one of the vertices
	node parent, child, new_root;

	const node root_u = root_of[u];
	const node root_v = root_of[v];

	const uint32_t size_u = root_size[root_u];
	const uint32_t size_v = root_size[root_v];
	const uint32_t new_size = size_u + size_v;

	if (size_u < size_v) {
		root_of[root_u] = root_v;
		root_of[u] = root_v;

		new_root = root_v;
		root_size[new_root] = new_size;

		// update roots in the direction of v -> u
		parent = v; child = u;
	}
	else {
		root_of[root_v] = root_u;
		root_of[v] = root_u;

		new_root = root_u;
		root_size[new_root] = new_size;

		// update roots in the direction of u -> v
		parent = u; child = v;
	}

	// update roots of the smaller component,
	// in the direction parent -> child
	internal::BFS<tree> bfs(t);
	bfs.set_use_rev_edges(t.is_directed());
	bfs.set_process_current(
	[&](const auto&, node w) -> void { root_of[w] = new_root; }
	);
	bfs.set_visited(parent); // avoid going backwards
	bfs.start_at(child);
}

void __update_roots_remove(
	const tree& t, node u, node v,
	vector<node>& root_of, vector<uint32_t>& root_size
)
{
	// 'u' and 'v' are connected
	assert(root_of[u] == root_of[v]);

	const uint32_t size_uv = root_size[root_of[u]];

	internal::BFS<tree> bfs(t);

	// --- update u's info ---

	// Update the root of the vertices reachable from 'u'.
	//   (also calculate the size of u's component)
	uint32_t size_u = 0;
	bfs.set_use_rev_edges(t.is_directed());
	bfs.set_process_current(
	[&](const auto&, node w) -> void { root_of[w] = u; ++size_u; }
	);
	bfs.start_at(u);
	root_of[u] = u;
	root_size[u] = size_u;

	// --- update v's info ---

	// Update the root of the vertices reachable from 'v'.
	//   (there is no need to reset the BFS object)
	bfs.set_process_current(
	[&](const auto&, node w) -> void { root_of[w] = v; }
	);
	bfs.start_at(v);
	root_of[v] = v;
	root_size[v] = size_uv - size_u;
}

/* MEMBER FUNCTIONS */

//tree::tree() { }
//tree::~tree() { }

bool tree::is_tree() const {
	// NOTE: this would not really be true if the addition of edges
	// was not constrained. Since it is, in a way that no cycles can
	// be produced, then we only need to check for the number of edges.
	return (n_nodes() == 0 ? true : n_edges() == n_nodes() - 1);

	// NOTE 2: this is only true in a debug compilation of the library
	// since a release compilation does not actually constrain the addition
}

/* GETTERS */

bool tree::can_add_edge(node u, node v) const {
	assert(has_node(u));
	assert(has_node(v));

	const uint32_t n = n_nodes();
	const uint32_t m = n_edges();

	// in a tree we must have m <= n - 1
	if (m + 1 > n - 1) {
		return false;
	}

	// if m_labels[s] == m_labels[t] then
	// either the edge exists or there
	// exists a path from 's' to 't'
	return m_root_of[u] != m_root_of[v];
}

bool tree::can_add_edges(const std::vector<edge>& edges) const {
	const uint32_t n = n_nodes();
	const uint32_t m = n_edges();
	const uint64_t more_m = edges.size();

	// in a tree we must have m <= n - 1
	if (m + more_m > n - 1) {
		return false;
	}

	vector<uint32_t> _root_of = m_root_of;
	vector<uint32_t> _size_of = m_root_size;

	for (const edge& e : edges) {
		const node u = e.first;
		const node v = e.second;
		assert(has_node(u));
		assert(has_node(v));

		// if m_labels[s] == m_labels[t] then
		// either the edge exists or there
		// exists a path from 's' to 't'
		if (_root_of[u] == _root_of[v]) { return false; }
		__update_roots_add(*this, u, v, _root_of, _size_of);
	}
	return true;
}

/* PROTECTED */

void tree::tree_init(uint32_t n) {
	m_root_of = vector<uint32_t>(n, n);
	m_root_size = vector<uint32_t>(n, 0);
	for (node u = 0; u < n; ++u) {
		m_root_of[u] = u;
		m_root_size[u] = 1;
	}
}

void tree::tree_clear() {
	m_root_of.clear();
	m_root_size.clear();
}

void tree::extra_work_per_edge_add(node u, node v) {
	__update_roots_add(*this, u, v, m_root_of, m_root_size);
}
void tree::extra_work_per_edge_remove(node u, node v) {
	__update_roots_remove(*this, u, v, m_root_of, m_root_size);
}

} // -- namespace graphs
} // -- namespace lal
