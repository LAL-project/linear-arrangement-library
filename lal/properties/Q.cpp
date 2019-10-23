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
 
#include <lal/properties/Q.hpp>

// C++ includes
#include <cmath>
using namespace std;

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/properties/degrees.hpp>

namespace lal {
using namespace numeric;

namespace properties {

void enumerate_Q(const graph& g, vector<edge_pair>& Q) {
	const node n = static_cast<node>(g.n_nodes());
	Q.clear();

	pair<node,node> st, uv;

	// st
	for (node s = 0; s < n; ++s) {
	for (node t : g.get_neighbours(s)) {
	if (s > t) continue;

		// uv
		for (node u = s + 1; u < n; ++u) {
		for (node v : g.get_neighbours(u)) {
		if (u > v) continue;

			// s != u and t != u
			if (s == v or s == u) { continue; }
			if (t == v or t == u) { continue; }

			// no common endpoints
			st = make_pair(s, t);
			uv = make_pair(u, v);
			Q.push_back( make_pair(st, uv) );
		}}
	}}
}

integer size_Q_integer(const graph& g) {
	// sum of squared degrees
	integer nk2(0);
	for (node u = 0; u < g.n_nodes(); ++u) {
		uint32_t ku = g.degree(u);
		nk2 += ku*ku;
	}

	uint32_t m = g.n_edges();
	integer q(0);
	q.init_ui(m*(m + 1));
	q -= nk2;
	q /= 2;
	return q;
}

uint32_t size_Q(const graph& g) {
	// sum of squared degrees
	uint32_t nk2 = 0;
	for (node u = 0; u < g.n_nodes(); ++u) {
		uint32_t ku = g.degree(u);
		nk2 += ku*ku;
	}
	uint32_t m = g.n_edges();
	return (m*(m + 1) - nk2)/2;
}

} // -- namespace properties
} // -- namespace lal
