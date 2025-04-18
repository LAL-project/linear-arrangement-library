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

#include <lal/graphs/directed_graph.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <algorithm>

// lal includes
#include <lal/iterators/E_iterator.hpp>
#include <lal/properties/Q.hpp>
#include <lal/detail/array.hpp>
#include <lal/detail/sorting/bit_sort.hpp>
#include <lal/detail/graphs/enumerate_sets.hpp>
#include <lal/detail/graphs/utils.hpp>
#include <lal/detail/macros/search.hpp>
#include <lal/detail/properties/connected_components_compute.hpp>

namespace lal {
namespace graphs {

/* MODIFIERS */

void directed_graph::normalize() noexcept
{
	detail::array<char> mem(get_num_nodes(), 0);

	for (node u = 0; u < get_num_nodes(); ++u) {
		neighbourhood& out_nu = m_adjacency_list[u];
		if (not std::is_sorted(out_nu.begin(), out_nu.end())) {
			detail::sorting::bit_sort_mem<node>(
				out_nu.begin(), out_nu.end(), out_nu.size(), mem.begin()
			);
		}
		neighbourhood& in_nu = m_in_adjacency_list[u];
		if (not std::is_sorted(in_nu.begin(), in_nu.end())) {
			detail::sorting::bit_sort_mem<node>(
				in_nu.begin(), in_nu.end(), in_nu.size(), mem.begin()
			);
		}
	}
	m_is_normalized = true;
}

bool directed_graph::check_normalized() noexcept
{
	if (not graph::check_normalized()) {
		return false;
	}

	// check that every adjacency list is sorted
	for (node u = 0; u < get_num_nodes(); ++u) {
		const neighbourhood& out_nu = m_adjacency_list[u];
		if (not std::is_sorted(out_nu.begin(), out_nu.end())) {
			// if some is not then the graph is not normalized
			m_is_normalized = false;
			return false;
		}

		const neighbourhood& in_nu = m_in_adjacency_list[u];
		if (not std::is_sorted(in_nu.begin(), in_nu.end())) {
			// if some is not then the graph is not normalized
			m_is_normalized = false;
			return false;
		}
	}
	// all adjacency lists are sorted so the graph is normalized
	m_is_normalized = true;
	return true;
}

directed_graph& directed_graph::remove_node(
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
	{
		auto it = m_in_adjacency_list.begin();
		std::advance(it, u);
		m_in_adjacency_list.erase(it);
	}

	// now, relabel
	const auto n = get_num_nodes();
	for (node v = 0; v < n; ++v) {
		for (node& w : m_adjacency_list[v]) {
			w -= (w > u);
		}
		for (node& w : m_in_adjacency_list[v]) {
			w -= (w > u);
		}
	}

	actions_after_remove_node(u);
	return *this;
}

directed_graph& directed_graph::add_edge(
	const node u, const node v, const bool to_norm, const bool check_norm
) noexcept
{
#if defined DEBUG
	assert(not has_edge(u, v));
#endif

	neighbourhood& out_u = m_adjacency_list[u];
	neighbourhood& in_v = m_in_adjacency_list[v];
	out_u.push_back(v);
	in_v.push_back(u);

	actions_after_add_edge(u, v);

	if (is_normalized()) {
		// the graph was normalized
		if (to_norm) {
			// keep it normalized
			detail::sorting::bit_sort<node>(
				out_u.begin(), out_u.end(), out_u.size()
			);

			detail::sorting::bit_sort<node>(
				in_v.begin(), in_v.end(), in_v.size()
			);
		}
		else if (check_norm) {
			// Even though we have not been asked to normalize the
			// graph, it may still be so... This means we have to
			// check whether the graph is still normalized. We might
			// be lucky....
			const std::size_t out_u_s = out_u.size();
			const bool out_u_norm =
				(out_u_s <= 1 ? m_is_normalized
							  : out_u[out_u_s - 2] < out_u[out_u_s - 1]);
			const std::size_t in_v_s = in_v.size();
			const bool in_v_norm =
				(in_v_s <= 1 ? m_is_normalized
							 : in_v[in_v_s - 2] < in_v[in_v_s - 1]);
			m_is_normalized = m_is_normalized and out_u_norm and in_v_norm;
			/*if (su > 1) {
				m_is_normalized = nu[su - 2] < nu[su - 1];
			}*/
		}
		else {
			m_is_normalized = false;
		}
	}
	else {
		// the graph was not normalized.
		normalize_after_edge_addition(to_norm, check_norm);
	}

	return *this;
}

directed_graph&
directed_graph::add_edge_bulk(const node u, const node v) noexcept
{
#if defined DEBUG
	assert(not has_edge(u, v));
#endif

	m_adjacency_list[u].push_back(v);
	m_in_adjacency_list[v].push_back(u);
	++m_num_edges;
	return *this;
}

void directed_graph::finish_bulk_add(const bool to_norm, const bool check_norm)
	noexcept
{
	actions_after_add_edges_bulk();
	normalize_after_edge_addition(to_norm, check_norm);
}

directed_graph& directed_graph::add_edges(
	const std::vector<edge>& edges, const bool to_norm, const bool check_norm
) noexcept
{
	for (const auto& [u, v] : edges) {
#if defined DEBUG
		assert(not has_edge(u, v));
#endif

		m_adjacency_list[u].push_back(v);
		m_in_adjacency_list[v].push_back(u);
	}

	actions_after_add_edges(edges);
	normalize_after_edge_addition(to_norm, check_norm);
	return *this;
}

directed_graph& directed_graph::set_edges(
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
		m_in_adjacency_list[v].push_back(u);
	}

	m_num_edges = edges.size();

	normalize_after_edge_addition(to_norm, check_norm);
	return *this;
}

directed_graph&
directed_graph::remove_edge_bulk(const node u, const node v) noexcept
{
#if defined DEBUG
	assert(has_edge(u, v));
#endif

	neighbourhood& out_u = m_adjacency_list[u];
	neighbourhood& in_v = m_in_adjacency_list[v];
	remove_single_edge(u, v, out_u, in_v);

	--m_num_edges;
	return *this;
}

void directed_graph::finish_bulk_remove(
	const bool to_norm, const bool check_norm
) noexcept
{
	actions_after_remove_edges_bulk();
	normalize_after_edge_addition(to_norm, check_norm);
}

directed_graph& directed_graph::remove_edge(
	const node u, const node v, const bool norm, const bool check_norm
) noexcept
{
#if defined DEBUG
	assert(has_edge(u, v));
#endif

	neighbourhood& out_u = m_adjacency_list[u];
	neighbourhood& in_v = m_in_adjacency_list[v];
	remove_single_edge(u, v, out_u, in_v);

	actions_after_remove_edge(u, v);
	normalize_after_edge_removal(norm, check_norm);
	return *this;
}

directed_graph& directed_graph::remove_edges(
	const std::vector<edge>& edges, const bool norm, const bool check_norm
) noexcept
{
	for (const auto& [u, v] : edges) {
#if defined DEBUG
		assert(has_edge(u, v));
#endif
		neighbourhood& out_u = m_adjacency_list[u];
		neighbourhood& in_v = m_in_adjacency_list[v];
		remove_single_edge(u, v, out_u, in_v);
	}

	actions_after_remove_edges(edges);
	normalize_after_edge_removal(norm, check_norm);
	return *this;
}

directed_graph& directed_graph::remove_edges_incident_to(
	const node u, const bool norm, const bool check_norm
) noexcept
{
#if defined DEBUG
	assert(has_node(u));
#endif

	actions_before_remove_edges_incident_to(u);

	auto& in_neighs_u = m_in_adjacency_list[u];
	auto& out_neighs_u = m_adjacency_list[u];

	// the graph is NORMALISED
	if (is_normalized()) {
		// find the vertices that point to 'u'
		for (node v : in_neighs_u) {
			auto& out_v = m_adjacency_list[v];

			const auto it_u = std::lower_bound(out_v.begin(), out_v.end(), u);
#if defined DEBUG
			// check that the iterator points to the correct value
			assert(*it_u == u);
#endif
			out_v.erase(it_u);
		}

		// find the vertices pointed by 'u'
		for (node v : out_neighs_u) {
			auto& in_v = m_in_adjacency_list[v];

			const auto it_u =
				detail::find_sorted(in_v.begin(), in_v.end(), in_v.size(), u);
#if defined DEBUG
			// check that the iterator points to the correct value
			assert(*it_u == u);
#endif
			in_v.erase(it_u);
		}
	}
	// the graph is NOT NORMALISED
	else {
		// find the vertices that point to 'u'
		for (node v : in_neighs_u) {
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

		// find the vertices pointed by 'u'
		for (node v : out_neighs_u) {
			auto& in_v = m_in_adjacency_list[v];

			const auto it_u = std::find(in_v.begin(), in_v.end(), u);
#if defined DEBUG
			// check that the iterator points to the correct value
			assert(*it_u == u);
#endif
			in_v.erase(it_u);
		}
	}

	m_num_edges -= get_out_degree(u) + get_in_degree(u);
	out_neighs_u.clear();
	in_neighs_u.clear();

	normalize_after_edge_removal(norm, check_norm);
	return *this;
}

directed_graph& directed_graph::disjoint_union(const directed_graph& g) noexcept
{
	// this call updates the out-neighbors adjacency list,
	// as well as the number of edges and the graph's normalisation
	_disjoint_union(g);

	// update the neighbors adjacency list
	detail::append_adjacency_lists(m_adjacency_list, g.m_adjacency_list);
	detail::append_adjacency_lists(m_in_adjacency_list, g.m_in_adjacency_list);

	return *this;
}

/* SETTERS */

/* GETTERS */

std::vector<edge_pair> directed_graph::get_Q() const noexcept
{
	const auto qs = properties::num_pairs_independent_edges(*this);
	return detail::set_pairs_independent_edges(*this, qs);
}

std::vector<edge> directed_graph::get_edges() const noexcept
{
	return detail::set_edges(*this);
}

bool directed_graph::has_edge(const node u, const node v) const noexcept
{
#if defined DEBUG
	assert(u != v);
	assert(has_node(u));
	assert(has_node(v));
#endif

	const neighbourhood& out_u = m_adjacency_list[u];
	const neighbourhood& in_v = m_in_adjacency_list[v];

	if (is_normalized()) {
		return out_u.size() <= in_v.size()
				   ? detail::exists_sorted(
						 out_u.begin(), out_u.end(), out_u.size(), v
					 )
				   : detail::exists_sorted(
						 in_v.begin(), in_v.end(), in_v.size(), u
					 );
	}
	return out_u.size() <= in_v.size()
			   ? std::find(out_u.begin(), out_u.end(), v) != out_u.end()
			   : std::find(in_v.begin(), in_v.end(), u) != in_v.end();
}

undirected_graph
directed_graph::to_undirected(const bool norm, const bool check) const noexcept
{
	undirected_graph g(get_num_nodes());

	// add edges so that none are repeated
	for (iterators::E_iterator e_it(*this); not e_it.end(); e_it.next()) {
		const auto [u, v] = e_it.get_edge();
		if (not g.has_edge(u, v)) {
			g.add_edge_bulk(u, v);
		}
	}

	g.finish_bulk_add(norm, check);
	return g;
}

std::vector<directed_graph>
directed_graph::get_connected_components() const noexcept
{
	return detail::connected_components<false>(*this);
}

/* PROTECTED */

/* PRIVATE */

void directed_graph::remove_single_edge(
	const node u, const node v, neighbourhood& out_u, neighbourhood& in_v
) noexcept
{
	neighbourhood::iterator it_out_u, it_in_v;

	// find the nodes in the lists
	if (is_normalized()) {
		it_out_u =
			detail::find_sorted(out_u.begin(), out_u.end(), out_u.size(), v);
		it_in_v = detail::find_sorted(in_v.begin(), in_v.end(), in_v.size(), u);
	}
	else {
		it_out_u = std::find(out_u.begin(), out_u.end(), v);
		it_in_v = std::find(in_v.begin(), in_v.end(), u);
	}

#if defined DEBUG
	// check that the iterators are not past the end of their containers
	assert(it_out_u != out_u.end());
	assert(it_in_v != in_v.end());
	// check that the iterators point to the correct value
	assert(*it_out_u == v);
	assert(*it_in_v == u);
#endif

	// remove edge from the lists
	out_u.erase(it_out_u);
	in_v.erase(it_in_v);
}

} // namespace graphs
} // namespace lal
