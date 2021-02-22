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
#include <vector>
using namespace std;

// lal includes
#include <lal/iterators/Q_iterator.hpp>
#include <lal/internal/macros.hpp>
#include <lal/internal/data_array.hpp>

namespace lal {
using namespace graphs;
using namespace iterators;

#define idx(i,j, C) ((i)*(C) + (j))

namespace internal {

inline uint32_t __compute_C_brute_force_undir(
	const undirected_graph& g, const linear_arrangement& pi,
	node * __restrict__ T
)
{
	const uint32_t n = g.num_nodes();
	for (uint32_t i = 0; i < n; ++i) {
		T[ pi[i] ] = i;
	}

	uint32_t C = 0;

	// iterate over the pairs of edges that will potentially cross
	// using the information given in the linear arrangement
	for (node u = 0; u < g.num_nodes(); ++u) {
		// 'pu' is the position of node 'u'
		const position pu = pi[u];
		const neighbourhood& Nu = g.get_neighbours(u);
		for (const node& v : Nu) {
			// 'pv' is the position of node 'v'
			const position pv = pi[v];
			if (pu >= pv) { continue; }

			// 'u' and 'v' is a pair of connected nodes such that 'u'
			// is "to the left of" 'v' in the linear arrangement 'seq'

			// iterate through the positions between 'u' and 'v'
			const position begin = pi[u] + 1;
			const position end = pi[v] - 1;

			for (position pw = begin; pw <= end; ++pw) {
				// 'w' is the node at position 'pw'
				const node w = T[pw];
				const neighbourhood& Nw = g.get_neighbours(w);
				for (const node& z : Nw) {

					// if     pi[w] < pi[z]    then
					// 'w' and 'z' is a pair of connected nodes such that
					// 'w' is "to the left of" 'z' in the random seq 'seq'.
					// Formally: pi[w] < pi[z]

					// Also, by construction: pi[u] < pi[w]
					C += pi[w] < pi[z] and
						 pi[u] < pi[w] and pi[w] < pi[v] and pi[v] < pi[z];
				}
			}
		}
	}

	return C;
}

inline void __inner_computation_dir(
	const directed_graph& g, node u, node v,
	const linear_arrangement& pi, const node * __restrict__ T,
	uint32_t& C
)
{
	// 'u' and 'v' is a pair of connected nodes such that 'u'
	// is "to the left of" 'v' in the linear arrangement 'seq'

	// iterate through the positions between 'u' and 'v'
	const position begin = pi[u] + 1;
	const position end = pi[v] - 1;

	for (position pw = begin; pw <= end; ++pw) {
		// 'w' is the node at position 'pw'
		const node w = T[pw];
		const neighbourhood& Nw_out = g.get_out_neighbours(w);
		for (const node& z : Nw_out) {

			// if     pi[w] < pi[z]    then
			// 'w' and 'z' is a pair of connected nodes such that
			// 'w' is "to the left of" 'z' in the random seq 'seq'.
			// Formally: pi[w] < pi[z]

			// Also, by construction: pi[u] < pi[w]
			C += pi[w] < pi[z] and
				 pi[u] < pi[w] and pi[w] < pi[v] and pi[v] < pi[z];
		}
		const neighbourhood& Nw_in = g.get_in_neighbours(w);
		for (const node& z : Nw_in) {

			// if     pi[w] < pi[z]    then
			// 'w' and 'z' is a pair of connected nodes such that
			// 'w' is "to the left of" 'z' in the random seq 'seq'.
			// Formally: pi[w] < pi[z]

			// Also, by construction: pi[u] < pi[w]
			C += pi[w] < pi[z] and
				 pi[u] < pi[w] and pi[w] < pi[v] and pi[v] < pi[z];
		}
	}
}
inline uint32_t __compute_C_brute_force_dir(
	const directed_graph& g, const linear_arrangement& pi,
	node * __restrict__ T
)
{
	const uint32_t n = g.num_nodes();
	for (uint32_t i = 0; i < n; ++i) {
		T[ pi[i] ] = i;
	}

	uint32_t C = 0;

	// iterate over the pairs of edges that will potentially cross
	// using the information given in the linear arrangement
	for (node u = 0; u < g.num_nodes(); ++u) {
		// 'pu' is the position of node 'u'
		const position pu = pi[u];
		const neighbourhood& Nu_out = g.get_out_neighbours(u);
		for (const node& v : Nu_out) {
			// 'pv' is the position of node 'v'
			const position pv = pi[v];
			if (pu >= pv) { continue; }

			// 'u' and 'v' is a pair of connected nodes such that 'u'
			// is "to the left of" 'v' in the linear arrangement 'seq'
			__inner_computation_dir(g,u,v,pi,T,C);
		}
		const neighbourhood& Nu_in = g.get_in_neighbours(u);
		for (const node& v : Nu_in) {
			// 'pv' is the position of node 'v'
			const position pv = pi[v];
			if (pu >= pv) { continue; }

			// 'u' and 'v' is a pair of connected nodes such that 'u'
			// is "to the left of" 'v' in the linear arrangement 'seq'
			__inner_computation_dir(g,u,v,pi,T,C);
		}
	}

	return C;
}

// -----------------------------------------------------------------------------

// T: translation table, inverse of pi:
// T[p] = u <-> at position p we find node u
template<typename GRAPH>
inline uint32_t __call_C_brute_force
(const GRAPH& g, const linear_arrangement& pi){
	const uint32_t n = g.num_nodes();
	if (n < 4) { return 0; }

	/* allocate memory */

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p
	data_array<node> T(n);

	// compute the number of crossings
	if constexpr (std::is_base_of_v<undirected_graph, GRAPH>) {
		return __compute_C_brute_force_undir(g, pi, T.data);
	}
	else {
		return __compute_C_brute_force_dir(g, pi, T.data);
	}
}

uint32_t n_C_brute_force(const undirected_graph& g, const linear_arrangement& pi)
{
#if defined DEBUG
	assert(pi.size() == 0 or g.num_nodes() == pi.size());
#endif
	return internal::call_with_empty_arrangement
			(__call_C_brute_force<undirected_graph>, g, pi);
}

uint32_t n_C_brute_force(const directed_graph& g, const linear_arrangement& pi)
{
#if defined DEBUG
	assert(pi.size() == 0 or g.num_nodes() == pi.size());
#endif
	return internal::call_with_empty_arrangement
			(__call_C_brute_force<directed_graph>, g, pi);
}

// -----------------------------------------------------------------------------

template<typename GRAPH>
vector<uint32_t> n_C_brute_force_list
(const GRAPH& g, const vector<linear_arrangement>& pis)
{
	const uint32_t n = g.num_nodes();

	vector<uint32_t> cs(pis.size(), 0);
	if (n < 4) { return cs; }

	// inverse function of the linear arrangement:
	// T[p] = u <-> node u is at position p
	data_array<node> T(n);

	/* compute C for every linear arrangement */
	for (size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		if constexpr (std::is_base_of_v<undirected_graph, GRAPH>) {
			cs[i] = __compute_C_brute_force_undir(g, pis[i], T.data);
		}
		else {
			cs[i] = __compute_C_brute_force_dir(g, pis[i], T.data);
		}
	}

	return cs;
}

vector<uint32_t> n_C_brute_force_list
(const directed_graph& g, const vector<linear_arrangement>& pis) {
	return n_C_brute_force_list<directed_graph>(g, pis);
}
vector<uint32_t> n_C_brute_force_list
(const undirected_graph& g, const vector<linear_arrangement>& pis) {
	return n_C_brute_force_list<undirected_graph>(g, pis);
}

} // -- namespace internal
} // -- namespace lal
