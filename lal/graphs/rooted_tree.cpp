/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
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
#include <lal/internal/graphs/retrieve_subtrees.hpp>
#include <lal/internal/graphs/size_subtrees.hpp>
#include <lal/internal/graphs/tree_classification.hpp>
#include <lal/internal/graphs/union_find.hpp>
#include <lal/internal/graphs/conversions.hpp>

namespace lal {
namespace graphs {

/* MODIFIERS */

rooted_tree& rooted_tree::add_edge
(node u, node v, bool norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(can_add_edge(u,v));
#endif
	directed_graph::add_edge(u,v, norm, check_norm);

	// There is no need to invalidate
	//    m_valid_orientation = false;
	//    m_are_size_subtrees_valid = false;
	// since these attributes start at false and cannot be calculated if the
	// graph is not an actual tree. These can be calculated when the graph is
	// full. After that, this method can only be called after calling any
	// of the "remove edge" family.

	return *this;
}

rooted_tree& rooted_tree::add_edge_bulk(node u, node v) noexcept {
#if defined DEBUG
	assert(can_add_edge(u,v));
#endif
	directed_graph::add_edge_bulk(u,v);

	// There is no need to invalidate
	//    m_valid_orientation = false;
	//    m_are_size_subtrees_valid = false;
	// since these attributes start at false and cannot be calculated if the
	// graph is not an actual tree. These can be calculated when the graph is
	// full. After that, this method can only be called after calling any
	// of the "remove edge" family.

	return *this;
}

void rooted_tree::finish_bulk_add(bool norm, bool check) noexcept {
#if defined DEBUG
	assert(is_tree());
#endif
	directed_graph::finish_bulk_add(norm, check);
	fill_union_find();

	// There is no need to invalidate
	//    m_valid_orientation = false;
	//    m_are_size_subtrees_valid = false;
	// since these attributes start at false and cannot be calculated if the
	// graph is not an actual tree. These can be calculated when the graph is
	// full. After that, this method can only be called after calling any
	// of the "remove edge" family.
}

rooted_tree& rooted_tree::add_edges
(const vector<edge>& edges, bool norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(can_add_edges(edges));
#endif
	directed_graph::add_edges(edges, norm, check_norm);

	// There is no need to invalidate
	//    m_valid_orientation = false;
	//    m_are_size_subtrees_valid = false;
	// since these attributes start at false and cannot be calculated if the
	// graph is not an actual tree. These can be calculated when the graph is
	// full. After that, this method can only be called after calling any
	// of the "remove edge" family.

	return *this;
}

rooted_tree& rooted_tree::set_edges
(const vector<edge>& edges, bool to_norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(edges.size() == get_num_nodes() - 1);
	assert(can_add_edges(edges));
#endif
	directed_graph::set_edges(edges, to_norm, check_norm);

	// There is no need to invalidate
	//    m_are_size_subtrees_valid = false;
	// since these attributes start at false and cannot be calculated if the
	// graph is not an actual tree. These can be calculated when the graph is
	// full. After that, this method can only be called after calling any
	// of the "remove edge" family.

	// find the tree's root
	optional<node> R;
	for (node u = 0; u < get_num_nodes(); ++u) {
		if (get_in_degree(u) == 0) {
			// there can only be one root vertex
#if defined DEBUG
			assert(not R);
#endif
			R = u;
		}
	}
	set_root(*R);

	find_edge_orientation();
#if defined DEBUG
	assert(is_orientation_valid());
#endif

	tree_only_extra_work_edges_set();
	return *this;
}

rooted_tree& rooted_tree::remove_edge
(node u, node v, bool norm, bool check_norm) noexcept
{
	directed_graph::remove_edge(u,v, norm, check_norm);
	m_valid_orientation = false;
	m_are_size_subtrees_valid = false;
	return *this;
}

rooted_tree& rooted_tree::remove_edges
(const std::vector<edge>& edges, bool norm, bool check_norm) noexcept
{
	directed_graph::remove_edges(edges, norm, check_norm);
	m_valid_orientation = false;
	m_are_size_subtrees_valid = false;
	return *this;
}

rooted_tree& rooted_tree::remove_edges_incident_to
(node u, bool norm, bool check_norm)
noexcept
{
#if defined DEBUG
	assert(has_node(u));
#endif

	internal::UnionFind_update_roots_before_remove_all_incident_to(
		*this, u, &m_root_of[0], &m_root_size[0]
	);

	directed_graph::remove_edges_incident_to(u, norm, check_norm);
	return *this;
}

void rooted_tree::disjoint_union
(const rooted_tree& t, bool connect_roots) noexcept
{
	const uint32_t prev_n = get_num_nodes();
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
	for (node u = prev_n; u < get_num_nodes(); ++u) {
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

		if (are_size_subtrees_valid() and t.are_size_subtrees_valid()) {
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

bool rooted_tree::find_edge_orientation() noexcept {
#if defined DEBUG
	assert(is_tree());
	assert(has_root());
#endif

	// assign arborescence type to trees of 1 vertex
	if (get_num_nodes() == 1) {
		// the out-degree of the root is equal to and so it
		// would be assumed that it is not an arborescence
		m_valid_orientation = true;
		return m_valid_orientation;
	}

	// First case: the tree is NOT an anti-arborescence.
	// Do a BFS from the root. Make sure that all leaves
	// can be reached. If so, the tree is an arborescence.
	if (get_out_degree(get_root()) > 0) {
		internal::BFS<rooted_tree> bfs(*this);
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

void rooted_tree::init_rooted(const free_tree& _t, node r) noexcept {
	const uint32_t n = _t.get_num_nodes();
#if defined DEBUG
	assert(_t.is_tree());
#endif

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
	vector<edge> dir_edges(_t.get_num_edges());
	auto it_dir_edges = dir_edges.begin();

	// Build list of directed edges using a breadth-first search.
	// This is needed to make the edges point in the direction
	// indicated by the rooted tree type.
	internal::BFS<free_tree> bfs(_t);
	bfs.set_process_neighbour(
	[&](const auto&, const node s, const node t, bool) -> void {
		// the tree is an arborescence, i.e., the
		// edges point away from the root
		*it_dir_edges++ = edge(s,t);
	}
	);
	bfs.start_at(r);

	// allocate rooted tree
	rooted_tree::_init(n);
	// fill rooted tree
	rooted_tree::set_edges(dir_edges);
}

void rooted_tree::calculate_size_subtrees() noexcept {
#if defined DEBUG
	assert(is_rooted_tree());
#endif
	m_are_size_subtrees_valid = true;
	internal::get_size_subtrees(*this, get_root(), &m_size_subtrees[0]);
}

void rooted_tree::calculate_tree_type() noexcept {
	m_is_tree_type_valid = true;
	internal::classify_tree(*this, m_tree_type);
}

/* SETTERS */

void rooted_tree::set_root(node r) noexcept {
	// if the tree is empty simply consider it has a root...
	// although it really doesn't

	if (get_num_nodes() > 0) {
#if defined DEBUG
		assert(has_node(r));
#endif
		m_root = r;
	}
	m_has_root = true;
	m_are_size_subtrees_valid = false;
	m_valid_orientation = false;
	m_is_tree_type_valid = false;
}

/* GETTERS */

vector<edge> rooted_tree::get_edges_subtree(node u, bool relab) const noexcept {
	const auto subtree_info =
		internal::get_edges_subtree<false>(*this, u, relab);

#if defined DEBUG
	assert(subtree_info.second == nullptr);
#endif

	// move, please
	return std::move(subtree_info.first);
}

rooted_tree rooted_tree::get_subtree(node u) const noexcept {
	// if the tree does not have edges, return a copy.
	if (get_num_nodes() <= 1) { return *this; }

#if defined DEBUG
	assert(is_rooted_tree());
	assert(has_node(u));
#endif

	// retrieve the list of edges with their nodes relabelled
	const auto [es, subsizes] =
		internal::get_edges_subtree<true>(*this, u, true);

#if defined DEBUG
	assert(are_size_subtrees_valid() ? (subsizes != nullptr) : (subsizes == nullptr));
#endif

	// number of nodes of the subtree
	const uint32_t n_verts = static_cast<uint32_t>(es.size()) + 1;

	// make subtree
	rooted_tree sub(n_verts);
	sub.set_root(0);
	sub.m_valid_orientation = true;
	sub.set_edges(es);

	// if the subtree sizes are valid then copy them
	if (are_size_subtrees_valid()) {
		std::copy(
			&subsizes[0], &subsizes[n_verts],
			sub.m_size_subtrees.begin()
		);
		sub.m_are_size_subtrees_valid = true;

		delete[] subsizes;
	}

	return sub;
}

free_tree rooted_tree::to_free_tree(bool norm, bool check) const noexcept {
	free_tree t(get_num_nodes());

	for (iterators::E_iterator e_it(*this); not e_it.end(); e_it.next()) {
		const auto [u,v] = e_it.get_edge();
		t.add_edge_bulk(u, v);
	}

	t.finish_bulk_add(norm, check);
	return t;
}

head_vector rooted_tree::get_head_vector() const noexcept {
	return internal::from_tree_to_head_vector(*this);
}

/* PROTECTED */

void rooted_tree::_init(uint32_t n) noexcept {
	tree::tree_only_init(n);
	directed_graph::_init(n);
	m_size_subtrees = vector<uint32_t>(n);

	if (n <= 1) {
		set_root(0);
		set_valid_orientation(true);
	}
}

void rooted_tree::_clear() noexcept {
	tree::tree_only_clear();
	directed_graph::_clear();
	m_size_subtrees.clear();
}

void rooted_tree::call_union_find_after_add(
	node u, node v,
	uint32_t * const root_of,
	uint32_t * const root_size
) noexcept
{
	internal::UnionFind_update_roots_after_add(*this, u, v, root_of, root_size);
}
void rooted_tree::call_union_find_after_add(
	node u, node v,
	uint32_t * const root_of,
	uint32_t * const root_size
) const noexcept
{
	internal::UnionFind_update_roots_after_add(*this, u, v, root_of, root_size);
}

void rooted_tree::call_union_find_after_remove(
	node u, node v,
	uint32_t * const root_of,
	uint32_t * const root_size
) noexcept
{
	internal::UnionFind_update_roots_after_remove(*this, u, v, root_of, root_size);
}
void rooted_tree::call_union_find_after_remove(
	node u, node v,
	uint32_t * const root_of,
	uint32_t * const root_size
) const noexcept
{
	internal::UnionFind_update_roots_after_remove(*this, u, v, root_of, root_size);
}

void rooted_tree::copy_full_rooted_tree(const rooted_tree& r) noexcept {
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

void rooted_tree::move_full_rooted_tree(rooted_tree&& r) noexcept {
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
