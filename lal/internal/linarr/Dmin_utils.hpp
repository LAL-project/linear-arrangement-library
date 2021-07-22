/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
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
#include <lal/graphs/rooted_tree.hpp>
#include <lal/internal/data_array.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/graphs/size_subtrees.hpp>
#include <lal/internal/sorting/counting_sort.hpp>
#include <lal/internal/properties/tree_centroid.hpp>

namespace lal {
namespace internal {

typedef std::pair<lal::node,uint64_t> node_size;
typedef std::pair<lal::edge,uint64_t> edge_size;
typedef std::vector<edge_size>::iterator edge_size_t;

#define to_int64(x) static_cast<int64_t>(x)
#define to_uint64(x) static_cast<uint64_t>(x)

#define PLACE_LEFT_OF 0
#define PLACE_RIGHT_OF 1
#define PLACE_NONE_OF 2

#define RIGHT_SIDE 0
#define LEFT_SIDE 1

// if s = 0 then (s+1)&0x1 = 1
// if s = 1 then (s+1)&0x1 = 0
#define other_side(s) ((s + 1)&0x1)

/* ************************************************************************** */
/* ------------------------- INTERVALS namespace ---------------------------- */

/* The following namespace contains functions for the interval-based algorithms
 * to calculate the planar and projective minimum sum of edge lengths.
 */




namespace intervals {

typedef char place;

/* Make a minimum projective arrangement using the sorted, rooted adjacency list L.
 *
 * M: adjacency matrix of the tree with extra information: for each vertex,
 *		attach an integer that represents the size of the subtree rooted
 *		at that vertex. Each adjacency list is sorted DECREASINGLY by that size.
 * r: the vertex root of the subtree whose interval is to be made
 * r_place: where, respect to its parent, has 'r' been placed in the interval.
 *		LEFT_PLACE, RIGHT_PLACE, ROOT_PLACE
 *		The last value is only valid for the root of the whole tree.
 * ini, fin: left and right limits of the positions of the arrangement in which
 *		the tree has to be arranged. Note that the limits are included [ini,fin].
 * arr: the arrangement of the tree
 *
 * Returns the sum of the length of the outgoing edges from vertex 'r' plus
 * the length of the anchor of the edge from 'r' to its parent. Such length
 * is defined as the number of vertices to the left of 'r' if 'r_place'
 * is RIGHT_PLACE, or as the number of vertices to the right of 'r' if
 * 'r_place' is LEFT_PLACE.
 */
inline uint64_t Dmin_Pr_rooted_adjacency_list(
	const std::vector<std::vector<node_size>>& L, const node r,
	const place r_place,
	position ini, position fin,
	linear_arrangement& arr
)
noexcept
{
#if defined DEBUG
	assert(ini <= fin);
#endif

	// sizes of the subtrees
	const auto& children = L[r];

	// -- place the children --

	// work out the starting side of the first-largest subtree
	uint64_t side = (r_place == PLACE_RIGHT_OF ? RIGHT_SIDE : LEFT_SIDE);

	// size of the intervals from the root to the left end
	uint64_t acc_size_left = 0;
	// size of the intervals from the root to the right end
	uint64_t acc_size_right = 0;

	// number of intervals to the left of the root
	uint64_t n_intervals_left = 0;
	// number of intervals to the right of the root
	uint64_t n_intervals_right = 0;

	// sum of the optimal D for every subtree +
	// the length of the edge from 'r' to its parent (if any)
	uint64_t D = 0;
	// total sum of lengths of edges from 'r' to 'vi' without the anchor
	uint64_t d = 0;

	// while placing the children calculate the
	// length of the edge from 'r' to vertex 'vi'
	for (const auto& [vi, ni] : children) {

		// recursive call: make the interval of 'vi'
		D +=
		Dmin_Pr_rooted_adjacency_list(
			L, vi,
			(side == LEFT_SIDE ? PLACE_LEFT_OF : PLACE_RIGHT_OF),
			(side == LEFT_SIDE ? ini : fin - ni + 1),
			(side == LEFT_SIDE ? ini + ni - 1 : fin),
			arr
		);

		// accumulate size of interval
		d += ni*(side == LEFT_SIDE ? n_intervals_left : n_intervals_right);
		// add length of edge over root 'r'
		d += 1;

		// number of intervals to the left and right of the root
		n_intervals_left += side;
		n_intervals_right += other_side(side);

		// accumulate size of subtree rooted at vi
		acc_size_left += side*ni;
		acc_size_right += other_side(side)*ni;

		// update limits of embedding
		ini += side*ni;
		fin -= other_side(side)*ni;

		// change side
		side = other_side(side);
	}
#if defined DEBUG
	assert(ini == fin);
#endif
	arr[r] = ini;

	// accumulate the length of the edge from 'r' to its parent (if any)
	D +=
	(r_place == PLACE_NONE_OF ? 0 :
	 r_place == PLACE_LEFT_OF ? acc_size_right : acc_size_left);

	return D + d;
}

/* A method that wraps the first call to the recursive method above.
 */
inline uint64_t arrange(
	uint64_t n,
	const std::vector<std::vector<node_size>>& M,
	node r, linear_arrangement& arr
)
noexcept
{
	return Dmin_Pr_rooted_adjacency_list(M, r, PLACE_NONE_OF, 0, n-1, arr);
}

} // -- namespace intervals




/* ************************************************************************** */
/* ---------------------- DISPLACEMENT namespace ---------------------------- */

/* The following namespace contains functions for the interval-based algorithms
 * to calculate the planar and projective minimum sum of edge lengths.
 */




namespace displacement {

/* Procedure 'embed' as defined by Hochberg and Stallmann in \cite Hochberg2003a
 * and with the correction in \cite Alemany2021a.
 *
 * @param L Input sorted rooted adjacency list. Entry L[u] contains the
 * out-neighbours (with respect to the given root 'r') of vertex 'u' sorted
 * decreasingly by size of subtrees.
 *
 * @param v The current branch of the tree to be arranged
 * @param base The displacement for the starting position of the subtree
 * arrangement
 * @param dir Whether or not @e v is to the left or to the right of its parent
 * @param[out] rel_pos the displacement from the root of all nodes of the subtree
 */
inline uint64_t embed_branch(
	const std::vector<std::vector<node_size>>& L,
	node v,
	int64_t base,
	int64_t dir,
	data_array<int64_t>& rel_pos
)
noexcept
{
	const auto& Cv = L[v];
	uint64_t cost_branch = 0;

	uint64_t before = 0;
	uint64_t after = 0;
	uint64_t under_anchor = 0;

	for (size_t i = 1; i < Cv.size(); i += 2) {
		const auto [vi, ni] = Cv[i];
		under_anchor += ni;
	}
	base += dir*(to_int64(under_anchor) + 1);
	cost_branch += under_anchor;

	uint64_t i = Cv.size();
	// iterate from the smallest to the largest subtree
	for (auto it = Cv.rbegin(); it != Cv.rend(); ++it, --i) {
		const auto [vi, ni] = *it;

		// i is even
		if (i%2 == 0) {
			cost_branch += embed_branch(L, vi, base - dir*to_int64(before), -dir, rel_pos);
			cost_branch += before;

			before += ni;
		}
		// i is odd
		else {
			cost_branch += embed_branch(L, vi, base + dir*to_int64(after), dir, rel_pos);

			cost_branch += after;
			after += ni;
		}

		cost_branch += 1;
	}

	rel_pos[v] = base;
	return cost_branch;
}

/* Procedure 'embed' as defined by Hochberg and Stallmann in \cite Hochberg2003a.
 *
 * @param L Input sorted rooted adjacency list. Entry L[u] contains the
 * out-neighbours (with respect to the given root 'r') of vertex 'u' sorted
 * decreasingly by size of subtrees.
 *
 * @param r The vertex root used to construct L.
 *
 * @param[out] arr The optimal arrangement.
 */
inline uint64_t embed(
	const std::vector<std::vector<node_size>>& L,
	const node r,
	linear_arrangement& arr
)
noexcept
{
	const uint64_t n = L.size();
	uint64_t D = 0;

	data_array<int64_t> rel_pos(n, 0);
	uint64_t left_sum = 0;
	uint64_t right_sum = 0;

	uint64_t i = L[r].size();
	for (auto it = L[r].rbegin(); it != L[r].rend(); ++it, --i) {
		const auto [vi, ni] = *it;

		// if is even
		if (i%2 == 0) {
			D += embed_branch(L, vi, to_int64(right_sum), 1, rel_pos);
			D += right_sum;

			right_sum += ni;
		}
		// if i is odd
		else {
			D += embed_branch(L, vi, -to_int64(left_sum), -1, rel_pos);
			D += left_sum;

			left_sum += ni;
		}
		D += 1;
	}

	// the '-1' is used to offset the positions from [1,n] to [0,n-1]
	arr[r] = left_sum + 1 - 1;
	rel_pos[r] = 0;
	for (node v = 0; v < n; ++v) {
		const int64_t pos = to_int64(arr[r]) + rel_pos[v];
#if defined DEBUG
		assert(pos >= 0);
#endif
		arr[v] = to_uint64(pos);
	}

	return D;
}

} // -- namespace displacement




/* ************************************************************************** */
/* --------------------------- ROOTED namespace ----------------------------- */

/* The following namespace contains functions to calculate the sorted, rooted
 * adjacency list of rooted trees.
 */




namespace rooted {

/* Make a sorted, rooted adjacency list sorted according to the sizes of
 * the subtrees of the input rooted tree @e t.
 *
 * @param L The sorted, rooted adjacency list. This is an input/output
 * parameter.
 *
 * @pre Parameter @e L is initialised to have size n, the number of vertices of
 * the tree.
 */
template<typename sort_type = countingsort::decreasing_t>
inline void make_sorted_rooted_adjacency_list(
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
	const size_t k = t.are_size_subtrees_valid() ? 0 : t.get_num_nodes();
	data_array<uint64_t> size_subtrees(k, 0);

	countingsort::memory_counting_sort<edge_size> memcs(n, n);
	auto it = edge_list.begin();

	iterators::E_iterator<graphs::rooted_tree> E_it(t);
	if (t.are_size_subtrees_valid()) {
		// use the sizes that are already calculated
		while (not E_it.end()) {
			const edge e = E_it.get_edge();
			const node v = e.second;
			const uint64_t suv = t.get_num_nodes_subtree(v);
			*it++ = make_pair(e, suv);
			++memcs.count[suv];

			E_it.next();
		}
	}
	else {
		// fill in the size of the subtrees
		internal::get_size_subtrees(t, r, size_subtrees.begin());
		while (not E_it.end()) {
			const edge e = E_it.get_edge();
			const node v = e.second;
			const uint64_t suv = size_subtrees[v];
			*it++ = make_pair(e, suv);
			++memcs.count[suv];

			E_it.next();
		}
	}

	// sort all tuples in L using the size of the subtree
	internal::counting_sort
	<edge_size, edge_size*, sort_type, true>
	(
		edge_list.begin(), edge_list.end(), n,
		[](const edge_size& T) -> size_t { return T.second; },
		memcs
	);
	}

	// M[u] : adjacency list of vertex u sorted decreasingly according
	// to the sizes of the subtrees.
	// This is used to find the optimal projective arrangement of the tree.
	for (const auto& T : edge_list) {
		const auto [u, v] = T.first;
		const uint64_t nv = T.second;
		L[u].push_back(std::make_pair(v,nv));
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

} // -- namespace rooted




/* ************************************************************************** */
/* ----------------------------- FREE namespace ----------------------------- */

/* The following namespace contains functions to calculate the sorted, rooted
 * adjacency list of free trees.
 */




namespace free {

inline void make_adjacency_list_rooted(
	const graphs::free_tree& t, node pu, node u,
	std::vector<std::vector<node_size>>& L
)
noexcept
{
	if (pu == u) {
		for (node v : t.get_neighbours(u)) {
			make_adjacency_list_rooted(t, u, v, L);
		}
		return;
	}

	// find the only instance of 'pu' in the
	// neighbourhood of 'u' and erase it.
	auto& Mu = L[u];

	auto it = Mu.begin();
	bool found = false;
	while (not found and it != Mu.end()) {
		if (it->first == pu) {
			Mu.erase(it);
			found = true;
		}
		else {
			++it;
		}
	}

	for (node v : t.get_neighbours(u)) {
		if (v != pu) {
			make_adjacency_list_rooted(t, u, v, L);
		}
	}
}

/* Make a sorted, rooted adjacency list sorted according to the sizes of
 * the subtrees of the input free tree @e t, using a centroidal vertex as the
 * root.
 *
 * @param L The sorted, rooted adjacency list. This is an input/output
 * parameter.
 *
 * @returns The centroidal vertex used to root the adjacency list.
 *
 * @pre Parameter @e L is initialised to have size n, the number of vertices of
 * the tree.
 */
template<typename sort_type = countingsort::decreasing_t>
inline node make_sorted_rooted_adjacency_list_centroid(
	const graphs::free_tree& t,
	std::vector<std::vector<node_size>>& L
)
noexcept
{
	std::vector<edge_size> sizes_edge;
	// Find a centroidal vertex of the tree.
	// With this method we can retrieve the sorted adjacency matrix;
	// such matrix is used to retrieve the centroid and arrange the tree.
	const node c = internal::retrieve_centroid(t, L, sizes_edge).first;

	// convert M into a rooted (also, directed) adjacency matrix
	make_adjacency_list_rooted(t, c, c, L);

	return c;
}

} // -- namespace free

} // -- namespace internal
} // -- namespace lal
