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
 
#include <lal/linarr/C.hpp>

// C includes
#include <string.h>

// C++ includes
#include <numeric>
using namespace std;

// lal includes
#include <lal/utils/macros.hpp>

namespace lal {

#define idx(i,j, C) ((i)*(C) + (j))

namespace linarr {

// T: translation table, inverse of pi:
// T[p] = u <-> at position p we find node u
uint32_t __n_crossings_brute_force(const graph& g, const vector<node>& T) {
	const uint32_t n = g.n_nodes();
	if (n < 4) {
		return 0;
	}

	uint32_t C = 0;

	// actual linear arrangement:
	// pi[u] = p <-> node u is at position p
	uint32_t *pi = static_cast<uint32_t *>( malloc(n*sizeof(uint32_t)) );
	for (uint32_t i = 0; i < n; ++i) {
		pi[ T[i] ] = i;
	}

	for (node u = 0; u < g.n_nodes(); ++u) {
	uint32_t pu = pi[u];
	const neighbourhood& Nu = g.get_neighbours(u);
	for (const node& v : Nu) {
		uint32_t pv = pi[v];

		if (pu >= pv) { continue; }
		// 'u' and 'v' is a pair of connected nodes such that 'u'
		// is "to the left of" 'v' in the linear arrangement 'seq'

		uint32_t begin = pi[u] + 1;
		uint32_t end = pi[v] - 1;
		for (uint32_t pw = begin; pw <= end; ++pw) {
		// 'w' is the node at position 'pw'
		node w = T[pw];
		const neighbourhood& Nw = g.get_neighbours(w);
		for (const node& z : Nw) {

			if (pi[w] < pi[z]) {
				// 'w' and 'z' is a pair of connected nodes such that
				// 'w' is "in front of" 'z' in the random seq 'seq'.
				// Formally: pi[w] < pi[z]

				// Also, by construction: pi[u] < pi[w]
				C += pi[u] < pi[w] and pi[w] < pi[v] and pi[v] < pi[z];
			}
		}}
	}}

	free(pi);
	return C;
}

uint32_t n_crossings_brute_force(const graph& g, const vector<node>& arr) {
	return macros::call_with_empty_arrangement(__n_crossings_brute_force, g, arr);
}

} // -- namespace linarr
} // -- namespace lal
