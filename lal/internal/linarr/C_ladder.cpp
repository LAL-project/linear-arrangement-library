/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
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
#include <lal/internal/data_array.hpp>
#include <lal/graphs/output.hpp>

#define to_uint32(x) static_cast<uint32_t>(x)
#define DECIDED_C_GT (g.get_num_edges()*g.get_num_edges())
#define DECIDED_C_LE C

namespace lal {
using namespace graphs;

namespace internal {

template<class G, bool decide_upper_bound>
inline uint32_t __compute_C_ladder(
	const G& g, const linear_arrangement& pi,
	char * const __restrict__ bn,
	uint32_t * const __restrict__ inv_pi,
	uint32_t * const __restrict__ L1,
	uint32_t upper_bound = 0
)
{
	if constexpr (not decide_upper_bound) {
		UNUSED(upper_bound);
	}

	const uint32_t n = g.get_num_nodes();
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
			C += to_uint32(bn[v])*(S - L1[q]);
			L1[q] += to_uint32(bn[v]);
			// --

			if constexpr (decide_upper_bound) {
				if (C > upper_bound) {
					return DECIDED_C_GT;
				}
			}

			bn[v] = 0;
		}

		L1[p] = 0;
	}

	return C;
}

// =============================================================================
// CALCULATION
// =============================================================================

// ------------------
// single arrangement

template<class G>
inline uint32_t __call_C_ladder(
	const G& g,
	const linear_arrangement& pi
)
{
	const uint32_t n = g.get_num_nodes();
	if (n < 4) {
		return 0;
	}

	// boolean neighbourhood of nodes
	data_array<char> bool_neighs(n, 0);

	const uint32_t n_elems = n + n;
	data_array<uint32_t> all_memory(n_elems, 0);

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p ( size n )
	uint32_t * const __restrict__ T = &all_memory[0];
	// array L1 (same as in the pseudocode) ( size n )
	uint32_t * const __restrict__ L1 = &all_memory[n];

	return __compute_C_ladder<G,false>(g, pi, bool_neighs.data, T,L1);
}

uint32_t n_C_ladder(
	const directed_graph& g,
	const linear_arrangement& pi
)
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif
	return internal::call_with_empty_arrangement
			(__call_C_ladder<directed_graph>, g, pi);
}

uint32_t n_C_ladder(
	const undirected_graph& g,
	const linear_arrangement& pi
)
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif
	return internal::call_with_empty_arrangement
			(__call_C_ladder<undirected_graph>, g, pi);
}

// --------------------
// list of arrangements

template<class G>
vector<uint32_t> n_C_ladder(
	const G& g,
	const vector<linear_arrangement>& pis
)
{
	const uint32_t n = g.get_num_nodes();

	vector<uint32_t> cs(pis.size(), 0);
	if (n < 4) {
		return cs;
	}

	data_array<char> boolean_neighborhood(n, 0);
	data_array<uint32_t> all_memory(n + n, 0);

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p ( size n )
	uint32_t * const __restrict__ T = &all_memory[0];
	// array L1 (same as in the pseudocode) ( size n )
	uint32_t * const __restrict__ L1 = &all_memory[n];

	/* compute C for every linear arrangement */
	for (size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		cs[i] = __compute_C_ladder<G,false>
				(g, pis[i], boolean_neighborhood.data, T,L1);

		boolean_neighborhood.fill(0);
		L1[n - 1] = 0;
	}

	return cs;
}

vector<uint32_t> n_C_ladder(
	const directed_graph& g,
	const vector<linear_arrangement>& pis
)
{
	return n_C_ladder<directed_graph>(g, pis);
}
vector<uint32_t> n_C_ladder(
	const undirected_graph& g,
	const vector<linear_arrangement>& pis
)
{
	return n_C_ladder<undirected_graph>(g, pis);
}

// =============================================================================
// DECISION
// =============================================================================

// ------------------
// single arrangement

template<class G>
inline uint32_t __call_C_ladder_is_lesseq_than(
	const G& g,
	const linear_arrangement& pi,
	uint32_t upper_bound
)
{
	const uint32_t n = g.get_num_nodes();
	if (n < 4) {
		return 0;
	}

	// boolean neighbourhood of nodes
	data_array<char> bool_neighs(n, 0);
	data_array<uint32_t> all_memory(n + n, 0);

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p ( size n )
	uint32_t * const __restrict__ T = &all_memory[0];
	// array L1 (same as in the pseudocode) ( size n )
	uint32_t * const __restrict__ L1 = &all_memory[n];

	return __compute_C_ladder<G,true>(g, pi, bool_neighs.data, T,L1, upper_bound);
}

uint32_t is_n_C_ladder_lesseq_than(
	const directed_graph& g,
	const linear_arrangement& pi,
	uint32_t upper_bound
)
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif
	return internal::call_with_empty_arrangement
			(__call_C_ladder_is_lesseq_than<directed_graph>, g, pi, upper_bound);
}

uint32_t is_n_C_ladder_lesseq_than(
	const undirected_graph& g,
	const linear_arrangement& pi,
	uint32_t upper_bound
)
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif
	return internal::call_with_empty_arrangement
			(__call_C_ladder_is_lesseq_than<undirected_graph>, g, pi, upper_bound);
}

// --------------------
// list of arrangements

template<class G>
vector<uint32_t> is_n_C_ladder_lesseq_than(
	const G& g,
	const vector<linear_arrangement>& pis,
	uint32_t upper_bound
)
{
	const uint32_t n = g.get_num_nodes();

	vector<uint32_t> cs(pis.size(), 0);
	if (n < 4) {
		return cs;
	}

	data_array<char> boolean_neighborhood(n, 0);
	data_array<uint32_t> all_memory(n + n, 0);

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p ( size n )
	uint32_t * const __restrict__ T = &all_memory[0];
	// array L1 (same as in the pseudocode) ( size n )
	uint32_t * const __restrict__ L1 = &all_memory[n];

	/* compute C for every linear arrangement */
	for (size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		cs[i] = __compute_C_ladder<G,true>
				(g, pis[i], boolean_neighborhood.data, T,L1, upper_bound);

		for (uint32_t z = 0; z < n; ++z) {
			L1[z] = 0;
			boolean_neighborhood[z] = 0;
		}
		L1[n - 1] = 0;
	}

	return cs;
}

vector<uint32_t> is_n_C_ladder_lesseq_than(
	const directed_graph& g,
	const vector<linear_arrangement>& pis,
	uint32_t upper_bound
)
{
	return is_n_C_ladder_lesseq_than<directed_graph>
			(g, pis, upper_bound);
}

vector<uint32_t> is_n_C_ladder_lesseq_than(
	const undirected_graph& g,
	const vector<linear_arrangement>& pis,
	uint32_t upper_bound
)
{
	return is_n_C_ladder_lesseq_than<undirected_graph>
			(g, pis, upper_bound);
}

template<typename G>
vector<uint32_t> is_n_C_ladder_lesseq_than(
	const G& g,
	const vector<linear_arrangement>& pis,
	const vector<uint32_t>& upper_bounds
)
{
#if defined DEBUG
	// ensure that there are as many linear arrangements as upper bounds
	assert(pis.size() == upper_bounds.size());
#endif

	const uint32_t n = g.get_num_nodes();

	vector<uint32_t> cs(pis.size(), 0);
	if (n < 4) {
		return cs;
	}

	data_array<char> boolean_neighborhood(n, 0);
	data_array<uint32_t> all_memory(n + n, 0);

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p ( size n )
	uint32_t * const __restrict__ T = &all_memory[0];
	// array L1 (same as in the pseudocode) ( size n )
	uint32_t * const __restrict__ L1 = &all_memory[n];

	/* compute C for every linear arrangement */
	for (size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		boolean_neighborhood.fill(0);

		cs[i] = __compute_C_ladder<G,true>
				(g, pis[i], boolean_neighborhood.data, T,L1, upper_bounds[i]);

		for (uint32_t z = 0; z < n; ++z) {
			L1[z] = 0;
			boolean_neighborhood[z] = 0;
		}
		L1[n - 1] = 0;
	}

	return cs;
}

vector<uint32_t> is_n_C_ladder_lesseq_than(
	const directed_graph& g,
	const vector<linear_arrangement>& pis,
	const vector<uint32_t>& upper_bounds
)
{
	return is_n_C_ladder_lesseq_than<directed_graph>
			(g, pis, upper_bounds);
}
vector<uint32_t> is_n_C_ladder_lesseq_than(
	const undirected_graph& g,
	const vector<linear_arrangement>& pis,
	const vector<uint32_t>& upper_bounds
)
{
	return is_n_C_ladder_lesseq_than<undirected_graph>
			(g, pis, upper_bounds);
}

} // -- namespace internal
} // -- namespace lal
