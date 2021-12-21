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

// =============================================================================
// ACTUAL ALGORITHM
// =============================================================================

// When decide_upper_bound is false:
//		returns the number of crossings
// When decide_upper_bound is true:
//		returns m*m + 1 if the number of crossings is greater than the upper_bound
//		returns the number of crossings if the number of crossings is less
//			than the upper_bound
template<class G, bool decide_upper_bound>
uint64_t __compute_C_ladder(
	const G& g,
	const linear_arrangement& pi,
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
		const node u = pi[position_t{pu}];

		// amount of crossings the edges incident to this node and
		// connecting nodes "to the right" of 'u' in the arrangement
		uint64_t S = 0;

		// neighbours of node u, as a list of Boolean values.
		detail::get_bool_neighbours<G>(g, u, bn);

		for (position pv = pu + 1; pv < n; ++pv) {
			const node v = pi[position_t{pv}];
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

// =============================================================================
// CALLS TO ALGORITHM
// =============================================================================

// ------------------
// single arrangement

template<class graph_type>
uint64_t __call_C_ladder(
	const graph_type& g,
	const linear_arrangement& pi
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

	return __compute_C_ladder<graph_type,false>
			(g, pi, boolean_neighborhood.begin(), L1.begin());
}

template<class graph_type>
uint64_t n_C_ladder(
	const graph_type& g,
	const linear_arrangement& pi
)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif
	return detail::call_with_empty_arrangement
			(__call_C_ladder<graph_type>, g, pi);
}

// --------------------
// list of arrangements

template<class graph_type>
std::vector<uint64_t> n_C_ladder(
	const graph_type& g,
	const std::vector<linear_arrangement>& pis
)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(pis.size(), 0);
	if (n < 4) {
		return cs;
	}

	// boolean neighbourhood of nodes
	data_array<unsigned char> boolean_neighborhood(n, 0);
	// array L1 (same as in the pseudocode) ( size n )
	data_array<uint64_t> L1(n, 0);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		cs[i] = __compute_C_ladder<graph_type,false>
				(g, pis[i], boolean_neighborhood.begin(), L1.begin());

		boolean_neighborhood.fill(0);
		L1[n - 1] = 0;
	}

	return cs;
}

// -----------------------------------------------------------------------------
// DECISION

// ------------------
// single arrangement

template<class graph_type>
uint64_t __call_C_ladder_is_lesseq_than(
	const graph_type& g,
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
	data_array<unsigned char> boolean_neighborhood(n, 0);
	// array L1 (same as in the pseudocode) ( size n )
	data_array<uint64_t> L1(n, 0);

	return __compute_C_ladder<graph_type,true>
			(g, pi, boolean_neighborhood.begin(), L1.begin(), upper_bound);
}

template<class graph_type>
uint64_t is_n_C_ladder_lesseq_than(
	const graph_type& g,
	const linear_arrangement& pi,
	uint64_t upper_bound
)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif
	return detail::call_with_empty_arrangement
			(__call_C_ladder_is_lesseq_than<graph_type>, g, pi, upper_bound);
}

// --------------------
// list of arrangements

template<class graph_type>
std::vector<uint64_t> is_n_C_ladder_lesseq_than(
	const graph_type& g,
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

	// boolean neighbourhood of nodes
	data_array<unsigned char> boolean_neighborhood(n, 0);
	// array L1 (same as in the pseudocode) ( size n )
	data_array<uint64_t> L1(n, 0);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		cs[i] = __compute_C_ladder<graph_type,true>
				(g, pis[i], boolean_neighborhood.begin(), L1.begin(), upper_bound);

		for (uint64_t z = 0; z < n; ++z) {
			L1[z] = 0;
			boolean_neighborhood[z] = 0;
		}
		L1[n - 1] = 0;
	}

	return cs;
}

template<class graph_type>
std::vector<uint64_t> is_n_C_ladder_lesseq_than(
	const graph_type& g,
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

	// boolean neighbourhood of nodes
	data_array<unsigned char> boolean_neighborhood(n, 0);
	// array L1 (same as in the pseudocode) ( size n )
	data_array<uint64_t> L1(n, 0);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		boolean_neighborhood.fill(0);

		cs[i] = __compute_C_ladder<graph_type,true>
				(g, pis[i], boolean_neighborhood.begin(), L1.begin(), upper_bounds[i]);

		for (uint64_t z = 0; z < n; ++z) {
			L1[z] = 0;
			boolean_neighborhood[z] = 0;
		}
		L1[n - 1] = 0;
	}

	return cs;
}

} // -- namespace detail
} // -- namespace lal
