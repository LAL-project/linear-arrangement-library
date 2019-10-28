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

#include <lal/dgraph.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <algorithm>
#include <cmath>
#include <set>
using namespace std;

// Use insertion sort to resort the vector (if needed)
// assuming that a new element has been appended at the end
// potentially making the vector unsorted.
// Assume vector is an adjacency list.
template<class T>
inline void resort(vector<T>& v) {
	if (v.size() <= 1) { return; }
	size_t i = v.size() - 1;
	while (i >= 1) {
		// strict '>' since there can't be repeated values.
		if (v[i - 1] > v[i]) {
			std::swap(v[i - 1], v[i]);
		}
		--i;
	}
}

namespace lal {
using namespace numeric;

/* PUBLIC */

dgraph::dgraph() : graph() { }
dgraph::dgraph(uint32_t n) : graph(n) { }
dgraph::~dgraph() { }

/* OPERATORS */

/* MODIFIERS */

dgraph& dgraph::add_edge(node u, node v, bool to_norm) {
	assert(not has_edge(u,v));
	assert(u != v);
	assert(has_node(u));
	assert(has_node(v));

	neighbourhood& nu = m_adjacency_list[u];
	nu.push_back(v);
	++m_num_edges;

	if (m_normalised) {
		// the graph was normalised
		if (to_norm) {
			// keep it normalised. Insertion sort
			// applied to the last nodes added
			resort(nu);
		}
		else {
			// Even though we have not been asked to normalise the
			// graph, it may still be so... This means we have to
			// check whether the graph is still normalised. We might
			// be lucky....
			size_t su = nu.size();
			if (su > 1) {
				m_normalised = nu[su - 2] < nu[su - 1];
			}
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
		assert(not has_edge(u,v));
		assert(u != v);

		neighbourhood& nu = m_adjacency_list[u];
		nu.push_back(v);
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

vector<edge> dgraph::edges() const {
	vector<edge> all_edges(m_num_edges);
	auto it = all_edges.begin();

	// insert all edges into a set to get only those that are unique
	for (node u = 0; u < n_nodes() and it != all_edges.end(); ++u) {
		auto adj_u = get_neighbours(u);
		for (size_t i = 0; i < adj_u.size() and it != all_edges.end(); ++i) {
			node v = m_adjacency_list[u][i];
			*it = edge(u, v);
			++it;
		}
	}

	return all_edges;
}

ugraph dgraph::to_undirected() const {
	// build list of undirected edges
	set<edge> all_undir_edges;

	// insert all edges into a set to get only those that are unique
	for (node u = 0; u < n_nodes(); ++u) {
		for (node v : get_neighbours(u)) {
			all_undir_edges.insert(
				u < v ? edge(u, v) : edge(v, u)
			);
		}
	}

	ugraph g(n_nodes());
	g.add_edges(vector<edge>(all_undir_edges.begin(), all_undir_edges.end()));
	return g;
}

bool dgraph::is_directed() const { return true; }
bool dgraph::is_undirected() const { return false; }

/* PRIVATE */

} // -- namespace lal

