/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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

#include <lal/internal/graphs/trees/Dmin.hpp>

// C++ includes
#include <cassert>
using namespace std;

// lal includes
#include <lal/linarr/D.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/graphs/trees/make_projective_arr.hpp>
#include <lal/internal/sorting/counting_sort.hpp>

typedef std::pair<lal::edge,uint32_t> edge_size;
typedef std::vector<edge_size>::iterator edge_size_t;
typedef std::pair<lal::node,uint32_t> node_size;
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
uint32_t Dmin_Pr__optimal_interval_of(
	const vector<vector<node_size>>& M, const node r,
	const place r_place,
	position ini, position fin,
	linear_arrangement& arr
)
{
	assert(ini <= fin);

	// sizes of the subtrees
	const auto& children = M[r];

	// -- place the children --

	// work out the starting side of the first-largest subtree
	uint32_t side = (r_place == PLACE_RIGHT_OF ? RIGHT_SIDE : LEFT_SIDE);

	// size of the intervals from the root to the left end
	uint32_t acc_size_left = 0;
	// size of the intervals from the root to the right end
	uint32_t acc_size_right = 0;

	// number of intervals to the left of the root
	uint32_t n_intervals_left = 0;
	// number of intervals to the right of the root
	uint32_t n_intervals_right = 0;

	// total sum of length of edges + the length of the edge from 'r'
	// to its parent (if any)
	uint32_t D = 0;
	// total sum of lengths of edges from 'r' to 'vi'
	uint32_t d = 0;

	// while placing the children calculate the
	// length of the edge from 'r' to vertex 'vi'
	for (auto it = children.begin(); it != children.end(); ++it) {
		const node vi = it->first;
		const uint32_t ni = it->second;

		// recursive call: make the interval of 'vi'
		D += Dmin_Pr__optimal_interval_of(
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
	arr[r] = ini;

	// accumulate the length of the edge from 'r' to its parent (if any)
	D +=
	(r_place == PLACE_NONE_OF ? 0 :
	 r_place == PLACE_LEFT_OF ? acc_size_right : acc_size_left);

	return D + d;
}

uint32_t Dmin_Pr__optimal_interval_of(
	const rooted_tree& t,
	const vector<vector<pair<node,uint32_t>>>& M,
	node r, linear_arrangement& arr
)
{
	const uint32_t n = t.n_nodes();
	return Dmin_Pr__optimal_interval_of(
		M, r, PLACE_NONE_OF, 0,n-1, arr
	);
}

pair<uint32_t, linear_arrangement> Dmin_Projective(const rooted_tree& t) {
	assert(t.is_rooted_tree());
	assert(t.size_subtrees_valid());

	const uint32_t n = t.n_nodes();
	if (n == 1) {
		return make_pair(0, linear_arrangement(0,0));
	}

	// for every edge (u,v), store the tuple
	//    (n_v, (u,v))
	// at L[u]
	vector<edge_size> L;
	{
	E_iterator Eit(t);
	while (Eit.has_next()) {
		Eit.next();
		const edge e = Eit.get_edge();
		const node v = e.second;
		L.push_back(make_pair(e,t.n_nodes_subtree(v)));
	}
	}

	// sort all tuples in L using the size of the subtree
	internal::counting_sort<edge_size_t, edge_size, false>(
		L.begin(), L.end(), n,
		[](const edge_size& T) -> size_t { return std::get<1>(T); }
	);

	// M[u] : adjacency list of vertex u sorted decreasingly according
	// to the sizes of the subtrees.
	vector<vector<node_size>> M(n);
	for (const auto& T : L) {
		const node u = std::get<0>(T).first;
		const node v = std::get<0>(T).second;
		const uint32_t nv = std::get<1>(T);
		M[u].push_back(make_pair(v,nv));
		assert(t.has_edge(u,v));
	}

#if defined DEBUG
	for (node u = 0; u < n; ++u) {
		assert(M[u].size() == t.degree(u));
	}
#endif

	// construct the optimal intervals
	linear_arrangement arr(n);
	const uint32_t D = Dmin_Pr__optimal_interval_of(
		M, t.get_root(), PLACE_NONE_OF, 0,n-1, arr
	);

	return make_pair(D, arr);
}

} // -- namespace internal
} // -- namespace lal
