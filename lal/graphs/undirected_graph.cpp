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

#include <lal/graphs/undirected_graph.hpp>

// C++ includes
#include <algorithm>
#include <cmath>
#if defined DEBUG
#include <cassert>
#endif
using namespace std;

// lal includes
#include <lal/properties/Q.hpp>
#include <lal/internal/sorting/bit_sort.hpp>

namespace lal {
namespace graphs {

/* CONSTRUCTORS */

undirected_graph::undirected_graph() : graph() { }
undirected_graph::undirected_graph(uint32_t n) {
	init(n);
}
undirected_graph::undirected_graph(const undirected_graph& u) : graph() {
	copy_full_undirected_graph(u);
}
undirected_graph::undirected_graph(undirected_graph&& u) {
	move_full_undirected_graph(std::move(u));
}
undirected_graph::~undirected_graph() { }

/* OPERATORS */

undirected_graph& undirected_graph::operator= (const undirected_graph& u) {
	copy_full_undirected_graph(u);
	return *this;
}
undirected_graph& undirected_graph::operator= (undirected_graph&& u) {
	move_full_undirected_graph(std::move(u));
	return *this;
}

/* MODIFIERS */

undirected_graph& undirected_graph::add_edge(
	node u, node v, bool to_norm, bool check_norm
)
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
			internal::bit_sort(nu.begin(), nu.end(), nu.size());
			internal::bit_sort(nv.begin(), nv.end(), nv.size());
		}
		else if (check_norm) {
			// Even though we are not asked to normalise the graph, it may be
			// so after the addition. This means we have to check whether the
			// graph is normalised.
			const size_t su = nu.size();
			const size_t sv = nv.size();
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

undirected_graph& undirected_graph::add_edge_bulk(node u, node v) {
#if defined DEBUG
	assert(not has_edge(u,v));
#endif

	m_adjacency_list[u].push_back(v);
	m_adjacency_list[v].push_back(u);
	++m_num_edges;
	return *this;
}

void undirected_graph::finish_bulk_add(bool to_norm, bool check_norm) {
	// normalise
	graph::normalise_after_add(to_norm, check_norm);
}

undirected_graph& undirected_graph::add_edges(
	const vector<edge>& edges, bool to_norm, bool check_norm
)
{
	for (const edge& e : edges) {
		const node u = e.first;
		const node v = e.second;
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

undirected_graph& undirected_graph::set_edges(
	const vector<edge>& edges, bool to_norm, bool check_norm
)
{
	{
	const uint32_t n = n_nodes();
	clear(); init(n);
	}

	for (const edge& e : edges) {
		const node u = e.first;
		const node v = e.second;
#if defined DEBUG
		assert(not has_edge(u,v));
#endif

		m_adjacency_list[u].push_back(v);
		m_adjacency_list[v].push_back(u);
	}
	m_num_edges = static_cast<uint32_t>(edges.size());

	graph::normalise_after_add(to_norm, check_norm);
	return *this;
}

undirected_graph& undirected_graph::remove_edge(
	node u, node v, bool norm, bool check_norm
)
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

undirected_graph& undirected_graph::remove_edges(
	const vector<edge>& edges, bool norm, bool check_norm
)
{
	for (const edge& e : edges) {
		const node u = e.first;
		const node v = e.second;
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

void undirected_graph::disjoint_union(const undirected_graph& g) {
	graph::__disjoint_union(g);
	// nothing else to do
}

/* SETTERS */

/* GETTERS */

vector<edge_pair> undirected_graph::Q() const {
	return graph::Q(properties::size_Q(*this));
}

bool undirected_graph::has_edge(node u, node v) const {
#if defined DEBUG
	assert(u != v);
	assert(has_node(u));
	assert(has_node(v));
#endif

	const neighbourhood& nu = m_adjacency_list[u];
	const neighbourhood& nv = m_adjacency_list[v];

	if (is_normalised() and min(nu.size(), nv.size()) >= 64) {
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

void undirected_graph::_init(uint32_t n) {
	graph::_init(n);
}

void undirected_graph::_clear() {
	graph::_clear();
}

void undirected_graph::copy_full_undirected_graph(const undirected_graph& u) {
	// copy parent class
	copy_full_graph(u);

	// copy this class' members
}

void undirected_graph::move_full_undirected_graph(undirected_graph&& u) {
	// move-assign parent class
	move_full_graph(std::move(u));

	// move-assign this class' members
}

/* PRIVATE */

void undirected_graph::remove_single_edge(
	node u, node v, neighbourhood& out_u, neighbourhood& in_v
)
{
	// it_v: pointer to node v in out_u
	// it_u: pointer to node u in in_v
	vector<node>::iterator it_v, it_u;

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
