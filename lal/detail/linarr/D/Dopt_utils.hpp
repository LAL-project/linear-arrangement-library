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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Juan Luis Esteban (esteban@cs.upc.edu)
 *         LOGPROG: Logics and Programming Research Group
 *         Office 110, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://www.cs.upc.edu/~esteban/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/data_array.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/detail/graphs/size_subtrees.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/properties/tree_centroid.hpp>

namespace lal {
namespace detail {

/// Utilities for the various optimal linear arrangement algorithms.
namespace Dopt_utils {

/// Useful typedef to denote relative position.
typedef unsigned char place;
/// Useful typedef to denote relative position.
typedef unsigned char side;

inline constexpr place PLACE_LEFT_OF = 0;
inline constexpr place PLACE_RIGHT_OF = 1;
inline constexpr place PLACE_NONE_OF = 2;

inline constexpr side RIGHT_SIDE = 0;
inline constexpr side LEFT_SIDE  = 1;

// if s = 0 then (s+1)&0x1 = 1
// if s = 1 then (s+1)&0x1 = 0
inline constexpr side other_side(side s) noexcept { return ((s + 1)&0x1); }

/// The tree is left-anchored
inline constexpr char LEFT_ANCHOR = -1;
/// The tree is right-anchored
inline constexpr char RIGHT_ANCHOR = 1;
/// The tree is not anchored
inline constexpr char NO_ANCHOR = 0;
/// The tree is anchored
inline constexpr char ANCHOR = 1;

/* ************************************************************************** */
/* ----------------------- ROOTED ADJACENCY LISTS --------------------------- */

/* Functions to calculate the sorted, rooted
 * adjacency list of rooted and free trees.
 */

/**
 * @brief Make a sorted, rooted adjacency list sorted according to the sizes of
 * the subtrees of the input rooted tree @e t.
 *
 * @param t Input rooted tree.
 * @param[out] L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @pre Parameter @e L is initialised to have size n, the number of vertices of
 * the tree.
 */
template <typename sort_type>
void make_sorted_adjacency_list_rooted(
	const graphs::rooted_tree& t,
	std::vector<std::vector<node_size>>& L
)
noexcept
{
	const uint64_t n = t.get_num_nodes();
	const node r = t.get_root();

	// for every edge (u,v), store the tuple
	//    (n_v, (u,v))
	// at L[u]
	data_array<edge_size> edge_list(n - 1);

	{
	const std::size_t k = t.are_size_subtrees_valid() ? 0 : t.get_num_nodes();
	data_array<uint64_t> size_subtrees(k, 0);

	sorting::countingsort::memory<edge_size> memcs(n, n);
	auto it = edge_list.begin();

	iterators::E_iterator<graphs::rooted_tree> E_it(t);
	if (t.are_size_subtrees_valid()) {
		// use the sizes that are already calculated
		while (not E_it.end()) {
			const edge e = E_it.get_edge();
			const node v = e.second;
			const uint64_t suv = t.get_num_nodes_subtree(v);
			*it++ = {e, suv};
			++memcs.count[suv];

			E_it.next();
		}
	}
	else {
		// fill in the size of the subtrees
		detail::get_size_subtrees(t, r, size_subtrees.begin());
		while (not E_it.end()) {
			const edge e = E_it.get_edge();
			const node v = e.second;
			const uint64_t suv = size_subtrees[v];
			*it++ = {e, suv};
			++memcs.count[suv];

			E_it.next();
		}
	}

	// sort all tuples in L using the size of the subtree
	detail::sorting::counting_sort
		<edge_size, sort_type, true>
		(
			edge_list.begin(), edge_list.end(), n,
			[](const edge_size& T) -> std::size_t { return T.size; },
			memcs
		);
	}

	// M[u] : adjacency list of vertex u sorted decreasingly according
	// to the sizes of the subtrees.
	// This is used to find the optimal projective arrangement of the tree.
	for (const auto& T : edge_list) {
		const auto [u, v] = T.e;
		const uint64_t nv = T.size;
		L[u].push_back({v,nv});
#if defined DEBUG
		assert(t.has_edge(u,v));
#endif
	}

#if defined DEBUG
	for (node u = 0; u < n; ++u) {
		assert(L[u].size() == t.get_out_degree(u));
	}
#endif
}

/* ************************************************************************** */
/* ----------------------- BIPARTITE ARRANGEMENTS --------------------------- */

/**
 * @brief Optimal bipartite arrangement.
 *
 * This function implements the "common" algorithm to construct minimum or maximum
 * bipartite arrangements given in \cite Alemany2023a and \cite Alemany2024b.
 * @tparam make_arrangement Boolean value that indicates whether or not the minimum
 * arrangement should be returned.
 * @tparam sorting_type_t The type of sorting for the vertices. For minimum
 * arrangements, this must be @ref lal::detail::sorting::non_increasing_t. For
 * maximum arrangements, this must be @ref lal::detail::sorting::non_decreasing_t.
 * @tparam graph_t Type of graph. Any subclass of @ref lal::graphs::graph.
 * @param g Input (bipartite) graph.
 * @param c Coloring of the input graph.
 * @returns The cost of a maximal bipartite arrangement and possibly the arrangement
 * that attains it.
 * @pre The input graph @e g is a bipartite graph.
 */
template <
	bool make_arrangement,
	class sorting_type_t,
	class graph_t,
	class bipartite_coloring_t
>
std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t
>
optimal_bipartite_arrangement_AEF(
	const graph_t& g,
	const bipartite_coloring_t& c
)
noexcept
{
	static_assert(
		std::is_same_v<sorting_type_t, sorting::non_increasing_t> ||
		std::is_same_v<sorting_type_t, sorting::non_decreasing_t>
	);
	static_assert(std::is_base_of_v<graphs::graph, graph_t>);

	const auto n = g.get_num_nodes();

	// annoying corner case
	if (n == 1) {
		if constexpr (make_arrangement) {
			return {0, lal::linear_arrangement::identity(n)};
		}
		else {
			return 0;
		}
	}

	data_array<node> vertices_color_1(n - 1);
	std::size_t size_1 = 0;
	data_array<node> vertices_color_2(n - 1);
	std::size_t size_2 = 0;

	{
		const auto first_color = c[0];
		for (lal::node u = 0; u < n; ++u) {
			if (c[u] == first_color) {
				vertices_color_1[size_1++] = u;
			}
			else {
				vertices_color_2[size_2++] = u;
			}
		}

		const auto sort_nodes =
		[&](data_array<node>& nodes, std::size_t s) {
			lal::detail::sorting::counting_sort
				<lal::node, sorting_type_t>
				(
					nodes.begin(), nodes.begin() + s,
					n - 1,
					s,
					[&](const lal::node u) -> std::size_t {
						// in directed graphs, this function returns the sum of the
						// in-degree plus the out-degree of the vertex.
						return g.get_degree(u);
					}
				);
		};
		sort_nodes(vertices_color_1, size_1);
		sort_nodes(vertices_color_2, size_2);
	}

	uint64_t D = 0;
	lal::linear_arrangement arr;

	if constexpr (make_arrangement) {
		arr.resize(n);
	}

	lal::position p = 0;
	for (std::size_t i = size_1 - 1; i > 0; --i) {
		const node u = vertices_color_1[i];
		if constexpr (make_arrangement) {
			arr.assign(u, p);
			++p;
		}
		D += (n - p)*g.get_degree(u);
	}

	{
	const node u = vertices_color_1[0];
	if constexpr (make_arrangement) {
		arr.assign(vertices_color_1[0], p);
		++p;
	}
	D += (n - p)*g.get_degree(u);
	}

	for (std::size_t i = 0; i < size_2; ++i) {
		const node u = vertices_color_2[i];
		if constexpr (make_arrangement) {
			arr.assign(u, p);
			++p;
		}
		D -= (n - p)*g.get_degree(u);
	}

	if constexpr (make_arrangement) {
		return {D, std::move(arr)};
	}
	else {
		return D;
	}
}

} // -- namespcae Dopt_utils
} // -- namespace detail
} // -- namespace lal
