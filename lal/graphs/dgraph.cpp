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
#include <lal/iterators/edge_iterator.hpp>

namespace lal {
using namespace numeric;

namespace graphs {

/* PUBLIC */

dgraph::dgraph() : graph() { }
dgraph::dgraph(uint32_t n) {
	init(n);
}
dgraph::~dgraph() { }

/* OPERATORS */

/* MODIFIERS */

void dgraph::disjoint_union(const graph& g) {
	graph::disjoint_union(g);

	// if the call to 'graph::disjoint_union' did not
	// fail then graph 'g' is a directed graph
	const dgraph& dg = static_cast<const dgraph&>(g);

	m_in_degree.insert(
		m_in_degree.end(),
		dg.m_in_degree.begin(), dg.m_in_degree.end()
	);
}

dgraph& dgraph::add_edge(node u, node v, bool to_norm) {
	assert(not has_edge(u,v));
	assert(u != v);
	assert(has_node(u));
	assert(has_node(v));

	neighbourhood& nu = m_adjacency_list[u];
	nu.push_back(v);
	m_in_degree[v] += 1;
	++m_num_edges;

	if (m_normalised) {
		// the graph was normalised
		if (to_norm) {
			// keep it normalised. Insertion sort
			// applied to the last nodes added
			utils::sort_1_n(nu, n_nodes());
		}
		else {
			// Even though we have not been asked to normalise the
			// graph, it may still be so... This means we have to
			// check whether the graph is still normalised. We might
			// be lucky....
			const size_t su = nu.size();
			m_normalised = (su <= 1 ? m_normalised : nu[su - 2] < nu[su - 1]);
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
		node u = e.first;
		node v = e.second;
		assert(u != v);
		assert(not has_edge(u,v));

		neighbourhood& nu = m_adjacency_list[u];
		nu.push_back(v);
		m_in_degree[v] += 1;
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

/* SETTERS */

/* GETTERS */

bool dgraph::has_edge(node u, node v) const {
	assert(has_node(u));
	assert(has_node(v));

	const neighbourhood& nu = m_adjacency_list[u];
	return cget_neighbour_position(nu, v) != nu.end();
}

bool dgraph::is_directed() const { return true; }
bool dgraph::is_undirected() const { return false; }

uint32_t dgraph::in_degree(node u) const {
	assert(has_node(u));
	return m_in_degree[u];
}

ugraph dgraph::to_undirected() const {
	ugraph g(n_nodes());
	g.add_edges(edges());
	return g;
}

void dgraph::clear() {
	graph::clear();
	m_in_degree.clear();
}

/* PROTECTED */

void dgraph::_init(uint32_t n) {
	graph::_init(n);
	m_in_degree = vector<uint32_t>(n, 0);
}

/* PRIVATE */

} // -- namespace graphs
} // -- namespace lal

