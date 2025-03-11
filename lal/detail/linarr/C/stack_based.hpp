/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
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
#include <map>

// lal includes
#include <lal/graphs/graph.hpp>
#include <lal/detail/arrangement_wrapper.hpp>
#include <lal/detail/avl.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/array.hpp>
#include <lal/detail/macros/basic_convert.hpp>

#define edge_sorted_by_vertex_index(u, v) (u < v ? edge(u, v) : edge(v, u))
#define DECIDED_C_GT (upper_bound + 1)

namespace lal {
namespace detail {
namespace crossings {

/**
 * @brief Namespace for the stack-based algorithm to calculate \f$C\f$.
 *
 * See \cite Alemany2019a.
 */
namespace stack_based {

/// Useful typedef.
typedef std::pair<uint64_t, edge> indexed_edge;

// =============================================================================
// ACTUAL ALGORITHM
// =============================================================================

/**
 * @brief Auxiliary function to sort edges as a function of the arrangement.
 *
 * See \cite Alemany2019a for details on the correctness and behaviour.
 * @tparam graph_t Type of graph.
 * @tparam arrangement_t Type of arrangement.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param adjP @e adjP[v] contains the list of vertices @e u that form edges (u,v)
 * such that arr[u] < arr[v] sorted nondecreasingly by edge length.
 * @param adjN @e adjN[v] contains the list of vertices @e u that form edges (u,v)
 * such that arr[v] < arr[u] sorted nonincreasingly by edge length.
 * @param size_adjN_u Auxiliary memory array of size @e n.
 */
template <graphs::Graph graph_t, Arrangement arrangement_t>
void fill_adjP_adjN(
	const graph_t& g,
	const arrangement_t& arr,
	std::vector<neighbourhood>& adjP,
	std::vector<std::vector<indexed_edge>>& adjN,
	uint64_t * const size_adjN_u
) noexcept
{
	const uint64_t n = g.get_num_nodes();

	// Retrieve all edges of the graph to sort
	std::vector<edge> edges = g.get_edges();

	// sort edges of the graph by non-decreasing edge length
	// l(e_1) <= l(e_2) <= ... <= l(e_m)
	sorting::counting_sort<edge, sorting::sort_type::non_decreasing>(
		edges.begin(),
		edges.end(),
		n - 1, // length of the longest edge
		edges.size(),
		[&](const edge& e) -> std::size_t
		{
			const node uu = e.first;
			const node vv = e.second;
			const auto [u, v] =
				(arr[node_t{uu}] < arr[node_t{vv}] ? edge(uu, vv) : edge(vv, uu)
				);

			++size_adjN_u[u];
			return abs_diff(arr[node_t{u}], arr[node_t{v}]);
		}
	);

	// initialize adjN
	for (node u = 0; u < n; ++u) {
		// divide by two because the 'key' function in the call to
		// the sorting function is called twice for every edge
#if defined DEBUG
		assert((size_adjN_u[u] % 2) == 0);
		// the assertion
		//     assert(size_adjN_u[u] != 0);
		// is wrong.
#endif

		size_adjN_u[u] /= 2;
		adjN[u].resize(size_adjN_u[u]);
	}

	// fill adjP and adjN at the same time
	for (const auto& [uu, vv] : edges) {
		// arr[u] < arr[v]
		const auto [u, v] =
			(arr[node_t{uu}] < arr[node_t{vv}] ? edge(uu, vv) : edge(vv, uu));

		// oriented edge (u,v) "enters" node v
		adjP[v].push_back(u);

		// Oriented edge (u,v) "leaves" node u
		--size_adjN_u[u];
		adjN[u][size_adjN_u[u]] =
			indexed_edge(0, edge_sorted_by_vertex_index(u, v));
	}

#if defined DEBUG
	for (node u = 0; u < n; ++u) {
		assert(size_adjN_u[u] == 0);
	}
#endif
}

/**
 * @brief Stack based computation of \f$C\f$ for undirected graphs.
 *
 * When template parameter @e decide_upper_bound is false, the function returns
 * the number of crossings.
 * @tparam decide_upper_bound Boolean value to choose the nature of the return type.
 * @tparam graph_t Type of graph.
 * @tparam arrangement_t Type of arrangement.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param size_adjN_u See \cite Alemany2019a for details.
 * @param upper_bound Upper bound on the number of crossings.
 * @returns When @e decide_upper_bound is true, the return value is:
 * - one unit larger than the upper bound passed as parameter if \f$C>\f$ upper bound.
 * - \f$C\f$ if the number of crossings is less or equal than the upper bound.
 */
template <
	bool decide_upper_bound,
	graphs::Graph graph_t,
	Arrangement arrangement_t>
[[nodiscard]] uint64_t compute_C_stack_based(
	const graph_t& g,
	const arrangement_t& arr,
	uint64_t * const size_adjN_u,
	uint64_t upper_bound
) noexcept
{
	const uint64_t n = g.get_num_nodes();

	// Adjacency lists, sorted by edge length:
	// - adjP is sorted by increasing edge length
	std::vector<neighbourhood> adjP(n);
	// - adjN is sorted by decreasing edge length
	std::vector<std::vector<indexed_edge>> adjN(n);

	fill_adjP_adjN(g, arr, adjP, adjN, size_adjN_u);

	// relate each edge to an index
	std::map<edge, uint64_t> edge_to_idx;

	uint64_t idx = 0;
	for (position_t pu = 0ull; pu < n; ++pu) {
		const node u = arr[pu];
		for (auto& v : adjN[u]) {
			v.first = idx;

			edge_to_idx.insert(make_pair(v.second, idx));
			++idx;
		}
	}

	// stack of the algorithm
	AVL<indexed_edge> S;

	// calculate the number of crossings
	uint64_t C = 0;
	for (position_t pu = 0ull; pu < n; ++pu) {
		const node u = arr[pu];
		for (node v : adjP[u]) {
			const edge uv = edge_sorted_by_vertex_index(u, v);

			// the elements inserted into the tree are unique by construction,
			// so we can remove elements without using their counter
			// (remove<false>) and use the number of larger nodes
			// (on_top.num_nodes_larger) to calculate the number of crossings.
			const auto on_top =
				S.remove<false>(indexed_edge(edge_to_idx[uv], uv));
			C += on_top.num_nodes_larger;

			if constexpr (decide_upper_bound) {
				if (C > upper_bound) {
					return DECIDED_C_GT;
				}
			}
		}
		S.join_sorted_all_greater(std::move(adjN[u]));
	}

	// none of the conditions above were true, so we must have
	// C <= upper_bound
	return C;
}

} // namespace stack_based

// =============================================================================
// CALLS TO ALGORITHM
// =============================================================================

// ------------------
// single arrangement

/**
 * @brief Stack based computation of \f$C\f$.
 * @tparam graph_t Type of input graph.
 * @tparam arrangement_t Type of input arrangement.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @returns \f$C_{\pi}(G)\f$ on the input arrangement.
 */
template <graphs::Graph graph_t, Arrangement arrangement_t>
[[nodiscard]] uint64_t
n_C_stack_based(const graph_t& g, const arrangement_t& arr) noexcept
{
	const uint64_t n = g.get_num_nodes();

#if defined DEBUG
	assert(arr.size() == 0 or arr.size() == n);
#endif

	if (n < 4) {
		return 0;
	}

	// size_adjN_u[u] := size of adjN[u]
	// (adjN declared and defined inside the algorithm)
	array<uint64_t> size_adjN_u(n, 0);

	return stack_based::compute_C_stack_based<false>(
		g, arr, size_adjN_u.begin(), 0
	);
}

// --------------------
// list of arrangements

/**
 * @brief Stack based computation of \f$C\f$.
 * @tparam graph_t Type of input graph.
 * @param g Input graph.
 * @param arrs List of input arrangement.
 * @returns \f$C_{\pi}(G)\f$ on every input arrangement.
 */
template <graphs::Graph graph_t>
[[nodiscard]] std::vector<uint64_t> n_C_stack_based(
	const graph_t& g, const std::vector<linear_arrangement>& arrs
) noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(arrs.size(), 0);
	if (n < 4) {
		return cs;
	}

	// size_adjN_u[u] := size of adjN[u]
	// (adjN declared and defined inside the algorithm)
	array<uint64_t> size_adjN_u(n, 0);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < arrs.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(arrs[i].size() == n);
#endif

		// compute C
		cs[i] = stack_based::compute_C_stack_based<false>(
			g, nonidentity_arr(arrs[i]), size_adjN_u.begin(), 0
		);
	}

	return cs;
}

// -----------------------------------------------------------------------------
// DECISION

// ------------------
// single arrangement

/**
 * @brief Stack based computation of \f$C\f$ with early termination.
 * @tparam graph_t Type of input graph.
 * @tparam arrangement_t Type of input arrangement.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @param upper_bound Bound used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on the input arrangement if it is less than the
 * upper bound. It returns a value one unit larger than the upper bound otherwise.
 */
template <graphs::Graph graph_t, Arrangement arrangement_t>
[[nodiscard]] uint64_t is_n_C_stack_based_lesseq_than(
	const graph_t& g, const arrangement_t& arr, const uint64_t upper_bound
) noexcept
{
	const uint64_t n = g.get_num_nodes();

#if defined DEBUG
	assert(arr.size() == 0 or arr.size() == n);
#endif

	if (n < 4) {
		return 0;
	}

	// size_adjN_u[u] := size of adjN[u]
	// (adjN declared and defined inside the algorithm)
	array<uint64_t> size_adjN_u(n, 0);

	return stack_based::compute_C_stack_based<true>(
		g, arr, size_adjN_u.begin(), upper_bound
	);
}

// --------------------
// list of arrangements

/**
 * @brief Stack based computation of \f$C\f$ with early termination.
 * @tparam graph_t Type of input graph
 * @param g Input graph.
 * @param arrs List of input arrangement.
 * @param upper_bound Bound used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on every input arrangement if it is less than the
 * upper bound. It returns a value one unit larger than the upper bound otherwise.
 */
template <graphs::Graph graph_t>
[[nodiscard]] std::vector<uint64_t> is_n_C_stack_based_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	const uint64_t upper_bound
) noexcept
{
	const uint64_t n = g.get_num_nodes();

	std::vector<uint64_t> cs(arrs.size(), 0);
	if (n < 4) {
		return cs;
	}

	// size_adjN_u[u] := size of adjN[u]
	// (adjN declared and defined inside the algorithm)
	array<uint64_t> size_adjN_u(n, 0);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < arrs.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(arrs[i].size() == n);
#endif

		// compute C
		cs[i] = stack_based::compute_C_stack_based<true>(
			g, nonidentity_arr(arrs[i]), size_adjN_u.begin(), upper_bound
		);
	}

	return cs;
}

/**
 * @brief Stack based computation of \f$C\f$ with early termination.
 * @tparam graph_t Type of input graph
 * @param g Input graph.
 * @param arrs List of input arrangement.
 * @param upper_bounds List of bounds used for early termination.
 * @returns \f$C_{\pi}(G)\f$ on every input arrangement if it is less than the
 * corresponding upper bound. It returns a value one unit larger than the upper
 * bound otherwise.
 */
template <graphs::Graph graph_t>
[[nodiscard]] std::vector<uint64_t> is_n_C_stack_based_lesseq_than(
	const graph_t& g,
	const std::vector<linear_arrangement>& arrs,
	const std::vector<uint64_t>& upper_bounds
) noexcept
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

	// size_adjN_u[u] := size of adjN[u]
	// (adjN declared and defined inside the algorithm)
	array<uint64_t> size_adjN_u(n, 0);

	/* compute C for every linear arrangement */
	for (std::size_t i = 0; i < arrs.size(); ++i) {
#if defined DEBUG
		// ensure that no linear arrangement is empty
		assert(arrs[i].size() == n);
#endif

		// compute C
		cs[i] = stack_based::compute_C_stack_based<true>(
			g, nonidentity_arr(arrs[i]), size_adjN_u.begin(), upper_bounds[i]
		);
	}

	return cs;
}

} // namespace crossings
} // namespace detail
} // namespace lal
