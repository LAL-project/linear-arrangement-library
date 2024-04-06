/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#include <lal/graphs/rooted_tree.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>

// lal includes
#include <lal/iterators/E_iterator.hpp>
#include <lal/detail/graphs/traversal.hpp>
#include <lal/detail/graphs/retrieve_subtrees.hpp>
#include <lal/detail/graphs/size_subtrees.hpp>
#include <lal/detail/graphs/tree_classification.hpp>
#include <lal/detail/graphs/union_find.hpp>
#include <lal/detail/graphs/conversions.hpp>
#include <lal/detail/arrangement_wrapper.hpp>

namespace lal {
namespace graphs {

/* MODIFIERS */

void rooted_tree::init_rooted
(const free_tree& t, node r, bool norm, bool check_norm)
noexcept
{
	const uint64_t n = t.get_num_nodes();
	rooted_tree::_clear();

#if defined DEBUG
	assert(t.is_tree());
#endif

	if (n == 0) {
		rooted_tree::_init(0);
		set_root(0);
		return;
	}

#if defined DEBUG
	assert(t.has_node(r));
#endif

	// allocate rooted tree
	directed_graph::_init(n);
	set_root(r);
	tree_only_copy(t);
	m_size_subtrees.resize(n);
	m_are_size_subtrees_valid = false;
	m_is_normalised = t.m_is_normalised;

	// pre-allocate memory
	for (node u = 0; u < n; ++u) {
		reserve_out_degree(u, t.get_degree(u) - (u != r));
		reserve_in_degree(u, u != r ? 1 : 0);
	}

	// Build list of directed edges using a breadth-first search traversal.
	// This is needed to make the edges point in the direction indicated by
	// the root.
	detail::BFS bfs(t);
	bfs.set_process_neighbour(
	[&](const auto&, const node u, const node v, bool) -> void {
		m_adjacency_list[u].push_back(v);
		m_in_adjacency_list[v].push_back(u);
	}
	);
	bfs.start_at(r);

	m_num_edges = n - 1;

	if (norm or check_norm) {
		normalise_after_edge_addition(norm, check_norm);
	}
}

void rooted_tree::init_rooted
(free_tree&& t, node r, bool norm, bool check_norm)
noexcept
{
	const uint64_t n = t.get_num_nodes();
	rooted_tree::_clear();

#if defined DEBUG
	assert(t.is_tree());
#endif

	if (n == 0) {
		rooted_tree::_init(0);
		set_root(0);
		return;
	}

#if defined DEBUG
	assert(t.has_node(r));
#endif

	const bool is_t_normalized = t.is_normalised();

	// allocate rooted tree
	m_adjacency_list = std::move(t.m_adjacency_list);
	m_in_adjacency_list.resize(n);
	set_root(r);
	tree_only_move(std::forward<tree>(t));
	m_size_subtrees.resize(n);
	m_are_size_subtrees_valid = false;
	m_is_normalised = t.m_is_normalised;
	m_num_edges = n - 1;
	t.m_num_edges = 0;

	// pre-allocate memory
	for (node u = 0; u < n; ++u) {
		reserve_in_degree(u, u != r ? 1 : 0);
	}

	// Build list of directed edges using a breadth-first search traversal.
	// This is needed to make the edges point in the direction indicated by
	// the root.
	detail::BFS bfs(*this);
	bfs.set_use_rev_edges(false);
	bfs.set_process_neighbour(
	[&](const auto&, const node u, const node v, bool) -> void {
		// add missing edges.
		m_in_adjacency_list[v].push_back(u);

		// remove 'u' from the list of 'v'
		neighbourhood::iterator it_u;
		neighbourhood& out_v = m_adjacency_list[v];
		if (is_t_normalized) {
			it_u = std::lower_bound(out_v.begin(), out_v.end(), u);
		}
		else {
			it_u = std::find(out_v.begin(), out_v.end(), u);
		}
		out_v.erase(it_u);
	}
	);
	bfs.start_at(r);

	if (norm or check_norm) {
		normalise_after_edge_addition(norm, check_norm);
	}
}

rooted_tree& rooted_tree::remove_node
(node u, bool connect, bool norm, bool check_norm)
noexcept
{
#if defined DEBUG
	assert(has_node(u));
#endif

	// !
	// Recall that, due to the rules of construction, node u cannot have more
	// than one parent.

	// the new edges, if any
	std::vector<edge> new_edges;
	if (connect and get_in_degree(u) == 1 and get_out_degree(u) > 0) {
		new_edges.reserve(get_out_degree(u));
		// parent of u exists
		const node parent_u = get_in_neighbours(u)[0];
		for (node v : get_out_neighbours(u)) {
			new_edges.push_back({parent_u - (parent_u > u), v - (v > u)});
		}
	}

	// 'action_after_node_remove' is called by the following method.
	// This method updates 'm_is_tree_type_valid', the union-find data
	// structure, ...
	directed_graph::remove_node(u, norm, check_norm);

	// update root's label, if any
	if (m_root.has_value()) {
		if (m_root == u) {
			// the root has been removed
			m_root = get_num_nodes();
		}
		else if (m_root > u) {
			--*m_root;
		}
	}

	// update vector of subtree sizes
	m_are_size_subtrees_valid = false;
	m_size_subtrees.resize(get_num_nodes());

	if (connect) {
		add_edges(new_edges, norm, check_norm);
	}

	return *this;
}

rooted_tree& rooted_tree::add_edge
(node u, node v, bool norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(can_add_edge(u,v));
#endif

	directed_graph::add_edge(u,v, norm, check_norm);

	return *this;
}

rooted_tree& rooted_tree::add_edge_bulk(node u, node v) noexcept {
#if defined DEBUG
	assert(can_add_edge(u,v));
#endif

	directed_graph::add_edge_bulk(u,v);
	return *this;
}

void rooted_tree::finish_bulk_add(bool norm, bool check) noexcept {
	directed_graph::finish_bulk_add(norm, check);
}

void rooted_tree::finish_bulk_add_complete(bool norm, bool check) noexcept {
#if defined DEBUG
	assert(is_tree());
#endif

	directed_graph::finish_bulk_add(norm, check);
	tree_only_actions_after_add_edges_bulk_complete();
}

rooted_tree& rooted_tree::add_edges
(const std::vector<edge>& edges, bool norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(can_add_edges(edges));
#endif

	directed_graph::add_edges(edges, norm, check_norm);
	return *this;
}

rooted_tree& rooted_tree::set_edges
(const std::vector<edge>& edges, bool to_norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(edges.size() == get_num_nodes() - 1);
	assert(can_add_edges(edges));
#endif
	directed_graph::set_edges(edges, to_norm, check_norm);

	// find the tree's root
	node R = get_num_nodes() + 1;
	for (node u = 0; u < get_num_nodes(); ++u) {
		if (get_in_degree(u) == 0) {
			// there can only be one root vertex
#if defined DEBUG
			assert(R > get_num_nodes());
#endif
			R = u;
		}
	}
	set_root(R);

	tree_only_set_edges();

	// have been set:
	//    m_is_tree_type_valid
	m_are_size_subtrees_valid = false;

	return *this;
}

rooted_tree& rooted_tree::remove_edge
(node s, node t, bool norm, bool check_norm) noexcept
{
	directed_graph::remove_edge(s,t, norm, check_norm);
	m_are_size_subtrees_valid = false;
	return *this;
}

rooted_tree& rooted_tree::remove_edges
(const std::vector<edge>& edges, bool norm, bool check_norm)
noexcept
{
	directed_graph::remove_edges(edges, norm, check_norm);
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

	m_are_size_subtrees_valid = false;
	directed_graph::remove_edges_incident_to(u, norm, check_norm);
#if defined DEBUG
	assert(m_union_find__root_of[u] == u);
	assert(m_union_find__root_size[u] == 1);
#endif
	return *this;
}

void rooted_tree::disjoint_union
(const rooted_tree& t, bool connect_roots) noexcept
{
	const uint64_t prev_n = get_num_nodes();
	if (prev_n == 0) {
		*this = t;
		return;
	}

#define append(A, B) A.insert(A.end(), B.begin(), B.end())

	// join trees
	directed_graph::disjoint_union(t);
	append(m_size_subtrees, t.m_size_subtrees);

	// update union-find (1/3)
	append(m_union_find__root_of, t.m_union_find__root_of);
	append(m_union_find__root_size, t.m_union_find__root_size);
	// update the labels of the vertices' root of the union find (2/3)
	for (node u = prev_n; u < get_num_nodes(); ++u) {
		m_union_find__root_of[u] += prev_n;
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
		// if roots are not connected then the sizes need to be recalculated
		m_are_size_subtrees_valid = false;
	}

	// the type of tree always needs to be recalculated
	m_is_tree_type_valid = false;

#undef append
}

void rooted_tree::calculate_size_subtrees() noexcept {
#if defined DEBUG
	assert(is_rooted_tree());
#endif
	m_are_size_subtrees_valid = true;
	detail::get_size_subtrees(*this, get_root(), &m_size_subtrees[0]);
}

void rooted_tree::calculate_tree_type() noexcept {
	m_is_tree_type_valid = true;
	detail::classify_tree(*this, m_tree_type);
}

/* SETTERS */

/* GETTERS */

bool rooted_tree::can_add_edge(node u, node v) const noexcept {
	return get_in_degree(v) == 0 and tree::can_add_edge(u, v);
}

bool rooted_tree::can_add_edges(const std::vector<edge>& edges) const noexcept {
	// this function ensures there are no cycles in the tree
	if (not tree::can_add_edges(edges)) { return false; }

	const auto n = get_num_nodes();

	// check the in-degrees (no need to initialize the array here)
	detail::data_array<char> in_degrees(n);
	for (node u = 0; u < n; ++u) {
		// the in-degree is either 0 or 1.
		in_degrees[u] = static_cast<char>(get_in_degree(u));
	}

	// check the indegrees of the vertices...
	for (const auto& [u,v] : edges) {
		in_degrees[v] += 1;
		if (in_degrees[v] > 1) { return false; }
	}

	return true;
}

std::vector<edge> rooted_tree::get_edges_subtree(node u, bool relab) const noexcept {
	const auto subtree_info =
		detail::get_edges_subtree<false>(*this, u, relab);

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
		detail::get_edges_subtree<true>(*this, u, true);

#if defined DEBUG
	assert(are_size_subtrees_valid() ? (subsizes != nullptr) : (subsizes == nullptr));
#endif

	// number of nodes of the subtree
	const uint64_t n_verts = es.size() + 1;

	// make subtree
	rooted_tree sub(n_verts);
	sub.set_root(0);
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
		const auto [u, v] = e_it.get_edge();
		t.add_edge_bulk(u, v);
	}
	
	t.finish_bulk_add(norm, check);
	return t;
}

head_vector rooted_tree::get_head_vector(const linear_arrangement& arr) const
	noexcept
{
	return
		(arr.size() == 0 ?
			detail::from_tree_to_head_vector
				(*this, detail::identity_arr(arr))
			:
			detail::from_tree_to_head_vector
				(*this, detail::nonidentity_arr(arr))
		 );
}

bool rooted_tree::subtree_contains_node(node r, node u) const noexcept {
#if defined DEBUG
	assert(has_node(u));
	assert(has_node(r));
	assert(is_rooted_tree());
#endif

	if (r == get_root()) { return true; }
	if (r == u) { return true; }

	detail::BFS bfs(*this);

	// terminate the BFS traversal as soon as node 'u' has been reached.
	bfs.set_terminate
	([u](const auto&, node current) -> bool { return current == u; });
	// do not search backwards
	bfs.set_use_rev_edges(false);
	// start at the root
	bfs.start_at(r);

	return bfs.node_was_visited(u);
}

/* PROTECTED */

void rooted_tree::actions_after_add_edge(node u, node v) noexcept {
	directed_graph::actions_after_add_edge(u, v);
	tree_only_actions_after_add_edge(u, v);
}

void rooted_tree::actions_after_add_edges(const edge_list& e) noexcept {
	directed_graph::actions_after_add_edges(e);
	tree_only_actions_after_add_edges(e);
}

void rooted_tree::actions_after_add_edges_bulk() noexcept {
	directed_graph::actions_after_add_edges_bulk();
	tree_only_actions_after_add_edges_bulk();
}

void rooted_tree::actions_after_remove_edge(node u, node v) noexcept {
	directed_graph::actions_after_remove_edge(u, v);
	tree_only_actions_after_remove_edge(u, v);
}

void rooted_tree::actions_after_remove_edges(const edge_list& e) noexcept {
	directed_graph::actions_after_remove_edges(e);
	tree_only_actions_after_remove_edges(e);
}

void rooted_tree::actions_before_remove_edges_incident_to(node u) noexcept {
	directed_graph::actions_before_remove_edges_incident_to(u);
	tree_only_actions_before_remove_edges_incident_to(u);
}

void rooted_tree::actions_after_remove_node(node u) noexcept {
	directed_graph::actions_after_remove_node(u);
	tree_only_actions_after_remove_node(u);
}

// -----------------------------------------------------------------------------

void rooted_tree::update_union_find_after_add_edge(
	node u, node v,
	uint64_t * const root_of,
	uint64_t * const root_size
)
const noexcept
{
	detail::update_unionfind_after_add_edge
		(*this, u, v, root_of, root_size);
}

void rooted_tree::update_union_find_after_add_edges(
	const edge_list& edges,
	uint64_t * const root_of, uint64_t * const root_size
)
const noexcept
{
	detail::update_unionfind_after_add_edges
		(*this, edges, root_of, root_size);
}

void rooted_tree::update_union_find_after_add_edges_bulk
(uint64_t * const root_of, uint64_t * const root_size)
const noexcept
{
	detail::update_unionfind_after_add_edges_bulk
		(*this, root_of, root_size);
}

void rooted_tree::update_union_find_after_remove_edge(
	node u, node v,
	uint64_t * const root_of,
	uint64_t * const root_size
)
const noexcept
{
	detail::update_unionfind_after_remove_edge
		(*this, u, v, root_of, root_size);
}

void rooted_tree::update_union_find_after_remove_edges(
	const edge_list& edges,
	uint64_t * const root_of, uint64_t * const root_size
)
const noexcept
{
	detail::update_unionfind_after_remove_edges
		(*this, edges, root_of, root_size);
}

void rooted_tree::update_union_find_before_remove_incident_edges_to(
	node u,
	uint64_t * const root_of, uint64_t * const root_size
)
const noexcept
{
	detail::update_unionfind_before_remove_edges_incident_to
		(*this, u, root_of, root_size);
}

} // -- namespace graphs
} // -- namespace lal
