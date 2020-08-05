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

#include <lal/linarr/Dmin.hpp>

// C++ includes
#include <cassert>
using namespace std;

// lal includes
#include <lal/linarr/D.hpp>
#include <lal/utils/sorting/counting_sort.hpp>
#include <lal/utils/graphs/trees/make_projective_arr.hpp>

namespace lal {
using namespace graphs;
using namespace utils;

namespace linarr {

typedef char place;
#define LEFT_PLACE 0
#define RIGHT_PLACE 1
#define ROOT_PLACE 2

#define right_placed_pos(int_size) (int_size%2 == 1 ? int_size/2 : int_size/2 - 1)
#define left_placed_pos(int_size) (int_size%2 == 1 ? int_size/2 : int_size/2)

#define get_int_size(v) (t.out_degree(v) + 1)

#define get_neighbours(t,u) \
	(t.get_rtree_type() == rtree::rtree_type::arborescence ? \
	t.get_out_neighbours(u) : t.get_in_neighbours(u))

constexpr position pos_in_interval(uint32_t int_size, place P) {
	if (int_size == 1) { return 0; }
	switch (P) {
		case LEFT_PLACE: return left_placed_pos(int_size);
		case RIGHT_PLACE: return right_placed_pos(int_size);
	}
	return int_size/2;
}

constexpr bool start_left_right(uint32_t int_size, place P) {
	switch (P) {
		case LEFT_PLACE: return (int_size%2 == 1 ? false : true);
		case RIGHT_PLACE: return (int_size%2 == 1 ? true : false);
	}
	return true;
}

/*
 * t: the rooted tree.
 * r: the vertex root of the subtree whose interval is to be made
 * place: where, respect to its parent, has 'r' been placed in the interval.
 *		LEFT_PLACE, RIGHT_PLACE, ROOT_PLACE
 *		The last value is only valid for the root of the whole tree.
 * data: the interval of every vertex.
 * 		data[v][p] = u <-> vertex 'u' is at position 'p' of vertex 'v's interval
 *
 * Returns the sum of the length of the edges incident to vertex 'r' plus
 * the length of the anchor of the edge from 'r' to its parent. Such length
 * is defined as the number of vertices to the left of 'r' if 'r_place'
 * is RIGHT_PLACE, or as the number of vertices to the right of 'r' if
 * 'r_place' is LEFT_PLACE.
 */
uint32_t make_interval_of(
	const rooted_tree& t, node r, place r_place,
	vector<vector<node>>& data
)
{
	const uint32_t d_out = t.out_degree(r);
	const uint32_t r_int_size = get_int_size(r);

	vector<node>& interval = data[r];
	interval = vector<node>(r_int_size);

	if (r_int_size == 1) {
		data[r][0] = r;
		return 0;
	}

	if (r_int_size == 2) {
		const node vi = get_neighbours(t,r)[0];
		interval[0] = (r_place == LEFT_PLACE ? vi : r);
		interval[1] = (r_place == LEFT_PLACE ? r : vi);
		const place vi_place = (r_place == LEFT_PLACE ? LEFT_PLACE : RIGHT_PLACE);
		const uint32_t D = make_interval_of(t, vi, vi_place, data);
		return D + 1;

		/*
		uint32_t D;
		if (r_place == LEFT_PLACE) {
			interval[0] = vi;
			interval[1] = r;
			D = make_interval_of(t, vi, LEFT_PLACE, data);
		}
		else {
			interval[0] = r;
			interval[1] = vi;
			D = make_interval_of(t, vi, RIGHT_PLACE, data);
		}
		return D + 1;
		*/
	}

	// -----------------------------
	// get the sizes of the subtrees

	typedef pair<lal::node,uint32_t> nodesize;
	uint32_t max_size = 0;
	vector<nodesize> children(d_out);
	for (size_t i = 0; i < t.out_degree(r); ++i) {
		const node vi = get_neighbours(t,r)[i];
		children[i].first = vi;
		children[i].second = t.n_nodes_subtree(vi);

		/*if (max_size < children[i].second) {
			max_size = children[i].second;
		}*/
		max_size = std::max(max_size, children[i].second);
	}

	// -------------------------
	// sort the children by size

	typedef vector<nodesize>::iterator it;
	auto key = [](const nodesize& v) -> size_t {
		return static_cast<size_t>(v.second);
	};
	utils::counting_sort<it, nodesize>
	(children.begin(), children.end(), max_size + 1, key);

	// ---------------------------
	// first, choose 'r's position

	const size_t root_pos = pos_in_interval(r_int_size, r_place);

	// and place the root
	interval[root_pos] = r;

	// ------------------------
	// then, place the children

	size_t leftpos = root_pos - 1;
	size_t rightpos = root_pos + 1;

	// left == "start placing children to the left of the root or not"
	bool left = start_left_right(r_int_size, r_place);

	// size of the intervals from the root to the left end
	uint32_t acc_size_left = 0;
	// size of the intervals from the root to the right end
	uint32_t acc_size_right = 0;

	// total sum of length of edges + the length of the edge from 'r'
	// to its parent, if any
	uint32_t D = 0;
	// total sum of lengths of edges from 'r' to 'vi'
	uint32_t d = 0;

	// while placing the children calculate the
	// length of the edge from 'r' to vertex 'vi'
	for (const auto& p : children) {
		const node vi = p.first;
		const place vi_place = (left ? LEFT_PLACE : RIGHT_PLACE);

		// recursive call: make the interval of 'vi'
		D += make_interval_of(t, vi, vi_place, data);

		// accumulate size of interval
		d += (left ? acc_size_left : acc_size_right);
		// add length of edge over root 'r'
		d += 1;

		// place vertex vi
		interval[(left ? leftpos : rightpos)] = vi;

		// 1. increase/decrease right/left position
		// 2. accumulate size of subtree rooted at vi
		leftpos -= (left ? 1 : 0);
		acc_size_left += (left ? t.n_nodes_subtree(vi) : 0);

		rightpos += (left ? 0 : 1);
		acc_size_right += (left ? 0 : t.n_nodes_subtree(vi));

		// go to the other side
		left = not left;

		/*
		if (left) {
			d += acc_size_left;

			// make the interval of 'vi'
			D += make_interval_of(t, vi, LEFT_PLACE, data);

			left = false;
			interval[leftpos] = vi;
			--leftpos;
			acc_size_left += t.n_nodes_subtree(vi);
		}
		else {
			d += acc_size_right;

			// make the interval of 'vi'
			D += make_interval_of(t, vi, RIGHT_PLACE, data);

			left = true;
			interval[rightpos] = vi;
			++rightpos;
			acc_size_right += t.n_nodes_subtree(vi);
		}
		d += 1;
		*/
	}

	D +=
	(r_place == ROOT_PLACE ? 0 :
	 r_place == LEFT_PLACE ? acc_size_right : acc_size_left);

	/*
	if (r_place == ROOT_PLACE) {
		D += 0;
	}
	else if (r_place == LEFT_PLACE) {
		D += acc_size_right;
	}
	else if (r_place == RIGHT_PLACE) {
		D += acc_size_left;
	}
	*/
	return D + d;
}

pair<uint32_t, linearrgmnt> compute_Dmin_Projective(const rooted_tree& t) {
	assert(t.is_rooted_tree());

	const uint32_t n = t.n_nodes();
	if (n == 1) {
		return make_pair(0, linearrgmnt(0,0));
	}

	// construct the optimal intervals
	vector<vector<node>> data(t.n_nodes());
	const uint32_t D = make_interval_of(t, t.get_root(), ROOT_PLACE, data);

	// construct the arrangement
	const linearrgmnt arr = put_in_arrangement(t, data);

	return make_pair(D, arr);
}

} // -- namespace linarr
} // -- namespace lal
