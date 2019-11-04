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
#include <numeric>
using namespace std;

// lal includes
#include <lal/utils/macros.hpp>

namespace lal {
#define idx(i,j, C) ((i)*(C) + (j))

namespace linarr {

// T: translation table, inverse of pi:
// T[p] = u <-> at position p we find node u
inline uint32_t __n_crossings_dyn_prog(
	const ugraph& g, const vector<node>& T,
	vector<bool>& bn,
	uint32_t * __restrict__ pi,
	uint32_t * __restrict__ M,
	uint32_t * __restrict__ K
)
{
	const uint32_t n = g.n_nodes();

	// compute pi
	for (uint32_t i = 0; i < n; ++i) {
		pi[ T[i] ] = i;
	}

	/* fill matrix M */

	for (uint32_t pu = 0; pu < n - 3; ++pu) {
		// node at position pu + 1
		node u = T[pu + 1];

		bn = g.get_bool_neighbours(u);

		uint32_t k = g.degree(u);

		// check existence of edges between node u
		// and the nodes in positions 0 and 1 of
		// the arrangement
		k -= (bn[T[0]] + bn[T[1]]);
		bn[T[0]] = bn[T[1]] = false;

		// this is done because there is no need to
		// fill the first two columns.

		// Now we start filling M at the third column
		for (uint32_t i = 3; i < n; ++i) {
			k -= bn[T[i - 1]];

			// the row corresponding to node 'u' in M is
			// the same as its position in the sequence.
			// This explains M[pu][*]

			//M[pu][i - 3] = k;
			M[ idx(pu,i-3, n-3) ] = k;

			bn[T[i - 1]] = false;
		}
	}

	/* fill matrix K */
	memset(K, 0, (n - 3)*(n - 3)*sizeof(uint32_t));

	// special case for ii = 0 (see next for loop)
	K[idx(n-4,n-4, n-3)] = M[idx(n-4,n-4, n-3)];

	// pointer for next row in K
	uint32_t * __restrict__ next_k_it;
	// pointer for M
	uint32_t * __restrict__ m_it;
	// pointer for K
	uint32_t * __restrict__ k_it;

	for (uint32_t ii = 1; ii < n - 3; ++ii) {
		uint32_t i = n - 3 - ii - 1;

		//m_it = &M[i][i];
		m_it = &M[ idx(i,i, n-3) ];

		// place k_it at the beginning of i-th row ("beginning" here means
		// the first column with non-redundant information: the upper half
		// of the matrix)
		k_it = &K[ idx(i,i,(n - 3)) ];

		// place next_k_it at the same column as k_it but at the next row
		next_k_it = k_it + n - 3;

		for (uint32_t j = i; j < n - 3; ++j) {
			//K[i][j] = M[i][j] + K[i + 1][j];

			*k_it++ = *m_it++ + *next_k_it++;
		}
	}

	/* compute number of crossings */

	uint32_t C = 0;

	for (uint32_t pu = 0; pu < n - 3; ++pu) {
		const node u = T[pu];

		const neighbourhood& Nu = g.get_neighbours(u);
		for (const node& v : Nu) {

			/* 'u' and 'v' is an edge of the graph.
			 *
			 * In case that pi[u] < pi[v], or, equivalently, pu < pi[v],
			 * then 'u' is "in front of" 'v' in the linear arrangement.
			 * This explains the first condition 'pu < pi[v]'.
			 *
			 * The second condition '2 <= pi[v] and pi[v] < n -1' is used
			 * to avoid making illegal memory accesses.
			 */

			if (pu < pi[v] and 2 <= pi[v] and pi[v] < n - 1) {
				C += K[idx(pu,pi[v]-2, n-3)];
			}
		}
	}

	return C;
}

// T: translation table, inverse of pi:
// T[p] = u <-> at position p we find node u
uint32_t __n_crossings_dyn_prog(const ugraph& g, const vector<node>& T) {
	const uint32_t n = g.n_nodes();
	if (n < 4) {
		return 0;
	}

	/* allocate memory */

	// size of pi + size of M + size of K
	const uint32_t total_elements = n + 2*(n - 3)*(n - 3);
	uint32_t * __restrict__ all_memory = static_cast<uint32_t *>(malloc(total_elements*sizeof(uint32_t)));

	// actual linear arrangement (following notation used in the thesis):
	// pi[u] = p <-> node u is at position p
	uint32_t * __restrict__ pi = &all_memory[0];
	// matrix M (without 3 of its columns and rows)
	uint32_t * __restrict__ M = &all_memory[0 + n];
	// matrix K (without 3 of its columns and rows)
	uint32_t * __restrict__ K = &all_memory[0 + n + (n - 3)*(n - 3)];

	// boolean neighbourhood of nodes
	vector<bool> bool_neighs(n, false);

	/* compute number of crossings */
	uint32_t C = __n_crossings_dyn_prog(g, T, bool_neighs, pi,M,K);

	/* free memory */
	free(all_memory);
	return C;
}

uint32_t n_crossings_dyn_prog(const ugraph& g, const vector<node>& arr) {
	return macros::call_with_empty_arrangement(__n_crossings_dyn_prog, g, arr);
}

void n_crossings_dyn_prog_list
(const ugraph& g, const vector<vector<node> >& Ts, vector<uint32_t>& cs)
{
	const uint32_t n = g.n_nodes();
	if (n < 4) {
		cs = vector<uint32_t>(Ts.size(), 0);
		return;
	}

	/* allocate memory */
	cs.resize(Ts.size());

	// size of pi + size of M + size of K
	const uint32_t total_elements = n + 2*(n - 3)*(n - 3);
	uint32_t * __restrict__ all_memory = static_cast<uint32_t *>(malloc(total_elements*sizeof(uint32_t)));

	// actual linear arrangement (following notation used in the thesis):
	// pi[u] = p <-> node u is at position p
	uint32_t * __restrict__ pi = &all_memory[0];
	// matrix M (without 3 of its columns and rows)
	uint32_t * __restrict__ M = &all_memory[0 + n];
	// matrix K (without 3 of its columns and rows)
	uint32_t * __restrict__ K = &all_memory[0 + n + (n - 3)*(n - 3)];

	// boolean neighbourhood of nodes
	vector<bool> bool_neighs(n, false);

	/* compute C for every linear arrangement */
	for (size_t i = 0; i < Ts.size(); ++i) {
		cs[i] = __n_crossings_dyn_prog(g, Ts[i], bool_neighs, pi,M,K);
		bool_neighs.assign(n, false);
	}

	/* free memory */
	free(all_memory);
}

} // -- namespace linarr
} // -- namespace lal















/*
// This is a basic, straightforward and easy to understand
// implementation of the above dynamic programming algorithm.

inline void compute_M
(
	size_t L,
	const graph& G, const vector<size_t>& seq,
	vector<vector<size_t> >& M
)
{
	for (size_t pu = 0; pu < L; ++pu) {
		size_t u = seq[pu];

		// the row corresponding to node 'u' in M is
		// the same as its position in the sequence

		size_t k = G.degree(u);
		M[pu][0] = k;
		for (size_t i = 1; i < L and k > 0; ++i) {
			if (G.has_edge(u, seq[i - 1])) --k;
			M[pu][i] = k;
		}
	}
}

inline void compute_K
(
	size_t L,
	const graph& G, const vector<size_t>& seq,
	vector<vector<size_t> >& K
)
{
	// Fill M matrix
	vector<vector<size_t> > M(L, vector<size_t>(L));
	compute_M(L, G, seq, M);

	// Fill K matrix
	for (size_t i = L - 3 - 1; i > 0; --i) {
		for (size_t j = L - 1 - 1; j >= i + 2; --j) {
			K[i][j] = M[i + 1][j + 1] + K[i + 1][j];
		}
	}
	for (size_t j = L - 1 - 1; j >= 2; --j) {
		K[0][j] = M[1][j + 1] + K[1][j];
	}
}

size_t crossings_sequence_n2_n2(const graph& G, const vector<size_t>& seq) {
	const size_t L = seq.size();
	if (L < 4) return 0;

	// translation table
	// T[u] = p <-> node u is at position p
	vector<size_t> T(G.n_nodes(), L + 1);
	for (size_t i = 0; i < L; ++i) {
		T[ seq[i] ] = i;
	}

	vector<vector<size_t> > K(L, vector<size_t>(L, 0));
	compute_K(L, G, seq, K);

	size_t C = 0;

	for (size_t pu = 0; pu < L; ++pu) {
		size_t u = seq[pu];

		const neighbourhood& Nu = G.get_neighbours(u);
		for (const size_t& v : Nu) {

			if (pu < T[v]) {
				// 'u' and 'v' is a pair of connected nodes such that
				// 'u' is "in front of" 'v' in the random seq 'seq'.

				C += K[pu][ T[v] ];
			}
		}
	}

	return C;
}
*/
