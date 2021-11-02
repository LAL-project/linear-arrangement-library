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
#include <map>

// lal includes
#include <lal/graphs/graph.hpp>
#include <lal/detail/macros/call_with_empty_arr.hpp>
#include <lal/detail/avl.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/data_array.hpp>

typedef std::pair<uint64_t,lal::edge> indexed_edge;

#define edge_sorted_by_vertex_index(u,v) (u < v ? edge(u,v) : edge(v,u) )
#define DECIDED_C_GT (g.get_num_edges()*g.get_num_edges() + 1)
#define DECIDED_C_LE C

namespace lal {
namespace detail {

// =============================================================================
// ACTUAL ALGORITHM
// =============================================================================

inline
void fill_adjP_adjN(
	const graphs::graph& g, const linear_arrangement& pi,
	std::vector<neighbourhood>& adjP,
	std::vector<std::vector<indexed_edge>>& adjN,
	std::size_t * const size_adjN_u
)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	// Retrieve all edges of the graph to sort
	std::vector<edge> edges = g.get_edges();

	// sort edges of the graph by increasing edge length
	detail::counting_sort<edge, std::vector<edge>::iterator, countingsort::increasing_t>
	(
	edges.begin(), edges.end(),
	n-1, // length of the longest edge
	edges.size(),
	[&](const edge& e) -> std::size_t {
		const node_t u = e.first;
		const node_t v = e.second;
		++size_adjN_u[u.value];

#define my_abs_diff(a,b) (a < b ? b - a : a - b)
		return my_abs_diff(pi[u], pi[v]);
	}
	);

	// initialise adjN
	for (node u = 0; u < n; ++u) {
		// divide by two because the 'key' function in the call to
		// the sorting function is called twice for every edge
#if defined DEBUG
		assert( (size_adjN_u[u]%2) == 0 );
#endif
		size_adjN_u[u] /= 2;
		adjN[u].resize(size_adjN_u[u]);
	}

	// fill adjP and adjN at the same time
	for (const auto& [uu, vv] : edges) {
		// pi[u] < pi[v]
		const auto [u,v] =
			(pi[node_t{uu}] < pi[node_t{vv}] ? edge(uu,vv) : edge(vv,uu));

		// oriented edge (u,v) "enters" node v
		adjP[v].push_back(u);

		// Oriented edge (u,v) "leaves" node u
		--size_adjN_u[u];
		adjN[u][size_adjN_u[u]] = indexed_edge(0, edge_sorted_by_vertex_index(u,v));
	}
#if defined DEBUG
	for (node u = 0; u < n; ++u) {
		assert(size_adjN_u[u] == 0);
	}
#endif
}

// When decide_upper_bound is false:
//		returns the number of crossings
// When decide_upper_bound is true:
//		returns m*m + 1 if the number of crossings is greater than the upper_bound
//		returns the number of crossings if the number of crossings is less
//			than the upper_bound
template<bool decide_upper_bound>
inline
uint64_t __compute_C_stack_based(
	const graphs::graph& g, const linear_arrangement& pi,
	std::size_t * const size_adjN_u,
	uint64_t upper_bound = 0
)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	// Adjacency lists, sorted by edge length:
	// - adjP is sorted by increasing edge length
	// - adjN is sorted by decreasing edge length
	std::vector<neighbourhood> adjP(n);
	std::vector<std::vector<indexed_edge>> adjN(n);

	fill_adjP_adjN(g, pi, adjP, adjN, size_adjN_u);

	// relate each edge to an index
	std::map<edge, uint64_t> edge_to_idx;

	uint64_t idx = 0;
	for (position_t pu = 0ULL; pu < n; ++pu) {
		const node u = pi[pu];
		for (auto& v : adjN[u]) {
			v.first = idx;

			edge_to_idx.insert( make_pair(v.second, idx) );
			++idx;
		}
	}

	// stack of the algorithm
	detail::AVL<indexed_edge> S;

	// calculate the number of crossings
	uint64_t C = 0;
	for (position_t pu = 0ULL; pu < n; ++pu) {
		const node u = pi[pu];
		for (node v : adjP[u]) {
			const edge uv = edge_sorted_by_vertex_index(u,v);
			const auto on_top = S.remove(indexed_edge(edge_to_idx[uv], uv));

			C += static_cast<uint64_t>(on_top);
			if constexpr (decide_upper_bound) {
				if (C > upper_bound) { return DECIDED_C_GT; }
			}
		}
		S.join_sorted_all_greater(adjN[u]);
	}

	// none of the conditions above were true, so we must have
	// C <= upper_bound
	return C;
}

// =============================================================================
// CALLS TO ALGORITHM
// =============================================================================

inline
uint64_t __call_C_stack_based(
	const graphs::graph& g,
	const linear_arrangement& pi
)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	if (n < 4) { return 0; }

	// size_adjN_u[u] := size of adjN[u]
	// (adjN declared and defined inside the algorithm)
	data_array<std::size_t> size_adjN_u(n, 0);

	return __compute_C_stack_based<false>(g, pi, size_adjN_u.begin());
}

// ------------------
// single arrangement

inline
uint64_t n_C_stack_based(
	const graphs::graph& g,
	const linear_arrangement& pi
)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif
	return detail::call_with_empty_arrangement(__call_C_stack_based, g, pi);
}

// --------------------
// list of arrangements

inline
std::vector<uint64_t> n_C_stack_based(
	const graphs::graph& g,
	const std::vector<linear_arrangement>& pis
)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(pis.size(), 0);
	if (n < 4) { return cs; }

	// size_adjN_u[u] := size of adjN[u]
	// (adjN declared and defined inside the algorithm)
	data_array<std::size_t> size_adjN_u(n, 0);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		cs[i] = __compute_C_stack_based<false>
				(g, pis[i], size_adjN_u.begin());
	}

	return cs;
}

// -----------------------------------------------------------------------------
// DECISION

inline
uint64_t __call_C_stack_based_lesseq_than(
	const graphs::graph& g,
	const linear_arrangement& pi,
	uint64_t upper_bound
)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	if (n < 4) { return 0; }

	// size_adjN_u[u] := size of adjN[u]
	// (adjN declared and defined inside the algorithm)
	data_array<std::size_t> size_adjN_u(n, 0);

	return __compute_C_stack_based<true>
			(g, pi, size_adjN_u.begin(), upper_bound);
}

// ------------------
// single arrangement

inline
uint64_t is_n_C_stack_based_lesseq_than(
	const graphs::graph& g,
	const linear_arrangement& pi,
	uint64_t upper_bound
)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif
	return detail::call_with_empty_arrangement
			(__call_C_stack_based_lesseq_than, g, pi, upper_bound);
}

// --------------------
// list of arrangements

inline
std::vector<uint64_t> is_n_C_stack_based_lesseq_than(
	const graphs::graph& g,
	const std::vector<linear_arrangement>& pis,
	uint64_t upper_bound
)
noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(pis.size(), 0);
	if (n < 4) { return cs; }

	// size_adjN_u[u] := size of adjN[u]
	// (adjN declared and defined inside the algorithm)
	data_array<std::size_t> size_adjN_u(n, 0);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		cs[i] = __compute_C_stack_based<true>
				(g, pis[i], size_adjN_u.begin(), upper_bound);
	}

	return cs;
}

inline
std::vector<uint64_t> is_n_C_stack_based_lesseq_than(
	const graphs::graph& g,
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
	if (n < 4) { return cs; }

	// size_adjN_u[u] := size of adjN[u]
	// (adjN declared and defined inside the algorithm)
	data_array<std::size_t> size_adjN_u(n, 0);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < pis.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(pis[i].size() == n);
#endif

		// compute C
		cs[i] = __compute_C_stack_based<true>
				(g, pis[i], size_adjN_u.begin(), upper_bounds[i]);
	}

	return cs;
}

} // -- namespace detail
} // -- namespace lal
