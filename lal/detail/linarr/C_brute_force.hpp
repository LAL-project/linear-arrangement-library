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
#include <lal/iterators/Q_iterator.hpp>
#include <lal/detail/data_array.hpp>
#include <lal/detail/macros/call_with_empty_arr.hpp>

#define idx(i,j, C) ((i)*(C) + (j))
#define DECIDED_C_GT (upper_bound + 1)
#define DECIDED_C_LE C

namespace lal {
namespace detail {

namespace __lal {

// =============================================================================
// ACTUAL ALGORITHM
// =============================================================================

/**
 * @brief Brute force computation of \f$C\f$ for undirected graphs.
 *
 * When template parameter @e decide_upper_bound is false, the function returns
 * the number of crossings.
 * @tparam decide_upper_bound Boolean value to choose the nature of the return type.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param upper_bound Upper bound on the number of crossings.
 * @returns When @e decide_upper_bound is true, the return value is:
 * - one unit larger than the upper bound passed as parameter if \f$C>\f$ upper bound.
 * - \f$C\f$ if the number of crossings is less or equal than the upper bound.
 */
template<bool decide_upper_bound>
uint64_t compute_C_brute_force(
	const graphs::undirected_graph& g, const linear_arrangement& arr,
	uint64_t upper_bound = 0
)
noexcept
{
	uint64_t C = 0;

	// iterate over the pairs of edges that will potentially cross
	// using the information given in the linear arrangement
	for (node_t u = 0ULL; u < g.get_num_nodes(); ++u) {
		// 'pu' is the position of node 'u'
		const position pu = arr[u];
		const neighbourhood& Nu = g.get_neighbours(*u);
		for (node_t v : Nu) {
			// 'pv' is the position of node 'v'
			const position pv = arr[v];
			if (pu >= pv) { continue; }

			// 'u' and 'v' is a pair of connected nodes such that 'u'
			// is "to the left of" 'v' in the linear arrangement 'seq'

			// iterate through the positions between 'u' and 'v'
			const position begin = pu + 1;
			const position end = pv - 1;

			for (position_t pw = begin; pw <= end; ++pw) {
				// 'w' is the node at position 'pw'
				const node w = arr[pw];
				const neighbourhood& Nw = g.get_neighbours(w);
				for (node_t z : Nw) {
					const position pz = arr[z];

					// if     pi[w] < pi[z]    then
					// 'w' and 'z' is a pair of connected nodes such that
					// 'w' is "to the left of" 'z' in the random seq 'seq'.
					// Formally: pi[w] < pi[z]

					// Also, by construction: pi[u] < pi[w]
					C += pw < pz and
						 pu < pw and pw < pv and pv < pz;

					if constexpr (decide_upper_bound) {
						if (C > upper_bound) { return DECIDED_C_GT; }
					}
				}
			}
		}
	}

	// none of the conditions above were true, so we must have
	// C <= upper_bound
	return C;
}

/**
 * @brief Brute force computation of \f$C\f$ for directed graphs.
 *
 * When template parameter @e decide_upper_bound is false, the function returns
 * the number of crossings.
 * @tparam decide_upper_bound Boolean value to choose the nature of the return type.
 * @param g Input graph.
 * @param pu Starting position of the computation.
 * @param pv Ending position of the computation.
 * @param arr Input arrangement.
 * @param C Current number of crossings.
 * @param upper_bound Upper bound on the number of crossings.
 * @returns When @e decide_upper_bound is true, the return value is:
 * - one unit larger than the upper bound passed as parameter if \f$C>\f$ upper bound.
 * - \f$C\f$ if the number of crossings is less or equal than the upper bound.
 */
template<bool decide_upper_bound>
[[nodiscard]] uint64_t inner_computation
(
	const graphs::directed_graph& g,
	position pu, position pv,
	const linear_arrangement& arr,
	uint64_t C,
	uint64_t upper_bound = 0
)
noexcept
{
	// 'u' and 'v' is a pair of connected nodes such that 'u'
	// is "to the left of" 'v' in the linear arrangement 'seq'

	// iterate through the positions between 'u' and 'v'
	const position begin = pu + 1;
	const position end = pv - 1;

	for (position_t pw = begin; pw <= end; ++pw) {
		// 'w' is the node at position 'pw'
		const node w = arr[pw];
		const neighbourhood& Nw_out = g.get_out_neighbours(w);
		for (node_t z : Nw_out) {
			const position pz = arr[z];

			// if     pi[w] < pi[z]    then
			// 'w' and 'z' is a pair of connected nodes such that
			// 'w' is "to the left of" 'z' in the random seq 'seq'.
			// Formally: pi[w] < pi[z]

			// Also, by construction: pi[u] < pi[w]
			C += pw < pz and
				 pu < pw and pw < pv and pv < pz;

			if constexpr (decide_upper_bound) {
				if (C > upper_bound) { return DECIDED_C_GT; }
			}
		}
		const neighbourhood& Nw_in = g.get_in_neighbours(w);
		for (node_t z : Nw_in) {
			const position pz = arr[z];

			// if     pi[w] < pi[z]    then
			// 'w' and 'z' is a pair of connected nodes such that
			// 'w' is "to the left of" 'z' in the random seq 'seq'.
			// Formally: pi[w] < pi[z]

			// Also, by construction: pi[u] < pi[w]
			C += pw < pz and
				 pu < pw and pw < pv and pv < pz;

			if constexpr (decide_upper_bound) {
				if (C > upper_bound) { return DECIDED_C_GT; }
			}
		}
	}

	// none of the conditions above were true, so we must have
	// C <= upper_bound
	return C;
}

/**
 * @brief Brute force computation of \f$C\f$ for directed graphs.
 *
 * When template parameter @e decide_upper_bound is false, the function returns
 * the number of crossings.
 * @tparam decide_upper_bound Boolean value to choose the nature of the return type.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param upper_bound Upper bound on the number of crossings.
 * @returns When @e decide_upper_bound is true, the return value is:
 * - one unit larger than the upper bound passed as parameter if \f$C>\f$ upper bound.
 * - \f$C\f$ if the number of crossings is less or equal than the upper bound.
 */
template<bool decide_upper_bound = false>
uint64_t compute_C_brute_force(
	const graphs::directed_graph& g, const linear_arrangement& pi,
	uint64_t upper_bound = 0
)
noexcept
{
	uint64_t C = 0;

	// iterate over the pairs of edges that will potentially cross
	// using the information given in the linear arrangement
	for (node_t u = 0ULL; u < g.get_num_nodes(); ++u) {
		// 'pu' is the position of node 'u'
		const position pu = pi[u];
		const neighbourhood& Nu_out = g.get_out_neighbours(*u);
		for (node_t v : Nu_out) {
			// 'pv' is the position of node 'v'
			const position pv = pi[v];
			if (pu >= pv) { continue; }
			// 'u' and 'v' is a pair of connected nodes such that 'u'
			// is "to the left of" 'v' in the linear arrangement 'seq'

			if constexpr (decide_upper_bound) {
				C += inner_computation<true>(g, pu,pv, pi, C, upper_bound);

				// if decided that C > upper_bound, return
				if (C > upper_bound) { return DECIDED_C_GT; }
				// if not, continue
			}
			else {
				C += inner_computation<false>(g, pu,pv, pi, C);
			}
		}
		const neighbourhood& Nu_in = g.get_in_neighbours(*u);
		for (node_t v : Nu_in) {
			// 'pv' is the position of node 'v'
			const position pv = pi[v];
			if (pu >= pv) { continue; }
			// 'u' and 'v' is a pair of connected nodes such that 'u'
			// is "to the left of" 'v' in the linear arrangement 'seq'

			if constexpr (decide_upper_bound) {
				C += inner_computation<true>(g, pu,pv, pi, C, upper_bound);

				// if decided that C > upper_bound, return
				if (C > upper_bound) { return DECIDED_C_GT; }
				// if not, continue
			}
			else {
				C += inner_computation<false>(g, pu,pv, pi, C);
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

/**
 * @brief Brute force computation of \f$C\f$.
 * @tparam graph_t Type of input graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @returns \f$C_{\pi}(G)\f$ on the input arrangement.
 */
template<class graph_t>
uint64_t call_C_brute_force(const graph_t& g, const linear_arrangement& arr)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	if (n < 4) { return 0; }

	// compute the number of crossings
	return __lal::compute_C_brute_force<false>(g, arr);
}

/**
 * @brief Brute force computation of \f$C\f$.
 *
 * Calls function @ref lal::detail::call_C_brute_force.
 * @tparam graph_t Type of input graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @returns \f$C_{\pi}(G)\f$ on the input arrangement.
 */
template<class graph_t>
uint64_t n_C_brute_force(const graph_t& g, const linear_arrangement& arr)
noexcept
{
#if defined DEBUG
	assert(arr.size() == 0 or g.get_num_nodes() == arr.size());
#endif
	return detail::call_with_empty_arrangement
			(call_C_brute_force<graph_t>, g, arr);
}

// --------------------
// list of arrangements

/**
 * @brief Brute force computation of \f$C\f$.
 * @tparam graph_t Type of input graph.
 * @param g Input graph.
 * @param arrs List of input arrangement.
 * @returns \f$C_{\pi}(G)\f$ on every input arrangement.
 */
template<class graph_t>
std::vector<uint64_t> n_C_brute_force
(const graph_t& g, const std::vector<linear_arrangement>& arrs)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(arrs.size());
	if (n < 4) {
		// fill only when necessary
		std::fill(cs.begin(), cs.end(), 0);
		return cs;
	}

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < arrs.size(); ++i) {

#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(arrs[i].size() == n);
#endif

		// compute C
		cs[i] = __lal::compute_C_brute_force<false>(g, arrs[i]);
	}

	return cs;
}

// -----------------------------------------------------------------------------
// DECISION

// ------------------
// single arrangement

/**
 * @brief Brute force computation of \f$C\f$ with early termination.
 * @tparam graph_t Type of input graph.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param upper_bound Bound used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on the input arrangement if it is less than the
 * upper bound. It returns a value one unit larger than the upper bound otherwise.
 */
template<class graph_t>
uint64_t call_C_brute_force_lesseq_than
(const graph_t& g, const linear_arrangement& arr, uint64_t upper_bound)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	if (n < 4) { return 0; }

	// compute the number of crossings
	return __lal::compute_C_brute_force<true>(g, arr, upper_bound);
}

/**
 * @brief Brute force computation of \f$C\f$ with early termination.
 *
 * Calls function @ref lal::detail::call_C_brute_force_lesseq_than.
 * @tparam graph_t Type of input graph
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param upper_bound Bound used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on the input arrangement if it is less than the
 * upper bound. It returns a value one unit larger than the upper bound otherwise.
 */
template<class graph_t>
uint64_t is_n_C_brute_force_lesseq_than
(const graph_t& g, const linear_arrangement& arr, uint64_t upper_bound)
noexcept
{
#if defined DEBUG
	assert(arr.size() == 0 or g.get_num_nodes() == arr.size());
#endif
	return detail::call_with_empty_arrangement
			(call_C_brute_force_lesseq_than<graph_t>, g, arr, upper_bound);
}

// --------------------
// list of arrangements

/**
 * @brief Brute force computation of \f$C\f$ with early termination.
 * @tparam graph_t Type of input graph
 * @param g Input graph.
 * @param arrs List of input arrangement.
 * @param upper_bound Bound used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on every input arrangement if it is less than the
 * upper bound. It returns a value one unit larger than the upper bound otherwise.
 */
template<class graph_t>
std::vector<uint64_t> is_n_C_brute_force_lesseq_than
(const graph_t& g, const std::vector<linear_arrangement>& arrs, uint64_t upper_bound)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(arrs.size());
	if (n < 4) {
		std::fill(cs.begin(), cs.end(), 0);
		return cs;
	}

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < arrs.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(arrs[i].size() == n);
#endif

		// compute C
		cs[i] = __lal::compute_C_brute_force<true>(g, arrs[i], upper_bound);
	}

	return cs;
}

/**
 * @brief Brute force computation of \f$C\f$ with early termination.
 * @tparam graph_t Type of input graph
 * @param g Input graph.
 * @param arrs List of input arrangement.
 * @param upper_bounds List of bounds used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on every input arrangement if it is less than the
 * corresponding upper bound. It returns a value one unit larger than the upper
 * bound otherwise.
 */
template<class graph_t>
std::vector<uint64_t> is_n_C_brute_force_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	const std::vector<uint64_t>& upper_bounds
)
noexcept
{
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(arrs.size() == upper_bounds.size());
#endif

	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(arrs.size());
	if (n < 4) {
		std::fill(cs.begin(), cs.end(), 0);
		return cs;
	}

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < arrs.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(arrs[i].size() == n);
#endif

		// compute C
		cs[i] = __lal::compute_C_brute_force<true>(g, arrs[i], upper_bounds[i]);
	}

	return cs;
}

} // -- namespace detail
} // -- namespace lal
