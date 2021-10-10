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

#include <lal/graphs/directed_graph.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <algorithm>
#include <cmath>

// lal includes
#include <lal/internal/data_array.hpp>
#include <lal/internal/sorting/bit_sort.hpp>
#include <lal/internal/graphs/enumerate_sets.hpp>
#include <lal/internal/graphs/utils.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/properties/Q.hpp>

namespace lal {
namespace graphs {

/* MODIFIERS */

void directed_graph::normalise() noexcept {
	internal::data_array<char> mem(get_num_nodes(), 0);

	for (node u = 0; u < get_num_nodes(); ++u) {
		neighbourhood& out_nu = m_adjacency_list[u];
		if (not is_sorted(out_nu.begin(), out_nu.end())) {
			internal::bit_sort_mem<node>
			(out_nu.begin(), out_nu.end(), out_nu.size(), mem.data());
		}
		neighbourhood& in_nu = m_in_adjacency_list[u];
		if (not is_sorted(in_nu.begin(), in_nu.end())) {
			internal::bit_sort_mem<node>
			(in_nu.begin(), in_nu.end(), in_nu.size(), mem.data());
		}
	}
	m_normalised = true;
}

bool directed_graph::check_normalised() noexcept {
	if (not graph::check_normalised()) { return false; }

	// check that every adjacency list is sorted
	for (node u = 0; u < get_num_nodes(); ++u) {
		const neighbourhood& out_nu = m_adjacency_list[u];
		if (not is_sorted(out_nu.begin(), out_nu.end())) {
			// if some is not then the graph is not normalised
			m_normalised = false;
			return false;
		}

		const neighbourhood& in_nu = m_in_adjacency_list[u];
		if (not is_sorted(in_nu.begin(), in_nu.end())) {
			// if some is not then the graph is not normalised
			m_normalised = false;
			return false;
		}
	}
	// all adjacency lists are sorted so
	// the graph is normalised
	m_normalised = true;
	return true;
}

directed_graph& directed_graph::add_edge
(node u, node v, bool to_norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(not has_edge(u,v));
#endif

	neighbourhood& out_u = m_adjacency_list[u];
	neighbourhood& in_v = m_in_adjacency_list[v];
	out_u.push_back(v);
	in_v.push_back(u);

	extra_work_per_edge_add(u, v);

	if (is_normalised()) {
		// the graph was normalised
		if (to_norm) {
			// keep it normalised
			internal::bit_sort<node>
			(out_u.begin(), out_u.end(), out_u.size());

			internal::bit_sort<node>
			(in_v.begin(), in_v.end(), in_v.size());
		}
		else if (check_norm) {
			// Even though we have not been asked to normalise the
			// graph, it may still be so... This means we have to
			// check whether the graph is still normalised. We might
			// be lucky....
			const size_t out_u_s = out_u.size();
			const bool out_u_norm =
				(out_u_s <= 1 ? m_normalised : out_u[out_u_s - 2] < out_u[out_u_s - 1]);
			const size_t in_v_s = in_v.size();
			const bool in_v_norm =
				(in_v_s <= 1 ? m_normalised : in_v[in_v_s - 2] < in_v[in_v_s - 1]);
			m_normalised = m_normalised and out_u_norm and in_v_norm;
			/*if (su > 1) {
				m_normalised = nu[su - 2] < nu[su - 1];
			}*/
		}
		else {
			m_normalised = false;
		}
	}
	else {
		// the graph was not normalised.
		graph::normalise_after_add(to_norm, check_norm);
	}

	return *this;
}

directed_graph& directed_graph::add_edge_bulk(node u, node v) noexcept {
#if defined DEBUG
	assert(not has_edge(u,v));
#endif

	m_adjacency_list[u].push_back(v);
	m_in_adjacency_list[v].push_back(u);
	++m_num_edges;
	return *this;
}

void directed_graph::finish_bulk_add(bool to_norm, bool check_norm) noexcept {
	// normalise
	graph::normalise_after_add(to_norm, check_norm);
}

directed_graph& directed_graph::add_edges
(const std::vector<edge>& edges, bool to_norm, bool check_norm) noexcept
{
	for (const auto& [u,v] : edges) {
#if defined DEBUG
		assert(not has_edge(u,v));
#endif
		m_adjacency_list[u].push_back(v);
		m_in_adjacency_list[v].push_back(u);
		extra_work_per_edge_add(u, v);
	}
	graph::normalise_after_add(to_norm, check_norm);
	return *this;
}

directed_graph& directed_graph::set_edges
(const std::vector<edge>& edges, bool to_norm, bool check_norm) noexcept
{
	{
	const uint64_t n = get_num_nodes();
	clear(); init(n);
	}

	for (const auto& [u,v] : edges) {
#if defined DEBUG
		assert(not has_edge(u,v));
#endif
		m_adjacency_list[u].push_back(v);
		m_in_adjacency_list[v].push_back(u);
	}

	m_num_edges = edges.size();

	graph::normalise_after_add(to_norm, check_norm);
	return *this;
}

directed_graph& directed_graph::remove_edge
(node u, node v, bool norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(has_edge(u,v));
#endif

	neighbourhood& out_u = m_adjacency_list[u];
	neighbourhood& in_v = m_in_adjacency_list[v];
	remove_single_edge(u,v, out_u, in_v);

	graph::normalise_after_remove(norm, check_norm);
	return *this;
}

directed_graph& directed_graph::remove_edges
(const std::vector<edge>& edges, bool norm, bool check_norm) noexcept
{
	for (const auto& [u,v] : edges) {
#if defined DEBUG
		assert(has_edge(u,v));
#endif
		neighbourhood& out_u = m_adjacency_list[u];
		neighbourhood& in_v = m_in_adjacency_list[v];
		remove_single_edge(u,v, out_u, in_v);
	}

	graph::normalise_after_remove(norm, check_norm);
	return *this;
}

directed_graph& directed_graph::remove_edges_incident_to
(node u, bool norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(has_node(u));
#endif

	auto& in_neighs_u = m_in_adjacency_list[u];
	auto& out_neighs_u = m_adjacency_list[u];

	// the graph is NORMALISED
	if (is_normalised()) {
		// find the vertices that point to 'u'
		for (node v : in_neighs_u) {
			auto& out_v = m_adjacency_list[v];

			auto it_u = lower_bound(out_v.begin(), out_v.end(), u);
#if defined DEBUG
			// check that the iterator points to the correct value
			assert(*it_u == u);
#endif
			out_v.erase(it_u);
		}

		// find the vertices pointed by 'u'
		for (node v : out_neighs_u) {
			auto& in_v = m_in_adjacency_list[v];

			auto it_u = lower_bound(in_v.begin(), in_v.end(), u);
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

			auto it_u = find(out_v.begin(), out_v.end(), u);
#if defined DEBUG
			// check that the iterator points to the correct value
			assert(*it_u == u);
#endif
			out_v.erase(it_u);
		}

		// find the vertices pointed by 'u'
		for (node v : out_neighs_u) {
			auto& in_v = m_in_adjacency_list[v];

			auto it_u = find(in_v.begin(), in_v.end(), u);
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

	graph::normalise_after_remove(norm, check_norm);
	return *this;
}

void directed_graph::disjoint_union(const directed_graph& g) noexcept {
	// this call updates the out-neighbours adjacency list,
	// as well as the number of edges and the graph's normalisation
	graph::__disjoint_union(g);

	// update the neighbours adjacency list
	internal::append_adjacency_lists(m_adjacency_list, g.m_adjacency_list);
	internal::append_adjacency_lists(m_in_adjacency_list, g.m_in_adjacency_list);
}

/* SETTERS */

/* GETTERS */

std::vector<edge_pair> directed_graph::get_Q() const noexcept {
	const auto qs = properties::num_pairs_independent_edges(*this);
	return internal::Q(*this, qs);
}

std::vector<edge> directed_graph::get_edges() const noexcept {
	return internal::E(*this);
}

bool directed_graph::has_edge(node u, node v) const noexcept {
#if defined DEBUG
	assert(u != v);
	assert(has_node(u));
	assert(has_node(v));
#endif

	const neighbourhood& out_u = m_adjacency_list[u];
	const neighbourhood& in_v = m_in_adjacency_list[v];

	if (is_normalised() and std::min(out_u.size(), in_v.size()) >= 64) {
		return (out_u.size() <= in_v.size() ?
			binary_search(out_u.begin(), out_u.end(), v) :
			binary_search(in_v.begin(), in_v.end(), u)
		);
	}
	return (out_u.size() <= in_v.size() ?
		find(out_u.begin(), out_u.end(), v) != out_u.end() :
		find(in_v.begin(), in_v.end(), u) != in_v.end()
	);
}

undirected_graph directed_graph::to_undirected(bool norm, bool check) const noexcept {
	undirected_graph g(get_num_nodes());

	// add edges so that none are repeated
	for (iterators::E_iterator e_it(*this); not e_it.end(); e_it.next()) {
		const auto [u,v] = e_it.get_edge();
		if (not g.has_edge(u, v)) {
			g.add_edge_bulk(u, v);
		}
	}

	g.finish_bulk_add(norm, check);
	return g;
}

/* PROTECTED */

/* PRIVATE */

void directed_graph::remove_single_edge
(node u, node v, neighbourhood& out_u, neighbourhood& in_v) noexcept
{
	// it_v: pointer to node v in out_u
	// it_u: pointer to node u in in_v
	std::vector<node>::iterator it_v, it_u;

	// find the nodes in the lists
	if (is_normalised()) {
		it_v = lower_bound(out_u.begin(), out_u.end(), v);
		it_u = lower_bound(in_v.begin(), in_v.end(), u);
	}
	else {
		it_v = find(out_u.begin(), out_u.end(), v);
		it_u = find(in_v.begin(), in_v.end(), u);
	}

#if defined DEBUG
	// check that the iterators point to the correct value
	assert(*it_v == v);
	assert(*it_u == u);
#endif

	// remove edge from the lists
	out_u.erase(it_v);
	in_v.erase(it_u);

	// do the extra work!
	extra_work_per_edge_remove(u, v);
}

} // -- namespace graphs
} // -- namespace lal
