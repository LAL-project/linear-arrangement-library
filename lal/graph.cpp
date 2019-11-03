/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
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
 *          Resarch Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

#include <lal/graph.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <algorithm>
#include <cmath>
#include <set>
using namespace std;

// lal includes
#include <lal/utils/sort_integers.hpp>
#include <lal/iterators/edge_iterator.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/properties/Q.hpp>

namespace lal {
using namespace numeric;
using namespace iterators;

/* PUBLIC */

graph::graph() { }
graph::graph(uint32_t n) {
	init(n);
}
graph::~graph() { }

void graph::init(uint32_t n) {
	clear();
	_init(n);
}

/* OPERATORS */

/* MODIFIERS */

void graph::disjoint_union(const graph& g) {
	uint32_t n = n_nodes();
	m_num_edges += g.m_num_edges;

	for (node u = 0; u < g.n_nodes(); ++u) {
		// add new edges by appending all the neighbours of 'u' in 'g'
		m_adjacency_list.push_back( g.get_neighbours(u) );
		// relabel the vertices
		for (node& v : m_adjacency_list.back()) {
			v += n;
		}
	}

	// If one or none of the two graphs involved are normalised,
	// the result is not normalised.
	// If both graphs are normalised, the result is normalised.
	m_normalised = m_normalised and g.is_normalised();
}

void graph::normalise() {
	for (node u = 0; u < n_nodes(); ++u) {
		neighbourhood& nu = m_adjacency_list[u];
		macros::sort_1_n(nu, n_nodes());
	}
	m_normalised = true;
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

void graph::clear() {
	m_num_edges = 0;
	m_normalised = true;
	m_adjacency_list.clear();
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
	edge_iterator e_it(*this);
	while (e_it.has_next()) {
		e_it.next();
		*it = e_it.get_edge();
		++it;
	}
	return e;
}

vector<edge_pair> graph::Q() const {
	vector<edge_pair> q(properties::size_Q(*this));
	auto vec_it = q.begin();
	Q_iterator q_it(*this);
	while (q_it.has_next()) {
		q_it.next();
		*vec_it = q_it.get_pair();
		++vec_it;
	}
	return q;
}

const neighbourhood& graph::get_neighbours(node u) const {
	return m_adjacency_list[u];
}

neighbourhood_B graph::get_bool_neighbours(node u) const {
	neighbourhood_B neighs(m_adjacency_list.size(), false);
	for (const node& v : get_neighbours(u)) {
		neighs[v] = true;
	}
	return neighs;
}

uint32_t graph::degree(node u) const {
	return static_cast<uint32_t>(m_adjacency_list[u].size());
}

bool graph::is_normalised() const {
	return m_normalised;
}

void graph::get_adjacency_matrix(vector<vector<bool> >& mat) const {
	const uint32_t N = n_nodes();
	mat = vector<vector<bool> >(N, vector<bool>(N, false));
	edge_iterator it(*this);
	while (it.has_next()) {
		it.next();
		const auto [u,v] = it.get_edge();
		mat[u][v] = true;
		mat[v][u] = (is_undirected() ? true : mat[v][u]);
	}
}

/* PROTECTED */

void graph::_init(uint32_t n) {
	m_num_edges = 0;
	m_normalised = true;
	m_adjacency_list = vector<neighbourhood>(n);
}

lcit graph::cget_neighbour_position(const neighbourhood& n, node u) const {
	lcit cit = n.begin();
	while (cit != n.end()) {
		if (*cit == u) {
			return cit;
		}
		else {
			++cit;
		}
	}
	return n.end();
}

lit graph::get_neighbour_position(neighbourhood& n, node u) {
	lit it = n.begin();
	while (it != n.end()) {
		if (*it == u) {
			return it;
		}
		else {
			++it;
		}
	}
	return n.end();
}

} // -- namespace lal

