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

#include <lal/graphs/undirected_graph.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <algorithm>

// lal includes
#include <lal/properties/Q.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/detail/graphs/enumerate_sets.hpp>
#include <lal/detail/graphs/utils.hpp>
#include <lal/detail/graphs/conversions.hpp>
#include <lal/detail/sorting/bit_sort.hpp>
#include <lal/detail/macros/search.hpp>
#include <lal/detail/properties/connected_components_compute.hpp>

namespace lal {
namespace graphs {

/* MODIFIERS */

undirected_graph& undirected_graph::remove_node(
	const node u, const bool norm, const bool check_norm
) noexcept
{
#if defined DEBUG
	assert(has_node(u));
#endif
	// ---------------------------------
	// remove every edge incident to 'u'
	remove_edges_incident_to(u, norm, check_norm);

	// ---------------------------------
	// relabel the vertices in the graph

	// remove the corresponding row in the adjacency matrix
	{
		auto it = m_adjacency_list.begin();
		std::advance(it, u);
		m_adjacency_list.erase(it);
	}

	// now, relabel
	const auto n = get_num_nodes();
	for (node v = 0; v < n; ++v) {
		for (node& w : m_adjacency_list[v]) {
			w -= (w > u);
		}
	}

	actions_after_remove_node(u);
	return *this;
}

undirected_graph& undirected_graph::add_edge(
	const node u, const node v, const bool to_norm, const bool check_norm
) noexcept
{
#if defined DEBUG
	assert(not has_edge(u, v));
#endif

	neighbourhood& nu = m_adjacency_list[u];
	neighbourhood& nv = m_adjacency_list[v];
	nu.push_back(v);
	nv.push_back(u);

	actions_after_add_edge(u, v);

	if (is_normalized()) {
		// the graph was normalized
		if (to_norm) {
			// Keep it normalized. The attribute m_is_normalized need not be updated.
			detail::sorting::bit_sort<node>(nu.begin(), nu.end(), nu.size());
			detail::sorting::bit_sort<node>(nv.begin(), nv.end(), nv.size());
		}
		else if (check_norm) {
			// Even though we are not asked to normalize the graph, it may be
			// so after the addition. This means we have to check whether the
			// graph is normalized.
			const std::size_t su = nu.size();
			const std::size_t sv = nv.size();
			if (su > 1 and sv > 1) {
				m_is_normalized =
					nu[su - 2] < nu[su - 1] and nv[sv - 2] < nv[sv - 1];
			}
			else if (su > 1) {
				m_is_normalized = nu[su - 2] < nu[su - 1];
			}
			else if (sv > 1) {
				m_is_normalized = nv[sv - 2] < nv[sv - 1];
			}
		}
		else {
			// not 'to_norm' and not 'check_norm'
			m_is_normalized = false;
		}
	}
	else {
		// the graph was not normalized.
		normalize_after_edge_addition(to_norm, check_norm);
	}

	return *this;
}

undirected_graph&
undirected_graph::add_edge_bulk(const node u, const node v) noexcept
{
#if defined DEBUG
	assert(not has_edge(u, v));
#endif

	m_adjacency_list[u].push_back(v);
	m_adjacency_list[v].push_back(u);
	++m_num_edges;
	return *this;
}

void undirected_graph::finish_bulk_add(
	const bool to_norm, const bool check_norm
) noexcept
{
	actions_after_add_edges_bulk();
	normalize_after_edge_addition(to_norm, check_norm);
}

undirected_graph& undirected_graph::add_edges(
	const std::vector<edge>& edges, const bool to_norm, const bool check_norm
) noexcept
{
	for (const auto& [u, v] : edges) {
#if defined DEBUG
		assert(not has_edge(u, v));
#endif

		m_adjacency_list[u].push_back(v);
		m_adjacency_list[v].push_back(u);
	}

	actions_after_add_edges(edges);
	normalize_after_edge_addition(to_norm, check_norm);
	return *this;
}

undirected_graph& undirected_graph::set_edges(
	const std::vector<edge>& edges, const bool to_norm, const bool check_norm
) noexcept
{
	{
		const uint64_t n = get_num_nodes();
		clear();
		init(n);
	}

	for (const auto& [u, v] : edges) {
#if defined DEBUG
		assert(not has_edge(u, v));
#endif

		m_adjacency_list[u].push_back(v);
		m_adjacency_list[v].push_back(u);
	}
	m_num_edges = edges.size();

	normalize_after_edge_addition(to_norm, check_norm);
	return *this;
}

undirected_graph& undirected_graph::remove_edge(
	const node u, const node v, const bool norm, const bool check_norm
) noexcept
{
#if defined DEBUG
	assert(has_edge(u, v));
#endif

	neighbourhood& nu = m_adjacency_list[u];
	neighbourhood& nv = m_adjacency_list[v];
	remove_single_edge(u, v, nu, nv);

	actions_after_remove_edge(u, v);
	normalize_after_edge_removal(norm, check_norm);
	return *this;
}

undirected_graph&
undirected_graph::remove_edge_bulk(const node u, const node v) noexcept
{
#if defined DEBUG
	assert(has_edge(u, v));
#endif

	neighbourhood& nu = m_adjacency_list[u];
	neighbourhood& nv = m_adjacency_list[v];
	remove_single_edge(u, v, nu, nv);

	--m_num_edges;
	return *this;
}

void undirected_graph::finish_bulk_remove(
	const bool to_norm, const bool check_norm
) noexcept
{
	actions_after_remove_edges_bulk();
	normalize_after_edge_addition(to_norm, check_norm);
}

undirected_graph& undirected_graph::remove_edges(
	const std::vector<edge>& edges, const bool norm, const bool check_norm
) noexcept
{
	for (const auto& [u, v] : edges) {
#if defined DEBUG
		assert(has_edge(u, v));
#endif

		neighbourhood& nu = m_adjacency_list[u];
		neighbourhood& nv = m_adjacency_list[v];
		remove_single_edge(u, v, nu, nv);
	}

	actions_after_remove_edges(edges);
	normalize_after_edge_removal(norm, check_norm);
	return *this;
}

undirected_graph& undirected_graph::remove_edges_incident_to(
	const node u, const bool norm, const bool check_norm
) noexcept
{
#if defined DEBUG
	assert(has_node(u));
#endif
	actions_before_remove_edges_incident_to(u);

	auto& neighs_u = m_adjacency_list[u];

	// the graph is NORMALISED
	if (is_normalized()) {
		// find the vertices that point to 'u'
		for (node v : neighs_u) {
			auto& out_v = m_adjacency_list[v];

			const auto it_u = detail::find_sorted(
				out_v.begin(), out_v.end(), out_v.size(), u
			);
#if defined DEBUG
			// check that the iterator points to the correct value
			assert(*it_u == u);
#endif
			out_v.erase(it_u);
		}
	}
	// the graph is NOT NORMALISED
	else {
		// find the vertices that point to 'u'
		for (node v : neighs_u) {
			auto& out_v = m_adjacency_list[v];

			const auto it_u = std::find(out_v.begin(), out_v.end(), u);
#if defined DEBUG
			// check that the iterator points to the correct value
			assert(*it_u == u);
#endif
			out_v.erase(it_u);
		}
	}

	m_num_edges -= get_degree(u);
	neighs_u.clear();

	normalize_after_edge_removal(norm, check_norm);
	return *this;
}

undirected_graph& undirected_graph::disjoint_union(const undirected_graph& g
) noexcept
{
	// updates the number of edges and other base-class related attributes
	_disjoint_union(g);

	// update the adjacency list
	detail::append_adjacency_lists(m_adjacency_list, g.m_adjacency_list);
	return *this;
}

/* SETTERS */

/* GETTERS */

std::vector<edge_pair> undirected_graph::get_Q() const noexcept
{
	const auto qs = properties::num_pairs_independent_edges(*this);
	return detail::set_pairs_independent_edges(*this, qs);
}

std::vector<edge> undirected_graph::get_edges() const noexcept
{
	return detail::set_edges(*this);
}

bool undirected_graph::has_edge(const node u, const node v) const noexcept
{
#if defined DEBUG
	assert(u != v);
	assert(has_node(u));
	assert(has_node(v));
#endif

	const neighbourhood& nu = m_adjacency_list[u];
	const neighbourhood& nv = m_adjacency_list[v];

	if (is_normalized()) {
		return nu.size() <= nv.size()
				   ? detail::exists_sorted(nu.begin(), nu.end(), nu.size(), v)
				   : detail::exists_sorted(nv.begin(), nv.end(), nv.size(), u);
	}
	return nu.size() <= nv.size()
			   ? std::find(nu.begin(), nu.end(), v) != nu.end()
			   : std::find(nv.begin(), nv.end(), u) != nv.end();
}

std::vector<undirected_graph>
undirected_graph::get_connected_components() const noexcept
{
	return detail::connected_components<false>(*this);
}

/* PROTECTED */

/* PRIVATE */

void undirected_graph::remove_single_edge(
	const node u, const node v, neighbourhood& out_u, neighbourhood& in_v
) noexcept
{
	// it_v: pointer to node v in out_u
	// it_u: pointer to node u in in_v
	neighbourhood::iterator it_v, it_u;

	// find the nodes in the lists
	if (is_normalized()) {
		it_v = detail::find_sorted(out_u.begin(), out_u.end(), out_u.size(), v);
		it_u = detail::find_sorted(in_v.begin(), in_v.end(), in_v.size(), u);

		// after removing this edge the normalisation does not change.
	}
	else {
		it_v = std::find(out_u.begin(), out_u.end(), v);
		it_u = std::find(in_v.begin(), in_v.end(), u);
	}

#if defined DEBUG
	// check that the iterators point to the correct value
	assert(*it_v == v);
	assert(*it_u == u);
#endif

	// remove edges from the lists
	out_u.erase(it_v);
	in_v.erase(it_u);
}

} // namespace graphs
} // namespace lal
