#pragma once

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>
using namespace std;

// lal includes
#include <lal/definitions.hpp>

namespace lal {
namespace internal {

typedef std::pair<lal::edge,uint64_t> edge_size;
typedef std::vector<edge_size>::iterator edge_size_t;
typedef std::pair<lal::node,uint64_t> node_size;
typedef char place;

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
uint64_t Dmin_Pr_rooted_adjacency_list(
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
		Dmin_Pr_rooted_adjacency_list(
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

inline uint64_t Dmin_Projective_rooted_adjacency_list(
	uint64_t n,
	const vector<vector<pair<node,uint64_t>>>& M,
	node r, linear_arrangement& arr
)
noexcept
{
	return Dmin_Pr_rooted_adjacency_list(M, r, PLACE_NONE_OF, 0, n-1, arr);
}

} // -- namespace internal
} // -- namespace lal
