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

#include <lal/graphs/dgraph.hpp>

// C++ includes
#include <algorithm>
#include <cassert>
#include <cmath>
using namespace std;

// lal includes
#include <lal/utils/sorting/bit_sort.hpp>
#include <lal/properties/Q.hpp>

namespace lal {
namespace graphs {

/* PUBLIC */

dgraph::dgraph() { }
dgraph::dgraph(uint32_t n) {
	init(n);
}
dgraph::~dgraph() { }

/* OPERATORS */

/* MODIFIERS */

void dgraph::normalise() {
	vector<char> mem(n_nodes(), 0);

	for (node u = 0; u < n_nodes(); ++u) {
		neighbourhood& out_nu = m_adjacency_list[u];
		if (not std::is_sorted(out_nu.begin(), out_nu.end())) {
			utils::bit_sort_mem(out_nu.begin(), out_nu.end(), &mem[0]);
		}
		neighbourhood& in_nu = m_in_adjacency_list[u];
		if (not std::is_sorted(in_nu.begin(), in_nu.end())) {
			utils::bit_sort_mem(in_nu.begin(), in_nu.end(), &mem[0]);
		}
	}
	m_normalised = true;
}

bool dgraph::check_normalised() {
	if (not graph::check_normalised()) { return false; }

	// check that every adjacency list is sorted
	for (node u = 0; u < n_nodes(); ++u) {
		const neighbourhood& out_nu = m_adjacency_list[u];
		if (not std::is_sorted(out_nu.begin(), out_nu.end())) {
			// if some is not then the graph is not normalised
			m_normalised = false;
			return false;
		}

		const neighbourhood& in_nu = m_in_adjacency_list[u];
		if (not std::is_sorted(in_nu.begin(), in_nu.end())) {
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
	assert(has_node(u));
	assert(has_node(v));
	assert(u != v);
	assert(not has_edge(u,v));

	neighbourhood& out_u = m_adjacency_list[u];
	out_u.push_back(v);
	neighbourhood& in_v = m_in_adjacency_list[v];
	in_v.push_back(u);
	++m_num_edges;

	if (is_normalised()) {
		// the graph was normalised
		if (to_norm) {
			// keep it normalised
			utils::bit_sort(out_u.begin(), out_u.end());
			utils::bit_sort(in_v.begin(), in_v.end());
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
		assert(has_node(u));
		assert(has_node(v));
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

dgraph& dgraph::remove_edge(node u, node v, bool norm) {
	assert(has_node(u));
	assert(has_node(v));
	assert(u != v);
	assert(has_edge(u,v));
	--m_num_edges;

	neighbourhood& out_u = m_adjacency_list[u];
	neighbourhood& in_v = m_in_adjacency_list[v];

	remove_single_edge(u,v, out_u, in_v);

	// if (is_normalised()) {
	//		if (norm)     ... the graph is normalised. No need to check.
	//      if (not norm) ... the graph is normalised. No need to check.
	// }
	// if (not is_normalised()) {
	//      if (norm)     ... NORMALISE THE GRAPH
	//      if (not norm) ... Check if the result is normalised. It could be...
	// }

	if (not is_normalised()) {
		if (norm) {
			normalise();
		}
		else {
			// we might have been lucky...
			check_normalised();
		}
	}
	return *this;
}

dgraph& dgraph::remove_edges(const std::vector<edge>& edges, bool norm) {
	for (const edge& e : edges) {
		const node u = e.first;
		const node v = e.second;
		assert(has_node(u));
		assert(has_node(v));
		assert(u != v);
		assert(has_edge(u,v));
		--m_num_edges;

		neighbourhood& out_u = m_adjacency_list[u];
		neighbourhood& in_v = m_in_adjacency_list[v];
		remove_single_edge(u,v, out_u, in_v);
	}

	// if (is_normalised()) {
	//		if (norm)     ... the graph is normalised. No need to check.
	//      if (not norm) ... the graph is normalised. No need to check.
	// }
	// if (not is_normalised()) {
	//      if (norm)     ... NORMALISE THE GRAPH
	//      if (not norm) ... Check if the result is normalised. It could be...
	// }

	if (not is_normalised()) {
		if (norm) {
			normalise();
		}
		else {
			// we might have been lucky...
			check_normalised();
		}
	}
	return *this;
}

void dgraph::disjoint_union(const dgraph& g) {
	// number of vertices before adding the out-neighbours
	const uint32_t n = n_nodes();

	// this call updates the out-neighbours adjacency list,
	// as well as the number of edges and the graph's normalisation
	graph::__disjoint_union(g);

	// update the in-neighbours adjacency list
	for (node u = 0; u < g.n_nodes(); ++u) {
		// add new edges by appending all the neighbours of 'u' in 'g'
		m_in_adjacency_list.push_back( g.get_in_neighbours(u) );
		// relabel the nodes
		for (node& v : m_in_adjacency_list.back()) {
			v += n;
		}
	}

	// If one or none of the two graphs involved are normalised,
	// the result is not normalised.
	// If both graphs are normalised, the result is normalised.
	m_normalised = m_normalised and g.is_normalised();
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
			std::binary_search(out_u.begin(), out_u.end(), v) :
			std::binary_search(in_v.begin(), in_v.end(), u)
		);
	}
	return (out_u.size() <= in_v.size() ?
		std::find(out_u.begin(), out_u.end(), v) != out_u.end() :
		std::find(in_v.begin(), in_v.end(), u) != in_v.end()
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

void dgraph::remove_single_edge(
	node u, node v, neighbourhood& out_u, neighbourhood& in_v
)
{
	// it_v: pointer to node v in out_u
	// it_u: pointer to node u in in_v
	vector<node>::iterator it_v, it_u;

	// find the nodes in the lists
	if (is_normalised()) {
		it_v = std::lower_bound(out_u.begin(), out_u.end(), v);
		it_u = std::lower_bound(in_v.begin(), in_v.end(), u);
	}
	else {
		it_v = std::find(out_u.begin(), out_u.end(), v);
		it_u = std::find(in_v.begin(), in_v.end(), u);
	}

	// check that the iterators point to the correct value
	assert(*it_v == v);
	assert(*it_u == u);

	// remove edges from the lists
	out_u.erase(it_v);
	in_v.erase(it_u);
}

} // -- namespace graphs
} // -- namespace lal
