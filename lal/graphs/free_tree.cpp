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

namespace lal {
namespace graphs {

/* CONSTRUCTORS */

free_tree::free_tree(const undirected_graph& t) noexcept : undirected_graph(t) {
#if defined DEBUG
	// check that the input graph is a tree
	assert(detail::is_graph_a_tree(t));
#endif

	free_tree::tree_only_init(t.get_num_nodes());
	// no need to call set_edges
	tree_only_extra_work_edges_set();
}

free_tree::free_tree(undirected_graph&& t) noexcept : undirected_graph(std::move(t)) {
#if defined DEBUG
	// check that the input graph is a tree
	assert(detail::is_graph_a_tree(*this));
#endif

	free_tree::tree_only_init(get_num_nodes());
	// no need to call set_edges
	tree_only_extra_work_edges_set();
}

/* MODIFIERS */

free_tree& free_tree::add_edge(node u, node v, bool norm, bool check_norm) noexcept {
#if defined DEBUG
	assert(can_add_edge(u,v));
#endif
	undirected_graph::add_edge(u,v, norm, check_norm);

	// There is no need to invalidate
	//    m_is_tree_type_valid = false;
	// since these attributes start at false and cannot be calculated if the
	// graph is not an actual tree. These can be calculated when the graph is
	// full. After that, this method can only be called after calling any
	// of the "remove edge" family.

	return *this;
}

free_tree& free_tree::add_edge_bulk(node u, node v) noexcept {
#if defined DEBUG
	assert(can_add_edge(u,v));
#endif
	undirected_graph::add_edge_bulk(u,v);

	// There is no need to invalidate
	//    m_is_tree_type_valid = false;
	// since these attributes start at false and cannot be calculated if the
	// graph is not an actual tree. These can be calculated when the graph is
	// full. After that, this method can only be called after calling any
	// of the "remove edge" family.

	return *this;
}

void free_tree::finish_bulk_add(bool norm, bool check) noexcept {
#if defined DEBUG
	assert(is_tree());
#endif
	undirected_graph::finish_bulk_add(norm, check);
	fill_union_find();

	// There is no need to invalidate
	//    m_is_tree_type_valid = false;
	// since these attributes start at false and cannot be calculated if the
	// graph is not an actual tree. These can be calculated when the graph is
	// full. After that, this method can only be called after calling any
	// of the "remove edge" family.
}

free_tree& free_tree::add_edges
(const std::vector<edge>& edges, bool norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(can_add_edges(edges));
#endif
	undirected_graph::add_edges(edges, norm, check_norm);

	// There is no need to invalidate
	//    m_is_tree_type_valid = false;
	// since these attributes start at false and cannot be calculated if the
	// graph is not an actual tree. These can be calculated when the graph is
	// full. After that, this method can only be called after calling any
	// of the "remove edge" family.

	return *this;
}

free_tree& free_tree::set_edges
(const std::vector<edge>& edges, bool to_norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(edges.size() == get_num_nodes() - 1);
	assert(can_add_edges(edges));
#endif
	undirected_graph::set_edges(edges, to_norm, check_norm);
	tree_only_extra_work_edges_set();
	return *this;
}

free_tree& free_tree::remove_edge
(node s, node t, bool norm, bool check_norm) noexcept
{
	undirected_graph::remove_edge(s, t, norm, check_norm);
	m_is_tree_type_valid =false;
	return *this;
}

free_tree& free_tree::remove_edges
(const std::vector<edge>& edges, bool norm, bool check_norm) noexcept
{
	undirected_graph::remove_edges(edges, norm, check_norm);
	m_is_tree_type_valid =false;
	return *this;
}

free_tree& free_tree::remove_edges_incident_to
(node u, bool norm, bool check_norm)
noexcept
{
#if defined DEBUG
	assert(has_node(u));
#endif

	m_is_tree_type_valid = false;

	detail::UnionFind_update_roots_before_remove_all_incident_to
		(*this, u, &m_root_of[0], &m_root_size[0]);

	undirected_graph::remove_edges_incident_to(u, norm, check_norm);
	return *this;
}

void free_tree::disjoint_union(const free_tree& t) noexcept {
	const node prev_n = get_num_nodes();
	if (prev_n == 0) {
		*this = t;
		return;
	}

	m_is_tree_type_valid = false;

	// tree 't' and tree 'this' do not have cycles, so the disjoint
	// union of both trees does not have cycles. However, the resulting
	// tree will lack an edge.
	undirected_graph::disjoint_union(t);

	// join union-find
#define vec_join(A, B) A.insert(A.end(), B.begin(), B.end())
	vec_join(m_root_of, t.m_root_of);
	vec_join(m_root_size, t.m_root_size);

	// update the labels of the vertices' root of the union find
	for (node u = prev_n; u < get_num_nodes(); ++u) {
		m_root_of[u] += prev_n;
	}
}

void free_tree::calculate_tree_type() noexcept {
	m_is_tree_type_valid = true;
	detail::classify_tree(*this, m_tree_type);
}

/* GETTERS */

head_vector free_tree::get_head_vector(node r) const noexcept {
#if defined DEBUG
	assert(has_node(r));
#endif
	return detail::from_tree_to_head_vector(*this, r);
}

/* PROTECTED */

void free_tree::call_union_find_after_add(
	node u, node v,
	uint64_t * const root_of,
	uint64_t * const root_size
) noexcept
{
	detail::UnionFind_update_roots_after_add(*this, u, v, root_of, root_size);
}
void free_tree::call_union_find_after_add(
	node u, node v,
	uint64_t * const root_of,
	uint64_t * const root_size
) const noexcept
{
	detail::UnionFind_update_roots_after_add(*this, u, v, root_of, root_size);
}

void free_tree::call_union_find_after_remove(
	node u, node v,
	uint64_t * const root_of,
	uint64_t * const root_size
) noexcept
{
	detail::UnionFind_update_roots_after_remove(*this, u, v, root_of, root_size);
}
void free_tree::call_union_find_after_remove(
	node u, node v,
	uint64_t * const root_of,
	uint64_t * const root_size
) const noexcept
{
	detail::UnionFind_update_roots_after_remove(*this, u, v, root_of, root_size);
}

} // -- namespace graphs
} // -- namespace lal
