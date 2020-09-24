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
#include <iostream>
#include <cassert>
using namespace std;

// lal includes
#include <lal/linarr/D.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/graphs/trees/make_projective_arr.hpp>
#include <lal/internal/sorting/counting_sort.hpp>

typedef std::pair<lal::node,uint32_t> node_size;

namespace lal {
using namespace graphs;
using namespace iterators;

namespace internal {

enum class place : uint8_t {
	LEFT_OF, RIGHT_OF, NONE_OF
};

/*
 * M: adjacency matrix of the tree with extra information: for each vertex,
 *		attach an integer that represents the size of the subtree rooted
 *		at that vertex. Each adjacency list is sorted INCREASINGLY by that size.
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
	bool left_side = (r_place == place::RIGHT_OF ? false : true);

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
	for (auto it = children.rbegin(); it != children.rend(); ++it) {
		const node vi = it->first;
		const uint32_t ni = it->second;

		// recursive call: make the interval of 'vi'
		D += Dmin_Pr__optimal_interval_of(
			M, vi,
			(left_side ? place::LEFT_OF : place::RIGHT_OF),
			(left_side ? ini : fin - ni + 1), (left_side ? ini + ni - 1 : fin),
			arr
		);

		// accumulate size of interval
		d += ni*(left_side ? n_intervals_left : n_intervals_right);
		// add length of edge over root 'r'
		d += 1;

		// number of intervals to the left and right of the root
		n_intervals_left += (left_side ? 1 : 0);
		n_intervals_right += (left_side ? 0 : 1);

		// accumulate size of subtree rooted at vi
		acc_size_left += (left_side ? ni : 0);
		acc_size_right += (left_side ? 0 : ni);

		// update limits of embedding
		ini += (left_side ? ni : 0);
		fin -= (left_side ? 0 : ni);

		// change side
		left_side = not left_side;
	}
	arr[r] = ini;

	// accumulate the length of the edge from 'r' to its parent (if any)
	D +=
	(r_place == place::NONE_OF ? 0 :
	 r_place == place::LEFT_OF ? acc_size_right : acc_size_left);

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
		M, r, place::NONE_OF, 0,n-1, arr
	);
}

pair<uint32_t, linear_arrangement> Dmin_Projective(const rooted_tree& t) {
	assert(t.is_rooted_tree());

	const uint32_t n = t.n_nodes();
	if (n == 1) {
		return make_pair(0, linear_arrangement(0,0));
	}

	typedef pair<edge,uint32_t> edge_size;
	typedef vector<edge_size>::iterator edge_size_t;

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
	internal::counting_sort<edge_size_t, edge_size, true>(
		L.begin(), L.end(), n,
		[](const edge_size& T) -> size_t { return std::get<1>(T); }
	);

	// M[u] : adjacency list of vertex u sorted increasingly according
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
		M, t.get_root(), place::NONE_OF, 0,n-1, arr
	);

	return make_pair(D, arr);
}

} // -- namespace internal
} // -- namespace lal
