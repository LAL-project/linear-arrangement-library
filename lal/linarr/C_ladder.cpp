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
 
#include <lal/linarr/C.hpp>

// C includes
#include <string.h>

// C++ includes
#include <algorithm>
#include <numeric>
using namespace std;

// lal includes
#include <lal/utils/macros.hpp>

namespace lal {
namespace linarr {

inline uint32_t __n_crossings_ladder(
	const ugraph& g, const vector<node>& pi,
	vector<bool>& bn,
	uint32_t * __restrict__ T,
	uint32_t * __restrict__ L1
)
{
	const uint32_t n = g.n_nodes();

	/* initialise memory */
	for (uint32_t i = 0; i < n; ++i) {
		T[ pi[i] ] = i;
		L1[i] = 0;
	}

	/* compute number of crossings */
	uint32_t C = 0;

	// no need to reach the last position of the arrangement
	for (uint32_t p = 0; p < n - 1; ++p) {
		const node u = T[p];

		// amount of crossings the edges incident to this node and
		// connecting nodes "to the right" of 'u' in the arrangement
		uint32_t S = 0;

		// neighbours of node u, as a list of Boolean values.
		bn = g.get_bool_neighbours(u);

		for (uint32_t q = p + 1; q < n; ++q) {
			node v = T[q];
			S += L1[q];
			if (bn[v]) {
				C += S - L1[q];
				++L1[q];
			}
			bn[v] = false;
		}
	}
	return C;
}

// T: translation table, inverse of pi:
// T[p] = u <-> at position p we find node u
uint32_t __n_crossings_ladder(const ugraph& g, const vector<node>& pi) {
	const uint32_t n = g.n_nodes();
	if (n < 4) {
		return 0;
	}

	/* allocate memory */

	// size of pi + size of L1
	const uint32_t total_elements = n + n;
	uint32_t *all_memory = static_cast<uint32_t *>(malloc(total_elements*sizeof(uint32_t)));

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p
	uint32_t * __restrict__ T = &all_memory[0];
	// array L1 (same as in the pseudocode)
	uint32_t * __restrict__ L1 = &all_memory[n];

	// boolean neighbourhood of nodes
	vector<bool> bool_neighs(n, false);

	/* compute number of crossings */
	uint32_t C = __n_crossings_ladder(g, pi, bool_neighs, T,L1);

	/* free memory */
	free(all_memory);
	return C;
}

uint32_t n_crossings_ladder(const ugraph& g, const vector<node>& pi) {
	return macros::call_with_empty_arrangement(__n_crossings_ladder, g, pi);
}

vector<uint32_t> n_crossings_ladder_list
(const ugraph& g, const vector<vector<node> >& pis)
{
	vector<uint32_t> cs(pis.size(), 0);
	const uint32_t n = g.n_nodes();
	if (n < 4) {
		return cs;
	}

	/* allocate memory */

	// size of pi + size of L1
	const uint32_t total_elements = n + n;
	uint32_t *all_memory = static_cast<uint32_t *>(malloc(total_elements*sizeof(uint32_t)));

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p
	uint32_t * __restrict__ T = &all_memory[0];
	// array L1 (same as in the pseudocode)
	uint32_t * __restrict__ L1 = &all_memory[n];

	// boolean neighbourhood of nodes
	vector<bool> bool_neighs(n, false);

	/* compute C for every linear arrangement */
	for (size_t i = 0; i < pis.size(); ++i) {
		cs[i] = __n_crossings_ladder(g, pis[i], bool_neighs, T,L1);
		bool_neighs.assign(n, false);
	}

	/* free memory */
	free(all_memory);
	return cs;
}

} // -- namespace linarr
} // -- namespace lal
