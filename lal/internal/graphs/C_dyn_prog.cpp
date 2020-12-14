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
#include <vector>
using namespace std;

// lal includes
#include <lal/internal/macros.hpp>
#include <lal/internal/graphs/utils.hpp>

#define idx(i,j, C) ((i)*(C) + (j))

namespace lal {
using namespace graphs;

namespace internal {

#define degree_graph(G,v) (G.is_undirected() ? G.degree(v) : G.out_degree(v) + G.in_degree(v))

// T: translation table, inverse of pi:
// T[p] = u <-> at position p we find node u
inline uint32_t __compute_C_dyn_prog(
	const graph& g, const linear_arrangement& pi,
	char * __restrict__ bn,
	node * __restrict__ inv_pi,
	uint32_t * __restrict__ M,
	uint32_t * __restrict__ K
)
{	
	const uint32_t n = g.n_nodes();
	std::fill(&bn[0], &bn[n], 0);
	std::fill(&K[0], &K[(n - 3)*(n - 3)], 0);

	// compute pi
	for (node i = 0; i < n; ++i) {
		inv_pi[ pi[i] ] = i;
	}

	/* fill matrix M */

	for (position pu = 0; pu < n - 3; ++pu) {
		// node at position pu + 1
		const node u = inv_pi[pu + 1];

		internal::get_bool_neighbours(g, u, bn);

		uint32_t k = degree_graph(g,u);

		// check existence of edges between node u
		// and the nodes in positions 0 and 1 of
		// the arrangement
		k -= (bn[inv_pi[0]] + bn[inv_pi[1]]);
		bn[inv_pi[0]] = bn[inv_pi[1]] = 0;

		// this is done because there is no need to
		// fill the first two columns.

		// Now we start filling M at the third column
		for (uint32_t i = 3; i < n; ++i) {
			k -= bn[inv_pi[i - 1]];

			// the row corresponding to node 'u' in M is
			// the same as its position in the sequence.
			// This explains M[pu][*]

			//M[pu][i - 3] = k;
			M[ idx(pu,i-3, n-3) ] = k;

			// clear boolean neighbours so that at the next
			// iteration all its values are valid
			bn[inv_pi[i - 1]] = 0;
		}
	}

	/* fill matrix K */

	// special case for ii = 0 (see next for loop)
	K[idx(n-4,n-4, n-3)] = M[idx(n-4,n-4, n-3)];

	// pointer for next row in K
	uint32_t * __restrict__ next_k_it;
	// pointer for M
	uint32_t * __restrict__ m_it;
	// pointer for K
	uint32_t * __restrict__ k_it;

	for (uint32_t ii = 1; ii < n - 3; ++ii) {
		const uint32_t i = n - 3 - ii - 1;

		//m_it = &M[i][i];
		m_it = &M[ idx(i,i, n-3) ];

		// place k_it at the beginning of i-th row ("beginning" here means
		// the first column with non-redundant information: the upper half
		// of the matrix)
		k_it = &K[ idx(i,i, n-3) ];

		// place next_k_it at the same column as k_it but at the next row
		next_k_it = k_it + n - 3;

		for (uint32_t j = i; j < n - 3; ++j) {
			//K[i][j] = M[i][j] + K[i + 1][j];

			*k_it++ = *m_it++ + *next_k_it++;
		}
	}

	/* compute number of crossings */

	uint32_t C = 0;

	for (position pu = 0; pu < n - 3; ++pu) {
		const node u = inv_pi[pu];

		const neighbourhood& Nu = g.get_neighbours(u);
		for (const node& v : Nu) {

			// 'u' and 'v' is an edge of the graph.
			// In case that pi[u] < pi[v], or, equivalently, pu < pi[v],
			// then 'u' is "in front of" 'v' in the linear arrangement.
			// This explains the first condition 'pu < pi[v]'.
			// The second condition '2 <= pi[v] and pi[v] < n -1' is used
			// to avoid making illegal memory accesses.
			// --
			/*if (pu < pi[v] and 2 <= pi[v] and pi[v] < n - 1) {
				C += K[idx(pu,pi[v]-2, n-3)];
			}*/
			// --
			if (pu < pi[v] and 2 <= pi[v] and pi[v] < n - 1) {
				C += K[idx(pu,pi[v]-2, n-3)];
			}
		}
		if (g.is_directed()) {
			const neighbourhood& Nu_in = g.get_in_neighbours(u);
			for (const node& v : Nu_in) {

				// 'u' and 'v' is an edge of the graph.
				// In case that pi[u] < pi[v], or, equivalently, pu < pi[v],
				// then 'u' is "in front of" 'v' in the linear arrangement.
				// This explains the first condition 'pu < pi[v]'.
				// The second condition '2 <= pi[v] and pi[v] < n -1' is used
				// to avoid making illegal memory accesses.
				// --
				/*if (pu < pi[v] and 2 <= pi[v] and pi[v] < n - 1) {
					C += K[idx(pu,pi[v]-2, n-3)];
				}*/
				// --
				if (pu < pi[v] and 2 <= pi[v] and pi[v] < n - 1) {
					C += K[idx(pu,pi[v]-2, n-3)];
				}
			}
		}
	}

	return C;
}

// T: translation table, inverse of pi:
// T[p] = u <-> at position p we find node u
inline uint32_t __call_C_dyn_prog(const graph& g, const linear_arrangement& pi) {
	const uint32_t n = g.n_nodes();
	if (n < 4) {
		return 0;
	}

	/* allocate memory */
	const size_t n_elems = n + 2*(n - 3)*(n - 3);
	uint32_t * __restrict__ all_memory = new uint32_t[n_elems];

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p ( size n )
	position * __restrict__ T = &all_memory[0];
	// matrix M (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint32_t * __restrict__ M = &all_memory[n];
	// matrix K (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint32_t * __restrict__ K = &all_memory[0 + n + (n - 3)*(n - 3)];

	// boolean neighbourhood of nodes
	char * __restrict__ bool_neighs = new char[n];

	/* compute number of crossings */
	const uint32_t C = __compute_C_dyn_prog(g, pi, bool_neighs, T,M,K);

	/* free memory */
	delete[] all_memory;
	delete[] bool_neighs;
	return C;
}

uint32_t n_C_dynamic_programming(const graph& g, const linear_arrangement& pi) {
#if defined DEBUG
	assert(pi.size() == 0 or g.n_nodes() == pi.size());
#endif
	return internal::call_with_empty_arrangement(__call_C_dyn_prog, g, pi);
}

vector<uint32_t> n_C_dynamic_programming_list
(const graph& g, const vector<linear_arrangement>& pis)
{
	const uint32_t n = g.n_nodes();

	vector<uint32_t> cs(pis.size(), 0);
	if (n < 4) {
		return cs;
	}

	/* allocate memory */
	const size_t n_elems = n + 2*(n - 3)*(n - 3);
	uint32_t * __restrict__ all_memory = new uint32_t[n_elems];

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p ( size n )
	position * __restrict__ T = &all_memory[0];
	// matrix M (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint32_t * __restrict__ M = &all_memory[n];
	// matrix K (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint32_t * __restrict__ K = &all_memory[0 + n + (n - 3)*(n - 3)];

	// boolean neighbourhood of nodes
	char * __restrict__ bool_neighs = new char[n];

	/* compute C for every linear arrangement */
	for (size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		cs[i] = __compute_C_dyn_prog(g, pis[i], bool_neighs, T,M,K);

		// contents of 'bool_neighs' is set to 0 inside the function
		//bool_neighs.assign(n, false);
	}

	/* free memory */
	delete[] all_memory;
	delete[] bool_neighs;
	return cs;
}

} // -- namespace internal
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