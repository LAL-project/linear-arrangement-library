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
 
#include <lal/graphs/ugraph.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <algorithm>
#include <cmath>
using namespace std;

// lal includes
#include <lal/utils/sort_integers.hpp>

namespace lal {
using namespace numeric;

namespace graphs {

/* PUBLIC */

ugraph::ugraph() : graph() { }
ugraph::ugraph(uint32_t n) {
	init(n);
}
ugraph::~ugraph() { }

/* OPERATORS */

/* MODIFIERS */

ugraph& ugraph::add_edge(node s, node t, bool to_norm) {
	assert(not has_edge(s,t));
	assert(s != t);
	assert(has_node(s));
	assert(has_node(t));

	neighbourhood& ns = m_adjacency_list[s];
	neighbourhood& nt = m_adjacency_list[t];
	ns.push_back(t);
	nt.push_back(s);
	++m_num_edges;

	if (m_normalised) {
		// the graph was normalised
		if (to_norm) {
			// keep it normalised. Insertion sort
			// applied to the last nodes added
			utils::sort_1_n(ns.begin(), ns.end());
			utils::sort_1_n(nt.begin(), nt.end());
		}
		else {
			// Even though we have not been asked to normalise the
			// graph, it may still be so... This means we have to
			// check whether the graph is still normalised. We may
			// be lucky....
			const size_t su = ns.size();
			const size_t sv = nt.size();
			if (su > 1 and sv > 1) {
				m_normalised = ns[su - 2] < ns[su - 1] and nt[sv - 2] < nt[sv - 1];
			}
			else if (su > 1) {
				m_normalised = ns[su - 2] < ns[su - 1];
			}
			else if (sv > 1) {
				m_normalised = nt[sv - 2] < nt[sv - 1];
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

ugraph& ugraph::add_edges(const vector<edge>& edges, bool to_norm) {
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

	return (nu.size() <= nv.size() ?
		find(nu.begin(), nu.end(), v) != nu.end() :
		find(nv.begin(), nv.end(), v) != nu.end()
	);
}

bool ugraph::is_directed() const { return false; }
bool ugraph::is_undirected() const { return true; }

/* PRIVATE */

} // -- namespace graphs
} // -- namespace lal

