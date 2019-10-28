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
 
#include <lal/ugraph.hpp>

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

ugraph::ugraph() : graph() { }
ugraph::ugraph(uint32_t n) : graph(n) { }
ugraph::~ugraph() { }

/* OPERATORS */

/* MODIFIERS */

ugraph& ugraph::add_edge(node u, node v, bool to_norm) {
	assert(not has_edge(u,v));
	assert(u != v);
	assert(has_node(u));
	assert(has_node(v));

	neighbourhood& nu = m_adjacency_list[u];
	neighbourhood& nv = m_adjacency_list[v];
	nu.push_back(v);
	nv.push_back(u);
	++m_num_edges;

	if (m_normalised) {
		// the graph was normalised
		if (to_norm) {
			// keep it normalised. Insertion sort
			// applied to the last nodes added
			resort(nu);
			resort(nv);
		}
		else {
			// Even though we have not been asked to normalise the
			// graph, it may still be so... This means we have to
			// check whether the graph is still normalised. We may
			// be lucky....
			size_t su = nu.size();
			size_t sv = nv.size();
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
	}
	else if (to_norm) {
		// the graph needs to be normalised,
		// from a non-normalised state
		normalise();
	}

	return *this;
}

ugraph& ugraph::add_edges(const std::vector<edge>& edges, bool to_norm) {
	for (const edge& e : edges) {
		node u = e.first;
		node v = e.second;
		assert(not has_edge(u,v));
		assert(u != v);

		neighbourhood& nu = m_adjacency_list[u];
		neighbourhood& nv = m_adjacency_list[v];
		nu.push_back(v);
		nv.push_back(u);
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

bool ugraph::has_edge(node u, node v) const {
	assert(has_node(u));
	assert(has_node(v));

	const neighbourhood& nu = m_adjacency_list[u];
	const neighbourhood& nv = m_adjacency_list[v];

	if (nu.size() <= nv.size()) {
		return cget_neighbour_position(nu, v) != nu.end();
	}
	return cget_neighbour_position(nv, u) != nv.end();
}

vector<edge> ugraph::edges() const {
	set<edge> unique_edges;

	// insert all edges into a set to get only those that are unique
	for (uint32_t i = 0; i < m_adjacency_list.size(); ++i) {
		lcit it = m_adjacency_list[i].begin();
		while (it != m_adjacency_list[i].end()) {

			edge e;
			if (i < *it) {
				e = edge(i, *it);
			}
			else {
				e = edge(*it, i);
			}

			bool new_edge = unique_edges.find(e) == unique_edges.end();
			if (new_edge) {
				unique_edges.insert(e);
			}
			++it;
		}
	}

	vector<edge> all_edges(unique_edges.size());

	// Dump all unique edges from the set into the vector 'all_edges'.
	// The size of the vector is equal to 'num_edges'
	set<edge>::const_iterator it = unique_edges.begin();
	vector<edge>::iterator eit = all_edges.begin();
	while (it != unique_edges.end()) {
		*eit = *it;
		++it;
		++eit;
	}

	return all_edges;
}

/* PRIVATE */

} // -- namespace lal

