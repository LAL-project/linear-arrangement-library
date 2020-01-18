/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Linear Arrangement Library.
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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

#include <lal/graphs/dgraph.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <algorithm>
#include <cmath>
#include <set>
using namespace std;

// lal includes
#include <lal/utils/sort_integers.hpp>
#include <lal/properties/Q.hpp>

namespace lal {
namespace graphs {

/* PUBLIC */

dgraph::dgraph() : graph() { }
dgraph::dgraph(uint32_t n) {
	init(n);
}
dgraph::~dgraph() { }

/* OPERATORS */

/* MODIFIERS */

void dgraph::normalise() {
	for (node u = 0; u < n_nodes(); ++u) {
		neighbourhood& out_nu = m_adjacency_list[u];
		if (not is_sorted(out_nu.begin(), out_nu.end())) {
			utils::sort_1_n_inc(out_nu.begin(), out_nu.end());
		}
		neighbourhood& in_nu = m_in_adjacency_list[u];
		if (not is_sorted(in_nu.begin(), in_nu.end())) {
			utils::sort_1_n_inc(in_nu.begin(), in_nu.end());
		}
	}
	m_normalised = true;
}

bool dgraph::check_normalised() {
	if (not graph::check_normalised()) { return false; }

	// check that every adjacency list is sorted
	for (node u = 0; u < n_nodes(); ++u) {
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

dgraph& dgraph::add_edge(node u, node v, bool to_norm) {
	assert(not has_edge(u,v));
	assert(u != v);
	assert(has_node(u));
	assert(has_node(v));

	neighbourhood& out_u = m_adjacency_list[u];
	out_u.push_back(v);
	neighbourhood& in_v = m_in_adjacency_list[v];
	m_in_adjacency_list[v].push_back(u);
	++m_num_edges;

	if (m_normalised) {
		// the graph was normalised
		if (to_norm) {
			// keep it normalised
			utils::sort_1_n_inc(out_u.begin(), out_u.end());
			utils::sort_1_n_inc(in_v.begin(), in_v.end());
		}
		else {
			// Even though we have not been asked to normalise the
			// graph, it may still be so... This means we have to
			// check whether the graph is still normalised. We might
			// be lucky....
			const auto out_u_s = out_u.size();
			const bool out_u_norm =
				(out_u_s <= 1 ? m_normalised : out_u[out_u_s - 2] < out_u[out_u_s - 1]);
			const auto in_v_s = in_v.size();
			const bool in_v_norm =
				(in_v_s <= 1 ? m_normalised : in_v[in_v_s - 2] < in_v[in_v_s - 1]);
			m_normalised = m_normalised and out_u_norm and in_v_norm;
			/*if (su > 1) {
				m_normalised = nu[su - 2] < nu[su - 1];
			}*/
		}
	}
	else if (to_norm) {
		// the graph needs to be normalised,
		// from a non-normalised state
		normalise();
	}

	return *this;
}

dgraph& dgraph::add_edges(const std::vector<edge>& edges, bool to_norm) {
	for (const edge& e : edges) {
		const node u = e.first;
		const node v = e.second;
		assert(u != v);
		assert(not has_edge(u,v));

		m_adjacency_list[u].push_back(v);
		m_in_adjacency_list[v].push_back(u);
		++m_num_edges;
	}

	if (to_norm) {
		// normalise directly, it might save us time
		normalise();
	}
	else {
		// only check
		check_normalised();
	}

	return *this;
}

void dgraph::disjoint_union(const graph& g) {
	graph::disjoint_union(g);

	// if the call to 'graph::disjoint_union' did not
	// fail then graph 'g' is a directed graph
	const dgraph& dg = dynamic_cast<const dgraph&>(g);

	m_in_adjacency_list.insert(
		m_in_adjacency_list.end(),
		dg.m_in_adjacency_list.begin(), dg.m_in_adjacency_list.end()
	);
}

/* SETTERS */

/* GETTERS */

std::vector<edge_pair> dgraph::Q() const {
	return graph::Q(properties::size_Q(*this));
}

bool dgraph::has_edge(node u, node v) const {
	assert(has_node(u));
	assert(has_node(v));

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

bool dgraph::is_directed() const { return true; }
bool dgraph::is_undirected() const { return false; }

const neighbourhood& dgraph::get_neighbours(node u) const {
	return get_out_neighbours(u);
}

uint32_t dgraph::degree(node u) const {
	return out_degree(u);
}

const neighbourhood& dgraph::get_out_neighbours(node u) const {
	assert(has_node(u));
	return m_adjacency_list[u];
}
const neighbourhood& dgraph::get_in_neighbours(node u) const {
	assert(has_node(u));
	return m_in_adjacency_list[u];
}

uint32_t dgraph::out_degree(node u) const {
	assert(has_node(u));
	return static_cast<uint32_t>(m_adjacency_list[u].size());
}
uint32_t dgraph::in_degree(node u) const {
	assert(has_node(u));
	return static_cast<uint32_t>(m_in_adjacency_list[u].size());
}

ugraph dgraph::to_undirected() const {
	ugraph g(n_nodes());
	g.add_edges(edges());
	return g;
}

/* PROTECTED */

void dgraph::_init(uint32_t n) {
	graph::_init(n);
	m_in_adjacency_list = vector<neighbourhood>(n);
}

void dgraph::_clear() {
	graph::_clear();
	m_in_adjacency_list.clear();
}

/* PRIVATE */

} // -- namespace graphs
} // -- namespace lal

