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
#include <cstring> // for 'memset' below
#include <vector>

// lal includes
#include <lal/detail/macros/call_with_empty_arr.hpp>
#include <lal/detail/graphs/utils.hpp>
#include <lal/detail/data_array.hpp>

#define DECIDED_C_GT (upper_bound + 1)
#define DECIDED_C_LE C

namespace lal {
namespace detail {
namespace crossings {

/**
 * @brief Namespace for the "ladder" algorithm to calculate \f$C\f$.
 *
 * See \cite Alemany2019a.
 */
namespace ladder {

// =============================================================================
// ACTUAL ALGORITHM
// =============================================================================

/**
 * @brief Ladder computation of \f$C\f$ for undirected graphs.
 *
 * When template parameter @e decide_upper_bound is false, the function returns
 * the number of crossings.
 * @tparam decide_upper_bound Boolean value to choose the nature of the return type.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param bn Array of size @e n. Boolean neighbourhood of a vertex @e u: @e bn[i]
 * is 1 if vertex @e u and vertex @e i are adjacent.
 * @param L1 See \cite Alemany2019a for details.
 * @param upper_bound Upper bound on the number of crossings.
 * @returns When @e decide_upper_bound is true, the return value is:
 * - one unit larger than the upper bound passed as parameter if \f$C>\f$ upper bound.
 * - \f$C\f$ if the number of crossings is less or equal than the upper bound.
 */
template<class graph_t, bool decide_upper_bound>
uint64_t compute(
	const graph_t& g,
	const linear_arrangement& arr,
	unsigned char * const __restrict__ bn,
	uint64_t * const __restrict__ L1,
	uint64_t upper_bound = 0
)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	/* compute number of crossings */
	uint64_t C = 0;

	// no need to reach the last position of the arrangement
	for (position pu = 0; pu < n - 1; ++pu) {
		const node u = arr[position_t{pu}];

		// amount of crossings the edges incident to this node and
		// connecting nodes "to the right" of 'u' in the arrangement
		uint64_t S = 0;

		// neighbours of node u, as a list of Boolean values.
		detail::get_bool_neighbours<graph_t>(g, u, bn);

		for (position pv = pu + 1; pv < n; ++pv) {
			const node v = arr[position_t{pv}];
			S += L1[pv];

			// --
			/*if (bn[v]) {
				C += S - L1[q];
				++L1[q];
			}*/
			C += bn[v]*(S - L1[pv]);
			L1[pv] += bn[v];
			// --

			if constexpr (decide_upper_bound) {
				if (C > upper_bound) {
					return DECIDED_C_GT;
				}
			}

			bn[v] = 0;
		}

		L1[pu] = 0;
	}

	// none of the conditions above were true, so we must have
	// C <= upper_bound
	return C;
}

} // -- namespace ladder

// =============================================================================
// CALLS TO ALGORITHM
// =============================================================================

// ------------------
// single arrangement

/**
 * @brief Ladder computation of \f$C\f$.
 * @tparam graph_t Type of input graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @returns \f$C_{\pi}(G)\f$ on the input arrangement.
 */
template<class graph_t>
uint64_t call_ladder(const graph_t& g, const linear_arrangement& arr)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	if (n < 4) {
		return 0;
	}

	// boolean neighbourhood of nodes
	data_array<unsigned char> boolean_neighborhood(n, 0);
	// array L1 (same as in the pseudocode) ( size n )
	data_array<uint64_t> L1(n, 0);

	return ladder::compute<graph_t, false>
			(g, arr, boolean_neighborhood.begin(), L1.begin());
}

/**
 * @brief Ladder computation of \f$C\f$.
 *
 * Calls function @ref lal::detail::call_ladder.
 * @tparam graph_t Type of input graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @returns \f$C_{\pi}(G)\f$ on the input arrangement.
 */
template<class graph_t>
uint64_t n_C_ladder(const graph_t& g, const linear_arrangement& arr)
noexcept
{
#if defined DEBUG
	assert(arr.size() == 0 or g.get_num_nodes() == arr.size());
#endif
	return detail::call_with_empty_arrangement
			(call_ladder<graph_t>, g, arr);
}

// --------------------
// list of arrangements

/**
 * @brief Ladder computation of \f$C\f$.
 * @tparam graph_t Type of input graph.
 * @param g Input graph.
 * @param arrs List of input arrangement.
 * @returns \f$C_{\pi}(G)\f$ on every input arrangement.
 */
template<class graph_t>
std::vector<uint64_t> n_C_ladder
(const graph_t& g, const std::vector<linear_arrangement>& arrs)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(arrs.size(), 0);
	if (n < 4) {
		return cs;
	}

	// boolean neighbourhood of nodes
	data_array<unsigned char> boolean_neighborhood(n, 0);
	// array L1 (same as in the pseudocode) ( size n )
	data_array<uint64_t> L1(n, 0);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < arrs.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(arrs[i].size() == n);
#endif

		// compute C
		cs[i] = ladder::compute<graph_t, false>
				(g, arrs[i], boolean_neighborhood.begin(), L1.begin());

		boolean_neighborhood.fill(0);
		L1[n - 1] = 0;
	}

	return cs;
}

// -----------------------------------------------------------------------------
// DECISION

// ------------------
// single arrangement

/**
 * @brief Ladder computation of \f$C\f$ with early termination.
 * @tparam graph_t Type of input graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param upper_bound Bound used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on the input arrangement if it is less than the
 * upper bound. It returns a value one unit larger than the upper bound otherwise.
 */
template<class graph_t>
uint64_t call_ladder_is_lesseq_than(
	const graph_t& g,
	const linear_arrangement& arr,
	uint64_t upper_bound
)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	if (n < 4) {
		return 0;
	}

	// boolean neighbourhood of nodes
	data_array<unsigned char> boolean_neighborhood(n, 0);
	// array L1 (same as in the pseudocode) ( size n )
	data_array<uint64_t> L1(n, 0);

	return ladder::compute<graph_t, true>
			(g, arr, boolean_neighborhood.begin(), L1.begin(), upper_bound);
}

/**
 * @brief Ladder computation of \f$C\f$ with early termination.
 *
 * Calls function @ref lal::detail::call_ladder_lesseq_than.
 * @tparam graph_t Type of input graph
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param upper_bound Bound used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on the input arrangement if it is less than the
 * upper bound. It returns a value one unit larger than the upper bound otherwise.
 */
template<class graph_t>
uint64_t is_n_C_ladder_lesseq_than(
	const graph_t& g,
	const linear_arrangement& arr,
	uint64_t upper_bound
)
noexcept
{
#if defined DEBUG
	assert(arr.size() == 0 or g.get_num_nodes() == arr.size());
#endif
	return detail::call_with_empty_arrangement
			(call_ladder_is_lesseq_than<graph_t>, g, arr, upper_bound);
}

// --------------------
// list of arrangements

/**
 * @brief Ladder computation of \f$C\f$ with early termination.
 * @tparam graph_t Type of input graph
 * @param g Input graph.
 * @param arrs List of input arrangement.
 * @param upper_bound Bound used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on every input arrangement if it is less than the
 * upper bound. It returns a value one unit larger than the upper bound otherwise.
 */
template<class graph_t>
std::vector<uint64_t> is_n_C_ladder_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	uint64_t upper_bound
)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(arrs.size(), 0);
	if (n < 4) {
		return cs;
	}

	// boolean neighbourhood of nodes
	data_array<unsigned char> boolean_neighborhood(n, 0);
	// array L1 (same as in the pseudocode) ( size n )
	data_array<uint64_t> L1(n, 0);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < arrs.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(arrs[i].size() == n);
#endif

		// compute C
		cs[i] = ladder::compute<graph_t, true>
				(g, arrs[i], boolean_neighborhood.begin(), L1.begin(), upper_bound);

		for (uint64_t z = 0; z < n; ++z) {
			L1[z] = 0;
			boolean_neighborhood[z] = 0;
		}
		L1[n - 1] = 0;
	}

	return cs;
}

/**
 * @brief Ladder computation of \f$C\f$ with early termination.
 * @tparam graph_t Type of input graph
 * @param g Input graph.
 * @param arrs List of input arrangement.
 * @param upper_bounds List of bounds used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on every input arrangement if it is less than the
 * corresponding upper bound. It returns a value one unit larger than the upper
 * bound otherwise.
 */
template<class graph_t>
std::vector<uint64_t> is_n_C_ladder_lesseq_than(
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
	if (n < 4) {
		return cs;
	}

	// boolean neighbourhood of nodes
	data_array<unsigned char> boolean_neighborhood(n, 0);
	// array L1 (same as in the pseudocode) ( size n )
	data_array<uint64_t> L1(n, 0);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < arrs.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(arrs[i].size() == n);
#endif

		// compute C
		boolean_neighborhood.fill(0);

		cs[i] = ladder::compute<graph_t, true>
				(g, arrs[i], boolean_neighborhood.begin(), L1.begin(), upper_bounds[i]);

		for (uint64_t z = 0; z < n; ++z) {
			L1[z] = 0;
			boolean_neighborhood[z] = 0;
		}
		L1[n - 1] = 0;
	}

	return cs;
}

} // -- namespace crossings
} // -- namespace detail
} // -- namespace lal
