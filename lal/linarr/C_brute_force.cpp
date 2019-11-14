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
#include <assert.h>

// C++ includes
#include <numeric>
using namespace std;

// lal includes
#include <lal/utils/macros.hpp>
#include <lal/iterators/Q_iterator.hpp>

namespace lal {
using namespace graphs;
using namespace iterators;

#define idx(i,j, C) ((i)*(C) + (j))

namespace linarr {

uint64_t __compute_crossings_brute_force(
	const ugraph& g, const vector<position>& pi,
	node * __restrict__ T
)
{
	const uint64_t n = g.n_nodes();
	for (uint64_t i = 0; i < n; ++i) {
		T[ pi[i] ] = i;
	}

	uint64_t C = 0;

	// iterate over the pairs of edges that will potentially cross
	// using the information given in the linear arrangement
	for (node u = 0; u < g.n_nodes(); ++u) {
	// 'pu' is the position of node 'u'
	const position pu = pi[u];
	const neighbourhood& Nu = g.get_neighbours(u);
	for (const node& v : Nu) {
	// 'pv' is the position of node 'v'
	const position pv = pi[v];
	if (pu >= pv) { continue; }

		// 'u' and 'v' is a pair of connected nodes such that 'u'
		// is "to the left of" 'v' in the linear arrangement 'seq'

		// iterate through the positions between 'u' and 'v'
		const position begin = pi[u] + 1;
		const position end = pi[v] - 1;

		for (position pw = begin; pw <= end; ++pw) {
		// 'w' is the node at position 'pw'
		const node w = T[pw];
		const neighbourhood& Nw = g.get_neighbours(w);
		for (const node& z : Nw) {

			// if     pi[w] < pi[z]    then
			// 'w' and 'z' is a pair of connected nodes such that
			// 'w' is "to the left of" 'z' in the random seq 'seq'.
			// Formally: pi[w] < pi[z]

			// Also, by construction: pi[u] < pi[w]
			C += pi[w] < pi[z] and
				 pi[u] < pi[w] and pi[w] < pi[v] and pi[v] < pi[z];
		}}
	}}

	return C;
}

// T: translation table, inverse of pi:
// T[p] = u <-> at position p we find node u
uint64_t __call_crossings_brute_force(const ugraph& g, const vector<position>& pi) {
	const uint64_t n = g.n_nodes();
	if (n < 4) {
		return 0;
	}

	/* allocate memory */

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p
	node * __restrict__ T = static_cast<node *>( malloc(n*sizeof(node)) );

	// compute the number of crossings
	uint64_t C = __compute_crossings_brute_force(g, pi, T);

	/* free memory */
	free(T);
	return C;
}

uint64_t __n_crossings_brute_force(const ugraph& g, const vector<position>& pi) {
	return utils::call_with_empty_arrangement(__call_crossings_brute_force, g, pi);
}

vector<uint64_t> __n_crossings_brute_force_list
(const ugraph& g, const vector<vector<node> >& pis)
{
	const uint64_t n = g.n_nodes();

	vector<uint64_t> cs(pis.size(), 0);
	if (n < 4) {
		return cs;
	}

	/* allocate memory */

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p
	node * __restrict__ T = static_cast<node *>( malloc(n*sizeof(node)) );

	/* compute C for every linear arrangement */
	for (size_t i = 0; i < pis.size(); ++i) {
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);

		// compute C
		cs[i] = __compute_crossings_brute_force(g, pis[i], T);
	}

	/* free memory */
	free(T);
	return cs;
}

} // -- namespace linarr
} // -- namespace lal
