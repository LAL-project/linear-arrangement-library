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
#if defined DEBUG
#include <cassert>
#endif
#include <vector>
using namespace std;

// lal includes
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/graphs/traversal.hpp>
#include <lal/internal/graphs/trees/retrieve_subtree.hpp>
#include <lal/internal/graphs/trees/size_subtrees.hpp>
#include <lal/internal/graphs/trees/tree_classification.hpp>
#include <lal/internal/graphs/union_find.hpp>

namespace lal {
using namespace internal;

namespace graphs {

/* CONSTRUCTORS */

rooted_tree::rooted_tree() noexcept : tree(), directed_graph() { }
rooted_tree::rooted_tree(uint32_t n) noexcept {
	rooted_tree::_init(n);
}
rooted_tree::rooted_tree(const rooted_tree& r) noexcept : graph(), tree(), directed_graph() {
	copy_full_rooted_tree(r);
}
rooted_tree::rooted_tree(rooted_tree&& r) noexcept {
	move_full_rooted_tree(std::move(r));
}
rooted_tree::rooted_tree(const free_tree& t, node r) noexcept {
	rooted_tree::_init(t.n_nodes());
	init_rooted(t, r);
}
rooted_tree::~rooted_tree() noexcept { }

/* OPERATORS */

rooted_tree& rooted_tree::operator= (const rooted_tree& r) noexcept {
	copy_full_rooted_tree(r);
	return *this;
}

rooted_tree& rooted_tree::operator= (rooted_tree&& r) noexcept {
	move_full_rooted_tree(std::move(r));
	return *this;
}

/* MODIFIERS */

rooted_tree& rooted_tree::add_edge(
	node u, node v, bool norm, bool check_norm
)
{
#if defined DEBUG
	assert(can_add_edge(u,v));
#endif
	directed_graph::add_edge(u,v, norm, check_norm);
	return *this;
}

rooted_tree& rooted_tree::add_edge_bulk(node u, node v) {
#if defined DEBUG
	assert(can_add_edge(u,v));
#endif
	directed_graph::add_edge_bulk(u,v);
	return *this;
}

void rooted_tree::finish_bulk_add(bool norm, bool check) {
#if defined DEBUG
	assert(is_tree());
#endif
	directed_graph::finish_bulk_add(norm, check);
	fill_union_find();
}

rooted_tree& rooted_tree::add_edges(
	const vector<edge>& edges, bool norm, bool check_norm
)
{
#if defined DEBUG
	assert(can_add_edges(edges));
#endif
	directed_graph::add_edges(edges, norm, check_norm);
	return *this;
}

rooted_tree& rooted_tree::set_edges(
	const vector<edge>& edges, bool to_norm, bool check_norm
)
{
#if defined DEBUG
	assert(can_add_edges(edges));
#endif
	directed_graph::set_edges(edges, to_norm, check_norm);
	fill_union_find();
	return *this;
}

rooted_tree& rooted_tree::remove_edge(
	node u, node v, bool norm, bool check_norm
)
{
	directed_graph::remove_edge(u,v, norm, check_norm);
	m_valid_orientation = false;
	m_are_size_subtrees_valid = false;
	return *this;
}

rooted_tree& rooted_tree::remove_edges(
	const std::vector<edge>& edges, bool norm, bool check_norm
)
{
	directed_graph::remove_edges(edges, norm, check_norm);
	m_valid_orientation = false;
	m_are_size_subtrees_valid = false;
	return *this;
}

void rooted_tree::disjoint_union(
	const rooted_tree& t, bool connect_roots
)
{
	const uint32_t prev_n = n_nodes();
	if (prev_n == 0) {
		*this = t;
		return;
	}

#define append(A, B) A.insert(A.end(), B.begin(), B.end())

	// join trees
	directed_graph::disjoint_union(t);
	append(m_size_subtrees, t.m_size_subtrees);

	// update union-find (1/3)
	append(m_root_of, t.m_root_of);
	append(m_root_size, t.m_root_size);
	// update the labels of the vertices' root of the union find (2/3)
	for (node u = prev_n; u < n_nodes(); ++u) {
		m_root_of[u] += prev_n;
	}

	// connect the roots if told to do so
	if (connect_roots) {
#if defined DEBUG
		assert(has_root());
		assert(t.has_root());
#endif

		const node this_r = get_root();
		const node t_r = prev_n + t.get_root();

		if (size_subtrees_valid() and t.size_subtrees_valid()) {
			// update the size under the root
			m_size_subtrees[this_r] += m_size_subtrees[t_r];
			m_are_size_subtrees_valid = true;
		}
		else {
			m_are_size_subtrees_valid = false;
		}

		// this operation also updates the
		// union-find data structure (3/3)
		add_edge(this_r, t_r);
	}
	else {
		// if roots are not connected then
		// the sizes need to be recalculated
		m_are_size_subtrees_valid = false;
	}
}

bool rooted_tree::find_edge_orientation() {
#if defined DEBUG
	assert(is_tree());
	assert(has_root());
#endif

	// assign arborescence type to trees of 1 vertex
	if (n_nodes() == 1) {
		// the out-degree of the root is equal to and so it
		// would be assumed that it is not an arborescence
		m_valid_orientation = true;
		return m_valid_orientation;
	}

	// First case: the tree is NOT an anti-arborescence.
	// Do a BFS from the root. Make sure that all leaves
	// can be reached. If so, the tree is an arborescence.
	if (out_degree(get_root()) > 0) {
		BFS<rooted_tree> bfs(*this);
		bfs.start_at(get_root());

		// if some node was not visited then the tree
		// will remain unclassified
		set_valid_orientation(bfs.all_visited());
	}
	else {
		// Second case: the tree is NOT an arborescence.
		// In this case we do not consider the tree to be (validly) rooted
		m_valid_orientation = false;
	}

	return m_valid_orientation;
}

void rooted_tree::set_valid_orientation(bool v) {
	m_valid_orientation = v;
}

void rooted_tree::init_rooted(const free_tree& _t, node r) {
	const uint32_t n = _t.n_nodes();
#if defined DEBUG
	assert(_t.is_tree());
#endif
	m_valid_orientation = true;

	if (n == 0) {
		rooted_tree::_init(0);
		m_valid_orientation = true;
		set_root(0);
		return;
	}

#if defined DEBUG
	assert(_t.has_node(r));
#endif

	// list of directed edges out of 'g'
	vector<edge> dir_edges(_t.n_edges());
	auto it_dir_edges = dir_edges.begin();

	// Build list of directed edges using a breadth-first search.
	// This is needed to make the edges point in the direction
	// indicated by the rooted tree type.
	BFS<free_tree> bfs(_t);
	bfs.set_process_neighbour(
	[&](const auto&, const node s, const node t, bool) -> void {
		// the tree is an arborescence, i.e., the
		// edges point away from the root
		*it_dir_edges = edge(s,t);
		++it_dir_edges;
	}
	);
	bfs.start_at(r);

	// allocate rooted tree
	rooted_tree::_init(n);

	// set root, add edges, and set valid orientation
	set_root(r);
	m_valid_orientation = true;
	rooted_tree::set_edges(dir_edges);
	fill_union_find();
}

void rooted_tree::calculate_size_subtrees() {
#if defined DEBUG
	assert(is_rooted_tree());
#endif
	m_are_size_subtrees_valid = true;
	internal::get_size_subtrees(*this, get_root(), &m_size_subtrees[0]);
}

void rooted_tree::calculate_tree_type() {
	internal::classify_tree(*this, m_tree_type);
}

/* SETTERS */

void rooted_tree::set_root(node r) {
	// if the tree is empty simply consider it has a root...
	// although it really doesn't

	if (n_nodes() > 0) {
#if defined DEBUG
		assert(has_node(r));
#endif
		m_root = r;
	}
	m_has_root = true;
	m_are_size_subtrees_valid = false;
	m_valid_orientation = false;
}

/* GETTERS */

vector<edge> rooted_tree::get_edges_subtree(node u, bool relab) const {
	const auto subtree_info = internal::get_edges_subtree(*this, u, relab, false);
#if defined DEBUG
	assert(subtree_info.second == nullptr);
#endif
	// move, please
	return subtree_info.first;
}

rooted_tree rooted_tree::get_subtree(node u) const {
	// if the tree does not have edges, return a copy.
	if (n_nodes() <= 1) { return *this; }

#if defined DEBUG
	assert(is_rooted_tree());
	assert(has_node(u));
#endif

	// retrieve the list of edges with their nodes relabelled
	const auto [es, subsizes] = internal::get_edges_subtree(*this, u, true, true);
#if defined DEBUG
	assert(size_subtrees_valid() ? (subsizes != nullptr) : (subsizes == nullptr));
#endif

	// number of nodes of the subtree
	const uint32_t n_verts = static_cast<uint32_t>(es.size()) + 1;

	// make subtree
	rooted_tree sub(n_verts);
	sub.set_root(0);
	sub.m_valid_orientation = true;
	sub.set_edges(es);

	// if the subtree sizes are valid then copy them
	if (size_subtrees_valid()) {
		sub.m_size_subtrees = vector<uint32_t>(n_verts, 0);
		std::copy(
			&subsizes[0], &subsizes[n_verts],
			sub.m_size_subtrees.begin()
		);
		sub.m_are_size_subtrees_valid = true;

		delete[] subsizes;
	}

	return sub;
}

free_tree rooted_tree::to_undirected(bool norm, bool check) const {
	free_tree t(n_nodes());

	iterators::E_iterator E_it(*this);
	while (E_it.has_next()) {
		E_it.next();
		const auto [u,v] = E_it.get_edge();
		t.add_edge_bulk(u, v);
	}

	t.finish_bulk_add(norm, check);
	return t;
}

/* PROTECTED */

void rooted_tree::_init(uint32_t n) {
	tree::tree_only_init(n);
	directed_graph::_init(n);
	m_size_subtrees = vector<uint32_t>(n);
}

void rooted_tree::_clear() {
	tree::tree_only_clear();
	directed_graph::_clear();
	m_size_subtrees.clear();
}

void rooted_tree::call_union_find_add(
	node u, node v,
	uint32_t *root_of,
	uint32_t *root_size
) noexcept
{
	internal::UnionFind_update_roots_add(*this, u, v, root_of, root_size);
}
void rooted_tree::call_union_find_add(
	node u, node v,
	uint32_t *root_of,
	uint32_t *root_size
) const noexcept
{
	internal::UnionFind_update_roots_add(*this, u, v, root_of, root_size);
}

void rooted_tree::call_union_find_remove(
	node u, node v,
	uint32_t *root_of,
	uint32_t *root_size
) noexcept
{
	internal::UnionFind_update_roots_remove(*this, u, v, root_of, root_size);
}
void rooted_tree::call_union_find_remove(
	node u, node v,
	uint32_t *root_of,
	uint32_t *root_size
) const noexcept
{
	internal::UnionFind_update_roots_remove(*this, u, v, root_of, root_size);
}

void rooted_tree::copy_full_rooted_tree(const rooted_tree& r) {
	// copy directed_graph class
	copy_full_directed_graph(r);

	// copy only tree's members
	tree_only_copy(r);

	// copy this class' members
	m_root = r.m_root;
	m_has_root = r.m_has_root;
	m_valid_orientation = r.m_valid_orientation;
	m_size_subtrees = r.m_size_subtrees;
	m_are_size_subtrees_valid = r.m_are_size_subtrees_valid;
}

void rooted_tree::move_full_rooted_tree(rooted_tree&& r) {
	// move-assign directed_graph class
	move_full_directed_graph(std::move(r));

	// move-assign only tree's members
	tree_only_move(std::move(r));

	// move this class' members
	m_root = r.m_root;
	m_has_root = r.m_has_root;
	m_valid_orientation = r.m_valid_orientation;
	m_size_subtrees = std::move(r.m_size_subtrees);
	m_are_size_subtrees_valid = r.m_are_size_subtrees_valid;

	r.m_root = 0;
	r.m_has_root = false;
	r.m_valid_orientation = false;
	r.m_are_size_subtrees_valid = false;
}

} // -- namespace graphs
} // -- namespace lal
