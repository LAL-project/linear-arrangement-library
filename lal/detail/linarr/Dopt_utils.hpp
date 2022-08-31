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
 *      Juan Luis Esteban (esteban@cs.upc.edu)
 *          Office 110, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~esteban/
 *          Research Gate: https://www.researchgate.net/profile/Juan_Esteban13
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
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

/**
 * @brief Roots an adjacency list with respect to root @e u.
 *
 * In an entry of @e L corresponding to vertex @e u, @e L[u], removes the vertex
 * in said entry that points to the parent of @e u.
 * @param t Input free tree.
 * @param parent_u Parent node of node @e u.
 * @param u Node being processed.
 * @param[out] L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @pre The initial value of @e parent_u must be @e u itself.
 */
inline
void root_adjacency_list(
	const graphs::free_tree& t, node parent_u, node u,
	std::vector<std::vector<node_size>>& L
)
noexcept
{
	if (parent_u == u) {
		for (node v : t.get_neighbours(u)) {
			root_adjacency_list(t, u, v, L);
		}
		return;
	}

	// find the only instance of 'pu' in the
	// neighbourhood of 'u' and erase it.
	auto& Lu = L[u];

	auto it = Lu.begin();
	bool found = false;
	while (not found and it != Lu.end()) {
		if (it->v == parent_u) {
			Lu.erase(it);
			found = true;
		}
		else {
			++it;
		}
	}

	for (node v : t.get_neighbours(u)) {
		if (v != parent_u) {
			root_adjacency_list(t, u, v, L);
		}
	}
}

/**
 * @brief Make a sorted, rooted adjacency list sorted according to the sizes of
 * the subtrees of the input free tree @e t.
 *
 * This function uses a centroidal vertex as the root.
 * @param t Input free tree.
 * @param L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @returns The centroidal vertex used to root the adjacency list.
 * @pre @e L is initialised to have size n, the number of vertices of
 * the tree.
 */
template <typename sort_type>
node make_sorted_adjacency_list_rooted_centroid(
	const graphs::free_tree& t,
	std::vector<std::vector<node_size>>& L
)
noexcept
{
	detail::data_array<edge_size> sizes_edge;
	// Find a centroidal vertex of the tree.
	// With this method we can retrieve the sorted adjacency matrix;
	// such matrix is used to retrieve the centroid and arrange the tree.
	const node c = detail::retrieve_centroid(t, L, sizes_edge).first;

	// convert M into a rooted (also, directed) adjacency matrix
	root_adjacency_list(t, c, c, L);

	return c;
}

} // -- namespcae Dopt_utils
} // -- namespace detail
} // -- namespace lal
