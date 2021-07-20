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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>
using namespace std;

// lal includes
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/graphs/size_subtrees.hpp>
#include <lal/internal/graphs/make_arrangement.hpp>
#include <lal/internal/sorting/counting_sort.hpp>

typedef std::pair<lal::edge,uint64_t> edge_size;
typedef std::vector<edge_size>::iterator edge_size_t;
typedef std::pair<lal::node,uint64_t> node_size;
typedef char place;

namespace lal {
using namespace graphs;
using namespace iterators;

namespace internal {

#define PLACE_LEFT_OF 0
#define PLACE_RIGHT_OF 1
#define PLACE_NONE_OF 2

#define RIGHT_SIDE 0
#define LEFT_SIDE 1

// if s = 0 then (s+1)&0x1 = 1
// if s = 1 then (s+1)&0x1 = 0
#define other_side(s) ((s + 1)&0x1)
//#define other_side(s) (s == RIGHT_SIDE ? LEFT_SIDE : RIGHT_SIDE)

/*
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
uint64_t __Dmin_Pr__optimal_interval_of(
	const vector<vector<node_size>>& M, const node r,
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
	const auto& children = M[r];

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
		__Dmin_Pr__optimal_interval_of(
			M, vi,
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

uint64_t Dmin_Pr__optimal_interval_of(
	uint64_t n,
	const vector<vector<pair<node,uint64_t>>>& M,
	node r, linear_arrangement& arr
)
noexcept
{
	return
	__Dmin_Pr__optimal_interval_of(M, r, PLACE_NONE_OF, 0, n - 1, arr);
}

pair<uint64_t, linear_arrangement> Dmin_Projective(const rooted_tree& t) noexcept
{
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif

	const uint64_t n = t.get_num_nodes();
	if (n == 1) {
		return make_pair(0, linear_arrangement(0,0));
	}

	// for every edge (u,v), store the tuple
	//    (n_v, (u,v))
	// at L[u]
	data_array<edge_size> L(n - 1);

	{
	const size_t k = t.are_size_subtrees_valid() ? 0 : t.get_num_nodes();
	data_array<uint64_t> size_subtrees(k, 0);

	countingsort::memory_counting_sort<edge_size> memcs(n, n);
	auto it = L.begin();
	E_iterator<rooted_tree> e_it(t);

	if (t.are_size_subtrees_valid()) {
		while (not e_it.end()) {
			const edge e = e_it.get_edge();
			const node v = e.second;
			const uint64_t suv = t.get_num_nodes_subtree(v);
			*it++ = make_pair(e, suv);
			++memcs.count[suv];

			e_it.next();
		}
	}
	else {
		// fill in the size of the subtrees
		internal::get_size_subtrees(t, t.get_root(), size_subtrees.begin());
		while (not e_it.end()) {
			const edge e = e_it.get_edge();
			const node v = e.second;
			const uint64_t suv = size_subtrees[v];
			*it++ = make_pair(e, suv);
			++memcs.count[suv];

			e_it.next();
		}
	}

	// sort all tuples in L using the size of the subtree
	internal::counting_sort
	<edge_size, edge_size*, countingsort::decreasing_t, true>
	(
		L.begin(), L.end(), n,
		[](const edge_size& T) -> size_t { return T.second; },
		memcs
	);
	}

	// M[u] : adjacency list of vertex u sorted decreasingly according
	// to the sizes of the subtrees.
	vector<vector<node_size>> M(n);
	for (const auto& T : L) {
		const auto [u, v] = T.first;
		const uint64_t nv = T.second;
		M[u].push_back(make_pair(v,nv));
#if defined DEBUG
		assert(t.has_edge(u,v));
#endif
	}

#if defined DEBUG
	for (node u = 0; u < n; ++u) {
		assert(M[u].size() == t.get_out_degree(u));
	}
#endif

	// construct the optimal intervals
	linear_arrangement arr(n);
	const uint64_t D =
	__Dmin_Pr__optimal_interval_of(M, t.get_root(), PLACE_NONE_OF, 0,n-1, arr);
	return make_pair(D, arr);
}

} // -- namespace internal
} // -- namespace lal