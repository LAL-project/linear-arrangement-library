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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <cstring> // for 'memset' below
#include <vector>
using namespace std;

// lal includes
#include <lal/internal/macros.hpp>
#include <lal/internal/graphs/utils.hpp>

namespace lal {
using namespace graphs;

namespace internal {

template<class G>
inline uint32_t __compute_C_ladder(
	const G& g, const linear_arrangement& pi,
	char * __restrict__ bn,
	uint32_t * __restrict__ inv_pi,
	uint32_t * __restrict__ L1
)
{
	const uint32_t n = g.n_nodes();
	// inverse arrangement
	for (uint32_t i = 0; i < n; ++i) {
		inv_pi[ pi[i] ] = i;
	}

	/* compute number of crossings */
	uint32_t C = 0;

	// no need to reach the last position of the arrangement
	for (uint32_t p = 0; p < n - 1; ++p) {
		const node u = inv_pi[p];

		// amount of crossings the edges incident to this node and
		// connecting nodes "to the right" of 'u' in the arrangement
		uint32_t S = 0;

		// neighbours of node u, as a list of Boolean values.
		internal::get_bool_neighbours<G>(g, u, bn);

		for (uint32_t q = p + 1; q < n; ++q) {
			const node v = inv_pi[q];
			S += L1[q];

			// --
			/*if (bn[v]) {
				C += S - L1[q];
				++L1[q];
			}*/
			C += bn[v]*(S - L1[q]);
			L1[q] += bn[v];
			// --

			bn[v] = 0;
		}

		L1[p] = 0;
	}
	return C;
}

// T: translation table, inverse of pi:
// T[p] = u <-> at position p we find node u
template<class G>
inline uint32_t __call_C_ladder(const G& g, const linear_arrangement& pi) {
	const uint32_t n = g.n_nodes();
	if (n < 4) {
		return 0;
	}

	/* allocate memory */
	const uint32_t n_elems = n + n;
	uint32_t * __restrict__ all_memory = new uint32_t[n_elems]{0};

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p ( size n )
	uint32_t * __restrict__ T = &all_memory[0];
	// array L1 (same as in the pseudocode) ( size n )
	uint32_t * __restrict__ L1 = &all_memory[n];
	// boolean neighbourhood of nodes
	char * __restrict__ bool_neighs = new char[n]{0};

	/* compute number of crossings */
	const uint32_t C = __compute_C_ladder(g, pi, bool_neighs, T,L1);

	/* free memory */
	delete[] all_memory;
	delete[] bool_neighs;
	return C;
}

// ------------------
// single arrangement

template<class G>
uint32_t n_C_ladder(const G& g, const linear_arrangement& pi) {
#if defined DEBUG
	assert(pi.size() == 0 or g.n_nodes() == pi.size());
#endif
	return
	internal::call_with_empty_arrangement<uint32_t,G>
	(__call_C_ladder, g, pi);
}

uint32_t n_C_ladder
(const directed_graph& g, const linear_arrangement& pi)
{ return n_C_ladder<directed_graph>(g, pi); }

uint32_t n_C_ladder
(const undirected_graph& g, const linear_arrangement& pi)
{ return n_C_ladder<undirected_graph>(g, pi); }

// --------------------
// list of arrangements

template<class G>
vector<uint32_t> n_C_ladder_list
(const G& g, const vector<linear_arrangement>& pis)
{
	const uint32_t n = g.n_nodes();

	vector<uint32_t> cs(pis.size(), 0);
	if (n < 4) {
		return cs;
	}

	/* allocate memory */
	const uint32_t n_elems = n + n;
	uint32_t * __restrict__ all_memory = new uint32_t[n_elems]{0};

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p ( size n )
	uint32_t * __restrict__ T = &all_memory[0];
	// array L1 (same as in the pseudocode) ( size n )
	uint32_t * __restrict__ L1 = &all_memory[n];
	// boolean neighbourhood of nodes
	char * __restrict__ bool_neighs = new char[n]{0};

	/* compute C for every linear arrangement */
	for (size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		cs[i] = __compute_C_ladder(g, pis[i], bool_neighs, T,L1);

		memset(bool_neighs, 0, n);
		L1[n - 1] = 0;
	}

	/* free memory */
	delete[] all_memory;
	delete[] bool_neighs;
	return cs;
}

vector<uint32_t> n_C_ladder_list
(const directed_graph& g, const vector<linear_arrangement>& pis)
{ return n_C_ladder_list<directed_graph>(g, pis); }
vector<uint32_t> n_C_ladder_list
(const undirected_graph& g, const vector<linear_arrangement>& pis)
{ return n_C_ladder_list<undirected_graph>(g, pis); }

} // -- namespace internal
} // -- namespace lal
