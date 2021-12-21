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

// =============================================================================
// ACTUAL ALGORITHM
// =============================================================================

// When decide_upper_bound is false:
//		returns the number of crossings
// When decide_upper_bound is true:
//		returns m*m + 1 if the number of crossings is greater than the upper_bound
//		returns the number of crossings if the number of crossings is less
//			than the upper_bound
template<bool decide_upper_bound>
uint64_t __compute_C_brute_force_undir(
	const graphs::undirected_graph& g, const linear_arrangement& pi,
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
		const neighbourhood& Nu = g.get_neighbours(u.value);
		for (node_t v : Nu) {
			// 'pv' is the position of node 'v'
			const position pv = pi[v];
			if (pu >= pv) { continue; }

			// 'u' and 'v' is a pair of connected nodes such that 'u'
			// is "to the left of" 'v' in the linear arrangement 'seq'

			// iterate through the positions between 'u' and 'v'
			const position begin = pu + 1;
			const position end = pv - 1;

			for (position_t pw = begin; pw <= end; ++pw) {
				// 'w' is the node at position 'pw'
				const node w = pi[pw];
				const neighbourhood& Nw = g.get_neighbours(w);
				for (node_t z : Nw) {
					const position pz = pi[z];

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

// When decide_upper_bound is false:
//		returns the number of crossings
// When decide_upper_bound is true:
//		returns 0 if the number of crossings than the upper_bound
//
template<bool decide_upper_bound = false>
std::conditional_t<decide_upper_bound, uint64_t, void>
__inner_computation_dir
(
	const graphs::directed_graph& g,
	position pu, position pv,
	const linear_arrangement& pi,
	uint64_t& C,
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
		const node w = pi[pw];
		const neighbourhood& Nw_out = g.get_out_neighbours(w);
		for (node_t z : Nw_out) {
			const position pz = pi[z];

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
			const position pz = pi[z];

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

	if constexpr (decide_upper_bound) {
		// none of the conditions above were true, so we must have
		// C <= upper_bound
		return DECIDED_C_LE;
	}
}

// When decide_upper_bound is false:
//		returns the number of crossings
// When decide_upper_bound is true:
//		returns 1 if the number of crossings than the upper_bound
//		returns 0 if the number of crossings than the upper_bound
template<bool decide_upper_bound = false>
uint64_t __compute_C_brute_force_dir(
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
		const neighbourhood& Nu_out = g.get_out_neighbours(u.value);
		for (node_t v : Nu_out) {
			// 'pv' is the position of node 'v'
			const position pv = pi[v];
			if (pu >= pv) { continue; }
			// 'u' and 'v' is a pair of connected nodes such that 'u'
			// is "to the left of" 'v' in the linear arrangement 'seq'

			if constexpr (decide_upper_bound) {
				const uint64_t res =
					__inner_computation_dir<true>(g, pu,pv, pi, C, upper_bound);

				// if decided that C > upper_bound, return
				if (res == DECIDED_C_GT) { return DECIDED_C_GT; }
				// if not, continue
			}
			else {
				__inner_computation_dir<false>(g, pu,pv, pi, C);
			}
		}
		const neighbourhood& Nu_in = g.get_in_neighbours(u.value);
		for (node_t v : Nu_in) {
			// 'pv' is the position of node 'v'
			const position pv = pi[v];
			if (pu >= pv) { continue; }
			// 'u' and 'v' is a pair of connected nodes such that 'u'
			// is "to the left of" 'v' in the linear arrangement 'seq'

			if constexpr (decide_upper_bound) {
				const uint64_t res =
					__inner_computation_dir<true>(g, pu,pv, pi, C, upper_bound);

				// if decided that C > upper_bound, return
				if (res == DECIDED_C_GT) { return DECIDED_C_GT; }
				// if not, continue
			}
			else {
				__inner_computation_dir<false>(g, pu,pv, pi, C);
			}
		}
	}

	// none of the conditions above were true, so we must have
	// C <= upper_bound
	return C;
}

// =============================================================================
// CALLS TO THE ALGORITHM
// =============================================================================

// ------------------
// single arrangement

template<class graph_type>
uint64_t __call_C_brute_force(
	const graph_type& g,
	const linear_arrangement& pi
)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	if (n < 4) { return 0; }

	// compute the number of crossings
	if constexpr (std::is_base_of_v<graphs::undirected_graph, graph_type>) {
		return __compute_C_brute_force_undir<false>(g, pi);
	}
	else {
		return __compute_C_brute_force_dir<false>(g, pi);
	}
}

template<class graph_type>
uint64_t n_C_brute_force(const graph_type& g, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif
	return detail::call_with_empty_arrangement
			(__call_C_brute_force<graph_type>, g, pi);
}

// --------------------
// list of arrangements

template<class graph_type>
std::vector<uint64_t> n_C_brute_force(
	const graph_type& g,
	const std::vector<linear_arrangement>& pis
)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(pis.size(), 0);
	if (n < 4) { return cs; }

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		if constexpr (std::is_base_of_v<graphs::undirected_graph, graph_type>) {
			cs[i] = __compute_C_brute_force_undir<false>(g, pis[i]);
		}
		else {
			cs[i] = __compute_C_brute_force_dir<false>(g, pis[i]);
		}
	}

	return cs;
}

// -----------------------------------------------------------------------------
// DECISION

// ------------------
// single arrangement

template<class graph_type>
uint64_t __call_brute_force_lesseq_than(
	const graph_type& g,
	const linear_arrangement& pi,
	uint64_t upper_bound
)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	if (n < 4) { return 0; }

	// compute the number of crossings
	if constexpr (std::is_base_of_v<graphs::undirected_graph, graph_type>) {
		return
		__compute_C_brute_force_undir<true>(g, pi, upper_bound);
	}
	else {
		return
		__compute_C_brute_force_dir<true>(g, pi, upper_bound);
	}
}

template<class graph_type>
uint64_t is_n_C_brute_force_lesseq_than(
	const graph_type& g,
	const linear_arrangement& pi,
	uint64_t c
)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif
	return detail::call_with_empty_arrangement
			(__call_brute_force_lesseq_than<graph_type>, g, pi, c);
}

// --------------------
// list of arrangements

template<class graph_type>
std::vector<uint64_t> is_n_C_brute_force_lesseq_than(
	const graph_type& g,
	const std::vector<linear_arrangement>& pis,
	uint64_t upper_bound
)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(pis.size(), 0);
	if (n < 4) { return cs; }

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		if constexpr (std::is_base_of_v<graphs::undirected_graph, graph_type>) {
			cs[i] =
			__compute_C_brute_force_undir<true>(g, pis[i], upper_bound);
		}
		else {
			cs[i] =
			__compute_C_brute_force_dir<true>(g, pis[i], upper_bound);
		}
	}

	return cs;
}

template<class graph_type>
std::vector<uint64_t> is_n_C_brute_force_lesseq_than(
	const graph_type& g,
	const std::vector<linear_arrangement>& pis,
	const std::vector<uint64_t>& upper_bounds
)
noexcept
{
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis.size() == upper_bounds.size());
#endif

	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(pis.size(), 0);
	if (n < 4) { return cs; }

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		if constexpr (std::is_base_of_v<graphs::undirected_graph, graph_type>) {
			cs[i] =
			__compute_C_brute_force_undir<true>(g, pis[i], upper_bounds[i]);
		}
		else {
			cs[i] =
			__compute_C_brute_force_dir<true>(g, pis[i], upper_bounds[i]);
		}
	}

	return cs;
}

} // -- namespace detail
} // -- namespace lal
