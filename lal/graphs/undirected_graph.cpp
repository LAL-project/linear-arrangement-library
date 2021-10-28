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

#include <lal/graphs/undirected_graph.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <algorithm>
#include <cmath>

// lal includes
#include <lal/properties/Q.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/detail/graphs/enumerate_sets.hpp>
#include <lal/detail/graphs/utils.hpp>
#include <lal/detail/sorting/bit_sort.hpp>

namespace lal {
namespace graphs {

/* MODIFIERS */

undirected_graph& undirected_graph::add_edge
(node u, node v, bool to_norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(not has_edge(u,v));
#endif

	neighbourhood& nu = m_adjacency_list[u];
	neighbourhood& nv = m_adjacency_list[v];
	nu.push_back(v);
	nv.push_back(u);

	extra_work_per_edge_add(u, v);

	if (is_normalised()) {
		// the graph was normalised
		if (to_norm) {
			// Keep it normalised. The attribute m_normalised need not be updated.
			detail::bit_sort<node>(nu.begin(), nu.end(), nu.size());
			detail::bit_sort<node>(nv.begin(), nv.end(), nv.size());
		}
		else if (check_norm) {
			// Even though we are not asked to normalise the graph, it may be
			// so after the addition. This means we have to check whether the
			// graph is normalised.
			const std::size_t su = nu.size();
			const std::size_t sv = nv.size();
			if (su > 1 and sv > 1) {
				m_normalised = nu[su - 2] < nu[su - 1] and nv[sv - 2] < nv[sv - 1];
			}
			else if (su > 1) {
				m_normalised = nu[su - 2] < nu[su - 1];
			}
			else if (sv > 1) {
				m_normalised = nv[sv - 2] < nv[sv - 1];
			}
		}
		else {
			// not 'to_norm' and not 'check_norm'
			m_normalised = false;
		}
	}
	else {
		// the graph was not normalised.
		normalise_after_add(to_norm, check_norm);
	}

	return *this;
}

undirected_graph& undirected_graph::add_edge_bulk(node u, node v) noexcept {
#if defined DEBUG
	assert(not has_edge(u,v));
#endif

	m_adjacency_list[u].push_back(v);
	m_adjacency_list[v].push_back(u);
	++m_num_edges;
	return *this;
}

void undirected_graph::finish_bulk_add(bool to_norm, bool check_norm) noexcept {
	// normalise
	graph::normalise_after_add(to_norm, check_norm);
}

undirected_graph& undirected_graph::add_edges
(const std::vector<edge>& edges, bool to_norm, bool check_norm) noexcept
{
	for (const auto& [u,v] : edges) {
#if defined DEBUG
		assert(not has_edge(u,v));
#endif

		m_adjacency_list[u].push_back(v);
		m_adjacency_list[v].push_back(u);
		extra_work_per_edge_add(u, v);
	}

	normalise_after_add(to_norm, check_norm);
	return *this;
}

undirected_graph& undirected_graph::set_edges
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
		m_adjacency_list[v].push_back(u);
	}
	m_num_edges = edges.size();

	graph::normalise_after_add(to_norm, check_norm);
	return *this;
}

undirected_graph& undirected_graph::remove_edge
(node u, node v, bool norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(has_edge(u,v));
#endif

	neighbourhood& nu = m_adjacency_list[u];
	neighbourhood& nv = m_adjacency_list[v];
	remove_single_edge(u,v, nu, nv);

	normalise_after_remove(norm, check_norm);
	return *this;
}

undirected_graph& undirected_graph::remove_edges
(const std::vector<edge>& edges, bool norm, bool check_norm) noexcept
{
	for (const auto& [u,v] : edges) {
#if defined DEBUG
		assert(has_edge(u,v));
#endif

		neighbourhood& nu = m_adjacency_list[u];
		neighbourhood& nv = m_adjacency_list[v];
		remove_single_edge(u,v, nu, nv);
	}

	normalise_after_remove(norm, check_norm);
	return *this;
}

undirected_graph& undirected_graph::remove_edges_incident_to
(node u, bool norm, bool check_norm) noexcept
{
#if defined DEBUG
	assert(has_node(u));
#endif

	auto& neighs_u = m_adjacency_list[u];

	// the graph is NORMALISED
	if (is_normalised()) {
		// find the vertices that point to 'u'
		for (node v : neighs_u) {
			auto& out_v = m_adjacency_list[v];

			auto it_u = lower_bound(out_v.begin(), out_v.end(), u);
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

			auto it_u = find(out_v.begin(), out_v.end(), u);
#if defined DEBUG
			// check that the iterator points to the correct value
			assert(*it_u == u);
#endif
			out_v.erase(it_u);
		}
	}

	m_num_edges -= get_degree(u);
	neighs_u.clear();

	graph::normalise_after_remove(norm, check_norm);
	return *this;
}

void undirected_graph::disjoint_union(const undirected_graph& g) noexcept {
	// updates the number of edges and other base-class related attributes
	graph::__disjoint_union(g);

	// update the adjacency list
	detail::append_adjacency_lists(m_adjacency_list, g.m_adjacency_list);
}

/* SETTERS */

/* GETTERS */

std::vector<edge_pair> undirected_graph::get_Q() const noexcept {
	const auto qs = properties::num_pairs_independent_edges(*this);
	return detail::Q(*this, qs);
}

std::vector<edge> undirected_graph::get_edges() const noexcept {
	return detail::E(*this);
}

bool undirected_graph::has_edge(node u, node v) const noexcept {
#if defined DEBUG
	assert(u != v);
	assert(has_node(u));
	assert(has_node(v));
#endif

	const neighbourhood& nu = m_adjacency_list[u];
	const neighbourhood& nv = m_adjacency_list[v];

	if (is_normalised() and std::min(nu.size(), nv.size()) >= 64) {
		return (nu.size() <= nv.size() ?
			binary_search(nu.begin(), nu.end(), v) :
			binary_search(nv.begin(), nv.end(), u)
		);
	}
	return (nu.size() <= nv.size() ?
		find(nu.begin(), nu.end(), v) != nu.end() :
		find(nv.begin(), nv.end(), u) != nv.end()
	);
}

/* PROTECTED */

/* PRIVATE */

void undirected_graph::remove_single_edge
(node u, node v, neighbourhood& out_u, neighbourhood& in_v) noexcept
{
	// it_v: pointer to node v in out_u
	// it_u: pointer to node u in in_v
	std::vector<node>::iterator it_v, it_u;

	// find the nodes in the lists
	if (is_normalised()) {
		it_v = lower_bound(out_u.begin(), out_u.end(), v);
		it_u = lower_bound(in_v.begin(), in_v.end(), u);

		// after removing this edge the normalisation does not change.
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

	// remove edges from the lists
	out_u.erase(it_v);
	in_v.erase(it_u);

	// do the extra work!
	extra_work_per_edge_remove(u, v);
}

} // -- namespace graphs
} // -- namespace lal
