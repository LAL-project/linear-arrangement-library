/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
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
#include <lal/detail/arrangement_wrapper.hpp>
#include <lal/detail/graphs/utils.hpp>
#include <lal/detail/array.hpp>

#define idx(i,j, C) ((i)*(C) + (j))
#define DECIDED_C_GT (upper_bound + 1)

namespace lal {
namespace detail {
namespace crossings {

/**
 * @brief Namespace for the dynamic programming algorithm to calculate \f$C\f$.
 *
 * See \cite Alemany2019a.
 */
namespace dyn_prog {

// =============================================================================
// ACTUAL ALGORITHM
// =============================================================================

/**
 * @brief Dynamic programming computation of \f$C\f$ for undirected graphs.
 *
 * When template parameter @e decide_upper_bound is false, the function returns
 * the number of crossings.
 * @tparam decide_upper_bound Boolean value to choose the nature of the return type.
 * @tparam arrangement_t Type of arrangement.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param bn Array of size @e n. Boolean neighbourhood of a vertex @e u: @e bn[i]
 * is 1 if vertex @e u and vertex @e i are adjacent.
 * @param M See \cite Alemany2019a for details. Also, see the end of the
 * corresponding file.
 * @param K See \cite Alemany2019a for details. Also, see the end of the
 * corresponding file.
 * @param upper_bound Upper bound on the number of crossings.
 * @returns When @e decide_upper_bound is true, the return value is:
 * - one unit larger than the upper bound passed as parameter if \f$C>\f$ upper bound.
 * - \f$C\f$ if the number of crossings is less or equal than the upper bound.
 */
template <bool decide_upper_bound, class graph_t, class arrangement_t>
[[nodiscard]] uint64_t compute
(
	const graph_t& g,
	const arrangement_t& arr,
	unsigned char * const __restrict__ bn,
	uint64_t * const __restrict__ M,
	uint64_t * const __restrict__ K,
	const uint64_t upper_bound
)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	std::fill(bn, &bn[n], 0);
	std::fill(K, &K[(n - 3)*(n - 3)], 0);

	const node u0 = arr[position_t{0ull}];
	const node u1 = arr[position_t{1ull}];

	/* fill matrix M */

	for (position_t pu = 0ull; pu < n - 3; ++pu) {
		// node at position pu + 1
		const node u = arr[pu + 1ull];

		get_bool_neighbors<graph_t>(g, u, bn);

		uint64_t k = g.get_degree(u);

		// check existence of edges between node u
		// and the nodes in positions 0 and 1 of
		// the arrangement
		k -= bn[u0] + bn[u1];
		bn[u0] = bn[u1] = 0;

		// this is done because there is no need to
		// fill the first two columns.

		// Now we start filling M at the third column
		for (position_t i = 3ull; i < n; ++i) {
			// node at position i - 1
			const node ui = arr[i - 1ull];
			k -= bn[ui];

			// the row corresponding to node 'u' in M is
			// the same as its position in the sequence.
			// This explains M[pu][*]

			//M[pu][i - 3] = k;
			M[ idx(*pu, *i - 3, n-3) ] = k;

			// clear boolean neighbors so that at the next
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

	const auto process_neighbors =
	[&](const position pu, const node_t v) -> void {
		const position pv = arr[v];
		// 'u' and 'v' is an edge of the graph.
		// In case that arr[u] < arr[v], or, equivalently, pu < arr[v],
		// then 'u' is "in front of" 'v' in the linear arrangement.
		// This explains the first condition 'pu < arr[v]'.
		// The second condition '2 <= arr[v] and arr[v] < n -1' is used
		// to avoid making illegal memory accesses.
		// --
		/*if (pu < arr[v] and 2 <= arr[v] and arr[v] < n - 1) {
			C += K[idx(pu,arr[v]-2, n-3)];
		}*/
		// --
		if (pu < pv and 2 <= pv and pv < n - 1) {
			C += K[idx(pu,pv-2, n-3)];
		}
	};

	for (position pu = 0; pu < n - 3; ++pu) {
		const node u = arr[position_t{pu}];

		if constexpr (std::is_base_of_v<graphs::directed_graph, graph_t>) {
			const neighbourhood& Nu = g.get_out_neighbors(u);
			for (node_t v : Nu) {
				process_neighbors(pu, v);
				if constexpr (decide_upper_bound) {
					if (C > upper_bound) { return DECIDED_C_GT; }
				}
			}
			const neighbourhood& Nu_in = g.get_in_neighbors(u);
			for (node_t v : Nu_in) {
				process_neighbors(pu, v);
				if constexpr (decide_upper_bound) {
					if (C > upper_bound) { return DECIDED_C_GT; }
				}
			}
		}
		else {
			const neighbourhood& Nu = g.get_neighbors(u);
			for (node_t v : Nu) {
				process_neighbors(pu, v);
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

} // -- namespace dyn_prog

// =============================================================================
// CALLS TO THE ALGORITHM
// =============================================================================

// ------------------
// single arrangement

/**
 * @brief Dynamic programming computation of \f$C\f$.
 * @tparam graph_t Type of input graph.
 * @tparam arrangement_t Type of input arrangement.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @returns \f$C_{\pi}(G)\f$ on the input arrangement.
 */
template <class graph_t, class arrangement_t>
[[nodiscard]] uint64_t n_C_dynamic_programming
(const graph_t& g, const arrangement_t& arr)
noexcept
{
	const uint64_t n = g.get_num_nodes();

#if defined DEBUG
	assert(arr.size() == 0 or arr.size() == n);
#endif

	if (n < 4) { return 0; }

	// boolean neighbourhood of nodes
	array<unsigned char> bool_neighs(n);

	const std::size_t n_elems = 2*(n - 3)*(n - 3);
	array<uint64_t> all_memory(n_elems);

	// matrix M (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ M = &all_memory[0];
	// matrix K (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ K = &all_memory[0 + (n - 3)*(n - 3)];

	return dyn_prog::compute<false>(g, arr, bool_neighs.begin(), M,K, 0);
}

// --------------------
// list of arrangements

/**
 * @brief Dynamic programming computation of \f$C\f$.
 * @tparam graph_t Type of input graph.
 * @param g Input graph.
 * @param arrs List of input arrangement.
 * @returns \f$C_{\pi}(G)\f$ on every input arrangement.
 */
template <class graph_t>
[[nodiscard]] std::vector<uint64_t> n_C_dynamic_programming
(const graph_t& g, const std::vector<linear_arrangement>& arrs)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(arrs.size(), 0);
	if (n < 4) { return cs; }

	/* allocate memory */
	const std::size_t n_elems = 2*(n - 3)*(n - 3);
	array<uint64_t> all_memory(n_elems);

	// matrix M (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ M = &all_memory[0];
	// matrix K (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ K = &all_memory[0 + (n - 3)*(n - 3)];

	// boolean neighbourhood of nodes
	array<unsigned char> bool_neighs(n);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < arrs.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(arrs[i].size() == n);
#endif

		// compute C
		cs[i] = dyn_prog::compute<false>
			(g, nonidentity_arr(arrs[i]), bool_neighs.begin(), M,K, 0);

		// contents of 'bool_neighs' is set to 0 inside the function
		//bool_neighs.assign(n, false);
	}

	return cs;
}

// -----------------------------------------------------------------------------
// DECISION

// ------------------
// single arrangement

/**
 * @brief Dynamic programming computation of \f$C\f$.
 * @tparam graph_t Type of input graph.
 * @tparam arrangement_t Type of input arrangement.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param upper_bound Bound used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on the input arrangement if it is less than the
 * upper bound. It returns a value one unit larger than the upper bound otherwise.
 */
template <class graph_t, class arrangement_t>
[[nodiscard]] uint64_t is_n_C_dynamic_programming_lesseq_than
(
	const graph_t& g,
	const arrangement_t& arr,
	const uint64_t upper_bound
)
noexcept
{
	const uint64_t n = g.get_num_nodes();

#if defined DEBUG
	assert(arr.size() == 0 or arr.size() == n);
#endif

	// boolean neighbourhood of nodes
	array<unsigned char> bool_neighs(n);

	const std::size_t n_elems = 2*(n - 3)*(n - 3);
	array<uint64_t> all_memory(n_elems);

	// matrix M (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ M = &all_memory[0];
	// matrix K (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ K = &all_memory[0 + (n - 3)*(n - 3)];

	return dyn_prog::compute<true>(g, arr, bool_neighs.begin(), M,K, upper_bound);
}

// --------------------
// list of arrangements

/**
 * @brief Dynamic programming computation of \f$C\f$ with early termination.
 * @tparam graph_t Type of input graph
 * @param g Input graph.
 * @param arrs List of input arrangement.
 * @param upper_bound Bound used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on every input arrangement if it is less than the
 * upper bound. It returns a value one unit larger than the upper bound otherwise.
 */
template <class graph_t>
[[nodiscard]] std::vector<uint64_t> is_n_C_dynamic_programming_lesseq_than
(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	const uint64_t upper_bound
)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(arrs.size(), 0);
	if (n < 4) { return cs; }

	/* allocate memory */
	const std::size_t n_elems = 2*(n - 3)*(n - 3);
	array<uint64_t> all_memory(n_elems);

	// matrix M (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ M = &all_memory[0];
	// matrix K (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ K = &all_memory[0 + (n - 3)*(n - 3)];

	// boolean neighbourhood of nodes
	array<unsigned char> bool_neighs(n);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < arrs.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(arrs[i].size() == n);
#endif

		// compute C
		cs[i] = dyn_prog::compute<true>
			(g, nonidentity_arr(arrs[i]), bool_neighs.begin(), M,K, upper_bound);

		// contents of 'bool_neighs' is set to 0 inside the function
		//bool_neighs.assign(n, false);
	}

	/* free memory */
	return cs;
}

/**
 * @brief Dynamic programming computation of \f$C\f$ with early termination.
 * @tparam graph_t Type of input graph
 * @param g Input graph.
 * @param arrs List of input arrangement.
 * @param upper_bounds List of bounds used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on every input arrangement if it is less than the
 * corresponding upper bound. It returns a value one unit larger than the upper
 * bound otherwise.
 */
template <class graph_t>
[[nodiscard]] std::vector<uint64_t> is_n_C_dynamic_programming_lesseq_than
(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	const std::vector<uint64_t>& upper_bounds
)
noexcept
{
#if defined DEBUG
	// ensure that there are as many linear arrangements as upper bounds
	assert(arrs.size() == upper_bounds.size());
#endif

	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(arrs.size(), 0);
	if (n < 4) { return cs; }

	/* allocate memory */
	const std::size_t n_elems = 2*(n - 3)*(n - 3);
	array<uint64_t> all_memory(n_elems);

	// matrix M (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ M = &all_memory[0];
	// matrix K (without 3 of its columns and rows) ( size (n-3)*(n-3) )
	uint64_t * const __restrict__ K = &all_memory[0 + (n - 3)*(n - 3)];

	// boolean neighbourhood of nodes
	array<unsigned char> bool_neighs(n);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < arrs.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(arrs[i].size() == n);
#endif

		// compute C
		cs[i] = dyn_prog::compute<true>
			(g, nonidentity_arr(arrs[i]), bool_neighs.begin(), M,K, upper_bounds[i]);

		// contents of 'bool_neighs' is set to 0 inside the function
		//bool_neighs.assign(n, false);
	}

	/* free memory */
	return cs;
}

} // -- namespace crossings
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

		const neighbourhood& Nu = G.get_neighbors(u);
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
