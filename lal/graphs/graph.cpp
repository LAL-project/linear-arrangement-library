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

#include <lal/graphs/graph.hpp>

// C++ includes
#include <algorithm>
#include <cassert>
#include <cstring>
#include <cmath>
#include <set>
using namespace std;

// lal includes
#include <lal/iterators/E_iterator.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/properties/Q.hpp>
#include <lal/internal/sorting/bit_sort.hpp>

namespace lal {
using namespace numeric;
using namespace iterators;

namespace graphs {

/* PUBLIC */

//graph::graph() { }
graph::graph(uint32_t n) {
	_init(n);
}
//graph::~graph() { }

void graph::init(uint32_t n) {
	_clear();
	_init(n);
}

void graph::clear() {
	_clear();
}

/* OPERATORS */

/* MODIFIERS */

void graph::normalise() {
	char *mem = static_cast<char *>(malloc(n_nodes()*sizeof(char)));
	memset(mem, 0, n_nodes()*sizeof(char));

	for (node u = 0; u < n_nodes(); ++u) {
		neighbourhood& nu = m_adjacency_list[u];
		if (not is_sorted(nu.begin(), nu.end())) {
			//utils::sort_1_n_inc(nu.begin(), nu.end());
			internal::bit_sort_mem(nu.begin(), nu.end(), mem);
		}
	}
	m_normalised = true;

	free(mem);
}

bool graph::check_normalised() {
	// check that every adjacency list is sorted
	for (node u = 0; u < n_nodes(); ++u) {
		const neighbourhood& nu = m_adjacency_list[u];
		if (not is_sorted(nu.begin(), nu.end())) {
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

/* SETTERS */

/* GETTERS */

bool graph::has_node(node u) const {
	return u < m_adjacency_list.size();
}

uint32_t graph::n_nodes() const {
	return static_cast<uint32_t>(m_adjacency_list.size());
}

uint32_t graph::n_edges() const {
	return m_num_edges;
}

vector<edge> graph::edges() const {
	vector<edge> e(n_edges());
	auto it = e.begin();
	E_iterator e_it(*this);
	while (e_it.has_next()) {
		e_it.next();
		*it = e_it.get_edge();
		++it;
	}
	return e;
}

bool graph::is_normalised() const {
	return m_normalised;
}

/* PROTECTED */

void graph::_init(uint32_t n) {
	m_num_edges = 0;
	m_normalised = true;
	m_adjacency_list = vector<neighbourhood>(n);
}

void graph::_clear() {
	m_num_edges = 0;
	m_normalised = true;
	m_adjacency_list.clear();
}

void graph::__disjoint_union(const graph& g) {
	// If I'm directed, g must be directed.
	// If I'm undirected, g must be undirected.
	assert(is_directed() ? g.is_directed() : g.is_undirected());

	const uint32_t n = n_nodes();
	m_num_edges += g.m_num_edges;

	for (node u = 0; u < g.n_nodes(); ++u) {
		// add new edges by appending all the neighbours of 'u' in 'g'
		m_adjacency_list.push_back( g.get_neighbours(u) );
		// relabel the nodes
		for (node& v : m_adjacency_list.back()) {
			v += n;
		}
	}

	// If one or none of the two graphs involved are normalised,
	// the result is not normalised.
	// If both graphs are normalised, the result is normalised.
	m_normalised = m_normalised and g.is_normalised();
}

vector<edge_pair> graph::Q(uint64_t qs) const {
	vector<edge_pair> q(qs);
	auto vec_it = q.begin();
	Q_iterator q_it(*this);
	while (q_it.has_next()) {
		q_it.next();
		*vec_it = q_it.get_pair();
		++vec_it;
	}
	return q;
}

} // -- namespace graphs
} // -- namespace lal
