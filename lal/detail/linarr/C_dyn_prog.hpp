/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
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

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/detail/macros/call_with_empty_arr.hpp>
#include <lal/detail/graphs/utils.hpp>
#include <lal/detail/data_array.hpp>

#define idx(i,j, C) ((i)*(C) + (j))
#define DECIDED_C_GT (upper_bound + 1)

namespace lal {
namespace detail {

namespace __lal {

// =============================================================================
// ACTUAL ALGORITHM
// =============================================================================

// When decide_upper_bound is false:
//		returns the number of crossings
// When decide_upper_bound is true:
//		returns m*m + 1 if the number of crossings is greater than the upper_bound
//		returns the number of crossings if the number of crossings is less
//			than the upper_bound
template<class graph_t, bool decide_upper_bound>
uint64_t compute_C_dyn_prog
(
	const graph_t& g, const linear_arrangement& pi,
	unsigned char * const __restrict__ bn,
	uint64_t * const __restrict__ M,
	uint64_t * const __restrict__ K,
	uint64_t upper_bound = 0
)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	std::fill(bn, &bn[n], 0);
	std::fill(K, &K[(n - 3)*(n - 3)], 0);

	const node u0 = pi[position_t{0ULL}];
	const node u1 = pi[position_t{1ULL}];

	/* fill matrix M */

	for (position_t pu = 0ULL; pu < n - 3; ++pu) {
		// node at position pu + 1
		const node u = pi[pu + 1ULL];

		detail::get_bool_neighbours<graph_t>(g, u, bn);

		uint64_t k = g.get_degree(u);

		// check existence of edges between node u
		// and the nodes in positions 0 and 1 of
		// the arrangement
		k -= bn[u0] + bn[u1];
		bn[u0] = bn[u1] = 0;

		// this is done because there is no need to
		// fill the first two columns.

		// Now we start filling M at the third column
		for (position_t i = 3ULL; i < n; ++i) {
			// node at position i - 1
			const node ui = pi[i - 1ULL];
			k -= bn[ui];

			// the row corresponding to node 'u' in M is
			// the same as its position in the sequence.
			// This explains M[pu][*]

			//M[pu][i - 3] = k;
			M[ idx(*pu, *i - 3, n-3) ] = k;

			// clear boolean neighbours so that at the next
			// iteration all its values are valid
			bn[ui] = 0;
		}
	}

	/* fill matrix K */

	// special case for ii = 0 (see next for loop)
	K[idx(n-4,n-4, n-3)] = M[idx(n-4,n-4, n-3)];

	// pointer for next row in K
	uint64_t * __restrict__ next_k_it;
	// pointer for M
	uint64_t * __restrict__ m_it;
	// pointer for K
	uint64_t * __restrict__ k_it;

	for (uint64_t ii = 1; ii < n - 3; ++ii) {
		const uint64_t i = n - 3 - ii - 1;

		//m_it = &M[i][i];
		m_it = &M[ idx(i,i, n-3) ];

		// place k_it at the beginning of i-th row ("beginning" here means
		// the first column with non-redundant information: the upper half
		// of the matrix)
		k_it = &K[ idx(i,i, n-3) ];

		// place next_k_it at the same column as k_it but at the next row
		next_k_it = k_it + n - 3;

		for (uint64_t j = i; j < n - 3; ++j) {
			//K[i][j] = M[i][j] + K[i + 1][j];

			*k_it++ = *m_it++ + *next_k_it++;
		}
	}

	/* compute number of crossings */

	uint64_t C = 0;

	const auto process_neighbours =
	[&](const position pu, const node_t v) -> void {
		const position pv = pi[v];
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
		if (pu < pv and 2 <= pv and pv < n - 1) {
			C += K[idx(pu,pv-2, n-3)];
		}
	};

	for (position pu = 0; pu < n - 3; ++pu) {
		const node u = pi[position_t{pu}];

		if constexpr (std::is_base_of_v<graphs::directed_graph, graph_t>) {
			const neighbourhood& Nu = g.get_out_neighbours(u);
			for (node_t v : Nu) {
				process_neighbours(pu, v);
				if constexpr (decide_upper_bound) {
					if (C > upper_bound) { return DECIDED_C_GT; }
				}
			}
			const neighbourhood& Nu_in = g.get_in_neighbours(u);
			for (node_t v : Nu_in) {
				process_neighbours(pu, v);
				if constexpr (decide_upper_bound) {
					if (C > upper_bound) { return DECIDED_C_GT; }
				}
			}
		}
		else {
			const neighbourhood& Nu = g.get_neighbours(u);
			for (node_t v : Nu) {
				process_neighbours(pu, v);
				if constexpr (decide_upper_bound) {
					if (C > upper_bound) { return DECIDED_C_GT; }
				}
			}
		}
	}

	// none of the conditions above were true, so we must have
	// C <= upper_bound
	return C;
}

} // -- namespace __lal

// =============================================================================
// CALLS TO THE ALGORITHM
// =============================================================================

// ------------------
// single arrangement

template<class graph_t>
uint64_t call_C_dyn_prog
(const graph_t& g, const linear_arrangement& pi)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	if (n < 4) {
		return 0;
	}

	// boolean neighbourhood of nodes
	data_array<unsigned char> bool_neighs(n);

	const std::size_t n_elems = 2*(n - 3)*(n - 3);
	data_array<uint64_t> all_memory(n_elems);

	// matrix M (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ M = &all_memory[0];
	// matrix K (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ K = &all_memory[0 + (n - 3)*(n - 3)];

	/* compute number of crossings */
	return __lal::compute_C_dyn_prog<graph_t, false>(g, pi, bool_neighs.begin(), M,K);
}

template<class graph_t>
uint64_t n_C_dynamic_programming
(const graph_t& g, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif
	return detail::call_with_empty_arrangement
			(call_C_dyn_prog<graph_t>, g, pi);
}

// --------------------
// list of arrangements

template<class graph_t>
std::vector<uint64_t> n_C_dynamic_programming(
	const graph_t& g,
	const std::vector<linear_arrangement>& pis
)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(pis.size(), 0);
	if (n < 4) {
		return cs;
	}

	/* allocate memory */
	const std::size_t n_elems = 2*(n - 3)*(n - 3);
	data_array<uint64_t> all_memory(n_elems);

	// matrix M (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ M = &all_memory[0];
	// matrix K (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ K = &all_memory[0 + (n - 3)*(n - 3)];

	// boolean neighbourhood of nodes
	data_array<unsigned char> bool_neighs(n);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		cs[i] = __lal::compute_C_dyn_prog<graph_t, false>
				(g, pis[i], bool_neighs.begin(), M,K);

		// contents of 'bool_neighs' is set to 0 inside the function
		//bool_neighs.assign(n, false);
	}

	return cs;
}

// -----------------------------------------------------------------------------
// DECISION

// ------------------
// single arrangement

template<class graph_t>
uint64_t call_C_dyn_prog_lesseq_than(
	const graph_t& g,
	const linear_arrangement& pi,
	uint64_t upper_bound
)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	if (n < 4) {
		return 0;
	}

	// boolean neighbourhood of nodes
	data_array<unsigned char> bool_neighs(n);

	const std::size_t n_elems = 2*(n - 3)*(n - 3);
	data_array<uint64_t> all_memory(n_elems);

	// matrix M (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ M = &all_memory[0];
	// matrix K (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ K = &all_memory[0 + (n - 3)*(n - 3)];

	/* decide */
	return __lal::compute_C_dyn_prog<graph_t, true>
			(g, pi, bool_neighs.begin(), M,K, upper_bound);
}

template<class graph_t>
uint64_t is_n_C_dynamic_programming_lesseq_than(
	const graph_t& g,
	const linear_arrangement& pi,
	uint64_t upper_bound
)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif
	return
	detail::call_with_empty_arrangement
	(call_C_dyn_prog_lesseq_than<graph_t>, g, pi, upper_bound);
}

// --------------------
// list of arrangements

template<class graph_t>
std::vector<uint64_t> is_n_C_dynamic_programming_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& pis,
	uint64_t upper_bound
)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(pis.size(), 0);
	if (n < 4) {
		return cs;
	}

	/* allocate memory */
	const std::size_t n_elems = 2*(n - 3)*(n - 3);
	data_array<uint64_t> all_memory(n_elems);

	// matrix M (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ M = &all_memory[0];
	// matrix K (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ K = &all_memory[0 + (n - 3)*(n - 3)];

	// boolean neighbourhood of nodes
	data_array<unsigned char> bool_neighs(n);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		cs[i] = __lal::compute_C_dyn_prog<graph_t, true>
				(g, pis[i], bool_neighs.begin(), M,K, upper_bound);

		// contents of 'bool_neighs' is set to 0 inside the function
		//bool_neighs.assign(n, false);
	}

	/* free memory */
	return cs;
}

template<class graph_t>
std::vector<uint64_t> is_n_C_dynamic_programming_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& pis,
	const std::vector<uint64_t>& upper_bounds
)
noexcept
{
#if defined DEBUG
	// ensure that there are as many linear arrangements as upper bounds
	assert(pis.size() == upper_bounds.size());
#endif

	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(pis.size(), 0);
	if (n < 4) {
		return cs;
	}

	/* allocate memory */
	const std::size_t n_elems = 2*(n - 3)*(n - 3);
	data_array<uint64_t> all_memory(n_elems);

	// matrix M (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ M = &all_memory[0];
	// matrix K (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ K = &all_memory[0 + (n - 3)*(n - 3)];

	// boolean neighbourhood of nodes
	data_array<unsigned char> bool_neighs(n);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		cs[i] = __lal::compute_C_dyn_prog<graph_t, true>
				(g, pis[i], bool_neighs.begin(), M,K, upper_bounds[i]);

		// contents of 'bool_neighs' is set to 0 inside the function
		//bool_neighs.assign(n, false);
	}

	/* free memory */
	return cs;
}

} // -- namespace detail
} // -- namespace lal















/*
// This is a basic, straightforward and easy-to-understand
// implementation of the above dynamic programming algorithm.

inline void compute_M
(
	std::size_t L,
	const graph& G, const vector<std::size_t>& seq,
	vector<vector<std::size_t> >& M
)
{
	for (std::size_t pu = 0; pu < L; ++pu) {
		std::size_t u = seq[pu];

		// the row corresponding to node 'u' in M is
		// the same as its position in the sequence

		std::size_t k = G.get_degree(u);
		M[pu][0] = k;
		for (std::size_t i = 1; i < L and k > 0; ++i) {
			if (G.has_edge(u, seq[i - 1])) --k;
			M[pu][i] = k;
		}
	}
}

inline void compute_K
(
	std::size_t L,
	const graph& G, const vector<std::size_t>& seq,
	vector<vector<std::size_t> >& K
)
{
	// Fill M matrix
	vector<vector<std::size_t> > M(L, vector<std::size_t>(L));
	compute_M(L, G, seq, M);

	// Fill K matrix
	for (std::size_t i = L - 3 - 1; i > 0; --i) {
		for (std::size_t j = L - 1 - 1; j >= i + 2; --j) {
			K[i][j] = M[i + 1][j + 1] + K[i + 1][j];
		}
	}
	for (std::size_t j = L - 1 - 1; j >= 2; --j) {
		K[0][j] = M[1][j + 1] + K[1][j];
	}
}

std::size_t crossings_sequence_n2_n2(const graph& G, const vector<std::size_t>& seq) {
	const std::size_t L = seq.size();
	if (L < 4) return 0;

	// translation table
	// T[u] = p <-> node u is at position p
	vector<std::size_t> T(G.get_num_nodes(), L + 1);
	for (std::size_t i = 0; i < L; ++i) {
		T[ seq[i] ] = i;
	}

	vector<vector<std::size_t> > K(L, vector<std::size_t>(L, 0));
	compute_K(L, G, seq, K);

	std::size_t C = 0;

	for (std::size_t pu = 0; pu < L; ++pu) {
		std::size_t u = seq[pu];

		const neighbourhood& Nu = G.get_neighbours(u);
		for (const std::size_t& v : Nu) {

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
