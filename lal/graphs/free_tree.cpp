/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
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
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
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

#include <lal/graphs/free_tree.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/detail/graphs/is_tree.hpp>
#include <lal/detail/graphs/tree_classification.hpp>
#include <lal/detail/graphs/union_find.hpp>
#include <lal/detail/graphs/conversions.hpp>
#include <lal/detail/arrangement_wrapper.hpp>

namespace lal {
namespace graphs {

/* CONSTRUCTORS */

free_tree::free_tree(const undirected_graph& t) noexcept
	: undirected_graph(t)
{
#if defined DEBUG
	// check that the input graph is a tree
	assert(detail::is_graph_a_tree(t));
#endif

	free_tree::tree_only_init(t.get_num_nodes());
	// no need to call set_edges
	tree_only_set_edges();
}

free_tree::free_tree(undirected_graph&& t) noexcept
	: undirected_graph(std::forward<undirected_graph>(t))
{
#if defined DEBUG
	// check that the input graph is a tree
	assert(detail::is_graph_a_tree(*this));
#endif

	free_tree::tree_only_init(get_num_nodes());
	// no need to call set_edges
	tree_only_set_edges();
}

/* MODIFIERS */

free_tree& free_tree::remove_node(
	const node u, const bool norm, const bool check_norm
) noexcept
{
#if defined DEBUG
	assert(has_node(u));
#endif

	// note: method 'actions_after_node_remove' is called by the next method
	undirected_graph::remove_node(u, norm, check_norm);
	return *this;
}

free_tree& free_tree::add_edge(
	const node u, const node v, const bool norm, const bool check_norm
) noexcept
{
#if defined DEBUG
	assert(can_add_edge(u, v));
#endif

	undirected_graph::add_edge(u, v, norm, check_norm);
	return *this;
}

free_tree& free_tree::add_edge_bulk(const node u, const node v) noexcept
{
#if defined DEBUG
	assert(can_add_edge(u, v));
#endif

	undirected_graph::add_edge_bulk(u, v);
	return *this;
}

void free_tree::finish_bulk_add(const bool norm, const bool check) noexcept
{
	undirected_graph::finish_bulk_add(norm, check);
}

void free_tree::finish_bulk_add_complete(const bool norm, const bool check)
	noexcept
{
#if defined DEBUG
	assert(is_tree());
#endif

	undirected_graph::finish_bulk_add(norm, check);
	tree_only_actions_after_add_edges_bulk_complete();
}

free_tree& free_tree::add_edges(
	const std::vector<edge>& edges, const bool norm, const bool check_norm
) noexcept
{
#if defined DEBUG
	assert(can_add_edges(edges));
#endif
	// note: method 'actions_after_edge_add' is called for every edge
	// in the set 'edges' by the next method.
	undirected_graph::add_edges(edges, norm, check_norm);

	// There is no need to invalidate
	//    m_is_tree_type_valid = false;
	// since these attributes start at false and cannot be calculated if the
	// graph is not an actual tree. These can be calculated when the graph is
	// full. After that, this method can only be called after calling any
	// of the "remove edge" family.

	return *this;
}

free_tree& free_tree::set_edges(
	const std::vector<edge>& edges, const bool to_norm, const bool check_norm
) noexcept
{
#if defined DEBUG
	assert(edges.size() == get_num_nodes() - 1);
	assert(can_add_edges(edges));
#endif
	undirected_graph::set_edges(edges, to_norm, check_norm);
	tree_only_set_edges();
	return *this;
}

free_tree& free_tree::remove_edge(
	const node s, const node t, const bool norm, const bool check_norm
) noexcept
{
	// note: method 'actions_after_edge_remove' is called by the next method
	undirected_graph::remove_edge(s, t, norm, check_norm);
	return *this;
}

free_tree& free_tree::remove_edges(
	const std::vector<edge>& edges, const bool norm, const bool check_norm
) noexcept
{
	undirected_graph::remove_edges(edges, norm, check_norm);
	return *this;
}

free_tree& free_tree::remove_edge_bulk(const node u, const node v) noexcept
{
	undirected_graph::remove_edge_bulk(u, v);
	return *this;
}

void free_tree::finish_bulk_remove(const bool norm, const bool check) noexcept
{
	undirected_graph::finish_bulk_remove(norm, check);
}

void free_tree::finish_bulk_remove_complete(const bool norm, const bool check)
	noexcept
{
#if defined DEBUG
	assert(get_num_edges() == 0);
#endif

	undirected_graph::finish_bulk_remove(norm, check);
	tree_only_actions_after_remove_edges_bulk_complete();
}

free_tree& free_tree::remove_edges_incident_to(
	const node u, const bool norm, const bool check_norm
) noexcept
{
#if defined DEBUG
	assert(has_node(u));
#endif

	undirected_graph::remove_edges_incident_to(u, norm, check_norm);
#if defined DEBUG
	assert(m_union_find_root_of[u] == u);
	assert(m_union_find_root_size[u] == 1);
#endif
	return *this;
}

free_tree& free_tree::disjoint_union(const free_tree& t) noexcept
{
	const node prev_n = get_num_nodes();
	if (prev_n == 0) {
		*this = t;
		return *this;
	}

	m_is_tree_type_valid = false;

	// tree 't' and tree 'this' do not have cycles, so the disjoint
	// union of both trees does not have cycles. However, the resulting
	// tree will lack an edge.
	undirected_graph::disjoint_union(t);

	// join union-find
#define vec_join(A, B) A.insert(A.end(), B.begin(), B.end())
	vec_join(m_union_find_root_of, t.m_union_find_root_of);
	vec_join(m_union_find_root_size, t.m_union_find_root_size);

	// update the labels of the vertices' root of the union find
	for (node u = prev_n; u < get_num_nodes(); ++u) {
		m_union_find_root_of[u] += prev_n;
	}

	return *this;
}

void free_tree::calculate_tree_type() noexcept
{
	m_is_tree_type_valid = true;
	detail::classify_tree(*this, m_tree_type);
}

/* GETTERS */

head_vector free_tree::get_head_vector(
	const node r, const linear_arrangement& arr
) const noexcept
{
#if defined DEBUG
	assert(has_node(r));
#endif

	return (
		arr.size() == 0 ? detail::from_tree_to_head_vector(
							  *this, detail::identity_arr(arr), r
						  )
						: detail::from_tree_to_head_vector(
							  *this, detail::nonidentity_arr(arr), r
						  )
	);
}

/* PROTECTED */

void free_tree::update_union_find_after_add_edge(
	const node u,
	const node v,
	uint64_t * const root_of,
	uint64_t * const root_size
) const noexcept
{
	detail::update_unionfind_after_add_edge(*this, u, v, root_of, root_size);
}

void free_tree::update_union_find_after_add_edges(
	const edge_list& edges, uint64_t * const root_of, uint64_t * const root_size
) const noexcept
{
	detail::update_unionfind_after_add_edges(*this, edges, root_of, root_size);
}

void free_tree::update_union_find_after_add_edges_bulk(
	uint64_t * const root_of, uint64_t * const root_size
) const noexcept
{
	detail::update_unionfind_after_add_rem_edges_bulk(
		*this, root_of, root_size
	);
}

void free_tree::update_union_find_after_remove_edge(
	const node u,
	const node v,
	uint64_t * const root_of,
	uint64_t * const root_size
) const noexcept
{
	detail::update_unionfind_after_remove_edge(*this, u, v, root_of, root_size);
}

void free_tree::update_union_find_after_remove_edges(
	const edge_list& edges, uint64_t * const root_of, uint64_t * const root_size
) const noexcept
{
	detail::update_unionfind_after_remove_edges(
		*this, edges, root_of, root_size
	);
}

void free_tree::update_union_find_after_remove_edges_bulk(
	uint64_t * const root_of, uint64_t * const root_size
) const noexcept
{
	detail::update_unionfind_after_add_rem_edges_bulk(
		*this, root_of, root_size
	);
}

void free_tree::update_union_find_before_remove_incident_edges_to(
	const node u, uint64_t * const root_of, uint64_t * const root_size
) const noexcept
{
	detail::update_unionfind_before_remove_edges_incident_to(
		*this, u, root_of, root_size
	);
}

} // namespace graphs
} // namespace lal
