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

#pragma once

#if !defined DEBUG
#if defined LAL_DEBUG_DMax_1_thistle
#error "LAL_DEBUG_DMax_1_thistle can only be defined when DEBUG is defined"
#endif
#endif

// C++ includes
#include <type_traits>
#include <algorithm>
#include <cstdint>
#include <vector>

#if defined DEBUG
#if defined LAL_DEBUG_DMax_1_thistle
#include <iostream>
#endif
#include <cassert>
#endif

// lal includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/formal_constraints.hpp>
#include <lal/linarr/D/D.hpp>
#if defined DEBUG
#include <lal/linarr/formal_constraints.hpp>
#if defined LAL_DEBUG_DMax_1_thistle
#include <lal/graphs/output.hpp>
#endif
#endif

#include <lal/detail/graphs/traversal.hpp>
#include <lal/detail/properties/bipartite_graph_colorability.hpp>
#include <lal/detail/properties/branchless_paths_compute.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/linarr/level_signature.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace thistle_1 {

/// Details of algorithm for 1-Thistle MaxLA
namespace bits {

/**
 * @brief Next binary combination of 0's and 1's.
 * @tparam iterator_t Type of pointer to a sequence.
 * @param begin Start of the sequence.
 * @param end End of the sequence.
 * @returns Whether or not there are more configurations
 */
template <typename iterator_t>
[[nodiscard]] inline bool next_binary(iterator_t begin, iterator_t end) noexcept
{
	while (begin != end and *begin == 1) {
		*begin = 0;
		++begin;
	}
	// reached the last configuration   "1......1"
	// which now has been turned into a "0......0"
	if (begin == end) {
		return false;
	}

	// the configuration is "0....01x...x"
	*begin = 1;
	// there are still more binary configurations
	return true;
}

/// Typedef for @ref properties::bipartite_graph_coloring::blue.
static constexpr auto blue = properties::bipartite_graph_coloring::blue;
/// Typedef for @ref properties::bipartite_graph_coloring::red.
static constexpr auto red = properties::bipartite_graph_coloring::red;

/// Left side of the thistle vertex.
static constexpr char LEFT_SIDE = 0;
/// Right side of the thistle vertex.
static constexpr char RIGHT_SIDE = 1;
/// The other side. "Right" if 'side' is "left"; "left" if 'side' is "right".
static constexpr char other_side(char side) noexcept
{
	return (side == LEFT_SIDE ? RIGHT_SIDE : LEFT_SIDE);
}
// sanity check
static_assert(other_side(RIGHT_SIDE) == LEFT_SIDE);
// sanity check
static_assert(other_side(LEFT_SIDE) == RIGHT_SIDE);
// sanity check
static_assert(other_side(RIGHT_SIDE) != RIGHT_SIDE);
// sanity check
static_assert(other_side(LEFT_SIDE) != LEFT_SIDE);

/// Result type of the function
template <bool make_arrangement>
using result_t = std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t>;

#define lal_level_position_(p) levels_per_vertex[node_t{inv_arr[p]}]
#define lal_level_vertex_(u) levels_per_vertex[node_t{u}]

#if defined LAL_DEBUG_DMax_1_thistle
inline void print_arrangement(
	const std::string& msg, const linear_arrangement& arr
) noexcept
{
	const auto dir = arr.direct_as_vector();
	const auto inv = arr.inverse_as_vector();
	std::cout << "        " << msg << ":\n";
	std::cout << "        Direct: ";
	for (std::size_t i = 0; i < dir.size(); ++i) {
		std::cout << ' ' << dir[i];
	}
	std::cout << '\n';
	std::cout << "        Inverse:";
	for (std::size_t i = 0; i < inv.size(); ++i) {
		std::cout << ' ' << inv[i];
	}
	std::cout << '\n';
}
#endif

/**
 * @brief Construct the initial arrangement that will later be improved.
 * @param t Input free tree.
 * @param thistle Thistle vertex.
 * @param thistle_level Level value for @e thistle.
 * @param thistle_side_per_vertex Side of @e thistle in which each vertex
 * have to be placed at in the first arrangement.
 * @param[out] levels_per_vertex Array containing the level value of each vertex of
 * the tree.
 * @param[out] inv_arr Inverse linear arrangement (used to evaluate the cost of the
 * distribution).
 */
inline void construct_initial_arrangement(
	const graphs::free_tree& t,
	const node thistle,
	const int64_t thistle_level,
	const array<char>& thistle_side_per_vertex,
	level_signature_per_vertex& levels_per_vertex,
	array<node>& inv_arr
) noexcept
{
	const uint64_t n = t.get_num_nodes();

	// The minimum level value in the configuration. There is always
	// a negative (<0) level value, so the minimum can be initialized at 0.
	int64_t min_level_value = 0;

	position left = 0;
	position right = n - 1ull;

	// Calculate the level for each vertex and place them in the inverse
	// arrangement. The arrangement built at this step is only preliminary.
	for (node u = 0; u < n; ++u) {
		if (u == thistle) {
			continue;
		}
		const int64_t d = to_int64(t.get_degree(u));
		if (thistle_side_per_vertex[u] == LEFT_SIDE) {
			lal_level_vertex_(u) = d;
			inv_arr[left++] = u;
		}
		else if (thistle_side_per_vertex[u] == RIGHT_SIDE) {
			lal_level_vertex_(u) = -d;
			inv_arr[right--] = u;
			min_level_value = std::min(min_level_value, -d);
		}
#if defined DEBUG
		else {
#if defined LAL_DEBUG_DMax_1_thistle
			std::cout << "Vertex " << u << " was not assigned a side\n";
#endif
			assert(false);
		}
#endif
	}

	// This function assumes that the thistle will never have negative (<0)
	// level value.
	lal_level_vertex_(thistle) = thistle_level;
	// The position to place the thistle is either 'left' or 'right' since,
	// at this point, their values are equal (see next assertion).
	inv_arr[left] = thistle;

#if defined DEBUG
	assert(left == right);
#endif

#if defined LAL_DEBUG_DMax_1_thistle
	std::cout << "        Level per vertex:\n";
	for (node u = 0; u < n; ++u) {
		std::cout << "            level[" << u
				  << "]= " << levels_per_vertex[node_t{u}] << '\n';
	}
#endif

	// Sort the vertices by level (first, those with positive level. Then, those
	// with negative level). Independent tasks.
	sorting::counting_sort<node, sorting::sort_type::non_increasing>(
		inv_arr.begin(),
		inv_arr.begin() + left,
		2 * n,
		n,
		[&](const node u)
		{
			return to_uint64(lal_level_vertex_(u) - min_level_value);
		}
	);
	sorting::counting_sort<node, sorting::sort_type::non_increasing>(
		inv_arr.begin() + right + 1,
		inv_arr.end(),
		2 * n,
		n,
		[&](const node u)
		{
			return to_uint64(lal_level_vertex_(u) - min_level_value);
		}
	);
}

/**
 * @brief Sorts the intervals of vertices of equal level value
 *
 * In such a way that, for positive (>= 0) level values
 * - the neighbors of the thistle are placed to the leftmost positions,
 * - then comes the thistle,
 * - and then the remaining vertices are placed in the rightmost positions.
 * for negative (< 0) level values
 * - then the remaining vertices are placed in the leftmost positions,
 * - then comes the thistle,
 * - and the neighbors of the thistle are placed to the rightmost positions.
 *
 * This is Ok thanks to Nurse and De Vos \cite Nurse2018a \cite Nurse2019a.
 * @param n Number of vertices
 * @param thistle Thistle vertex.
 * @param is_thistle_neighbor Array to query if a vertex is a neighbor of the thisle.
 * @param levels_per_vertex Array containing the level value of each vertex of
 * the tree.
 * @param[out] inv_arr Inverse linear arrangement.
 */
inline void sort_level_sequences(
	const uint64_t n,
	const node thistle,
	const array<char>& is_thistle_neighbor,
	const level_signature_per_vertex& levels_per_vertex,
	array<node>& inv_arr
) noexcept
{
	position p = 0;
	while (p < n) {

		position q = p;
		const int64_t current_level = lal_level_position_(p);
		while (q < n and lal_level_position_(q) == current_level) {
			++q;
		}

#if defined LAL_DEBUG_DMax_1_thistle
		std::cout << "        Sort the interval [" << p << ", " << q << ").\n";
#endif

		// sort interval [p,q)
		sorting::counting_sort<node, sorting::sort_type::non_decreasing>(
			inv_arr.begin() + p,
			inv_arr.begin() + q,
			2,
			q - p,
			[&](const node u)
			{
				// 0, 1, 2
				if (is_thistle_neighbor[u] == 1) {
					return 0ull;
				}
				if (u == thistle) {
					return 1ull;
				}
				return 2ull;
			}
		);

		p = q;
	}
}

/**
 * @brief Moves the vertex at position @e p to the right of the thistle vertex.
 *
 * This is implemented as a series of consecutive swaps starting at @p towards
 * the thistle between consecutive vertices in the arrangement.
 * @param t Input tree.
 * @param thistle Thistle vertex.
 * @param p Position of the vertex to move.
 * @param[out] arr Actual linear arrangement.
 * @pre The thistle vertex is at a position @e q such that \f$p < q\f$.
 * @post The arrangement is updated.
 */
inline void shift_vertex_to_right(
	const graphs::free_tree& t,
	const node thistle,
	position_t p,
	linear_arrangement& arr
) noexcept
{
	const auto n = t.get_num_nodes();
	while (p < n - 1 and arr[p + 1ull] != thistle) {
		arr.swap(p, p + 1ull);
		++p;
	}
	arr.swap(p, p + 1ull);
}

/**
 * @brief Adjust misplaced nonneighbors of the thistle vertex in a smart way.
 *
 * This function stops moving vertices as soon as one moving operation decreases
 * the cost of the arrangement (without actually applying it).
 * @param t Input tree
 * @param thistle_level Level of the thistle
 * @param thistle Thistle vertex
 * @param is_thistle_neighbor Array to query if a vertex is a neighbor of the thisle.
 * @param levels_per_vertex Array containing the level value of each vertex of
 * the tree.
 * @param[out] arr Arrangement to be manipulated.
 */
inline void adjust_nonneighbors_of_thistle_smart(
	const graphs::free_tree& t,
	const int64_t thistle_level,
	const node thistle,
	const array<char>& is_thistle_neighbor,
	const level_signature_per_vertex& levels_per_vertex,
	linear_arrangement& arr
) noexcept
{
#if defined DEBUG
	assert(arr[node_t{thistle}] > 0);
#endif

	// position of thistle can never be '0'
	position p = arr[node_t{thistle}] - 1;

#if defined LAL_DEBUG_DMax_1_thistle
	std::cout << "        Thistle position p= " << p + 1 << '\n';
#endif

	// number of vertices between the thistle and its first non-neighbor
	int64_t num_neighbors = 0;
	// sum of level values between the thistle and its first non-neighbor
	int64_t total_level_value = 0;

	bool stop = false;
	while (not stop) {
#if defined DEBUG
		// ensure 'p' does not contain an infinite
		// value caused by possible underflows
		assert(p <= t.get_num_nodes());
#endif

#if defined LAL_DEBUG_DMax_1_thistle
		std::cout << "        __________________________________________\n";
		std::cout << "        Find next non-neighbor starting at p= " << p
				  << '\n';
#endif

		// find the next non-neighbor of 'thistle'
		while (p > 0 and is_thistle_neighbor[arr[position_t{p}]] == 1) {
			total_level_value += lal_level_vertex_(arr[position_t{p}]);
			++num_neighbors;

#if defined LAL_DEBUG_DMax_1_thistle
			std::cout << "            Vertex " << arr[position_t{p}]
					  << " is a neighbor\n";
			std::cout << "                At position:    " << p << '\n';
			std::cout << "                Of level value: "
					  << lal_level_vertex_(arr[position_t{p}]) << '\n';
#endif

			--p;
		}

#if defined LAL_DEBUG_DMax_1_thistle
		std::cout << "        Sum of level values: " << total_level_value
				  << '\n';
		std::cout << "        Number of neighbors: " << num_neighbors << '\n';
#endif

		const node to_move = arr[position_t{p}];

		if (is_thistle_neighbor[to_move] == 1) {
#if defined DEBUG
			assert(p == 0);
#endif
			stop = true;
		}
		else {
#if defined DEBUG
			assert(is_thistle_neighbor[to_move] == 0);
#endif
			const int64_t level_nonneigh = lal_level_vertex_(to_move);

#if defined LAL_DEBUG_DMax_1_thistle
			std::cout << "        First non-neighbor:  " << to_move << '\n';
			std::cout << "               at position:  " << p << '\n';
			std::cout << "                  of level:  " << level_nonneigh
					  << '\n';
			std::cout << "        Total level values:  " << total_level_value
					  << '\n';
			std::cout << "        Number of neighbors: " << num_neighbors
					  << '\n';
			std::cout << "        -(j + 1)*level_nonneigh= "
					  << -(num_neighbors + 1) * level_nonneigh << '\n';
#endif

			const int64_t gain = -(num_neighbors + 1) * level_nonneigh +
								 total_level_value + thistle_level;

#if defined LAL_DEBUG_DMax_1_thistle
			std::cout << "        gain= " << gain << '\n';
#endif

			if (gain > 0) {
				// move the vertex at position 'q' to the right of the thistle
				shift_vertex_to_right(t, thistle, p, arr);

				// vertex 'thistle' has been moved one position to the left
			}

			stop = gain <= 0 or p == 0;
			// the case "p == 0" triggers "stop = true"
			p = (p > 0 ? p - 1 : p);
		}

#if defined LAL_DEBUG_DMax_1_thistle
		print_arrangement(
			"After moving vertex '" + std::to_string(to_move) + "'", arr
		);
#endif
	}
}

/*
inline void adjust_nonneighbors_of_thistle_exhaustive
(
	const graphs::free_tree& t,
	const node thistle,
	const array<char>& is_thistle_neighbor,
	linear_arrangement& arr
)
noexcept
{
#if defined DEBUG
	assert(arr[node_t{thistle}] > 0);
#endif

	linear_arrangement copy = arr;
	uint64_t D = linarr::sum_edge_lengths(t, copy);

	position_t p = arr[node_t{thistle}];

	bool stop = false;
	while (not stop) {
#if defined DEBUG
		// ensure 'p' does not contain an infinite
		// value caused by possible underflows
		assert(p <= t.get_num_nodes());
#endif

#if defined LAL_DEBUG_DMax_1_thistle
		std::cout << "        __________________________________________\n";
		std::cout << "        Find next non-neighbor starting at p= " << p << '\n';
#endif

		// find the next non-neighbor of 'thistle'
		while (p > 0ull and is_thistle_neighbor[arr[p]] == 1) {
			--p;
		}

		const node to_move = arr[p];

		if (is_thistle_neighbor[to_move] == 1) {
#if defined DEBUG
			assert(p == 0ull);
#endif
			stop = true;
		}
		else {

			shift_vertex_to_right(t, thistle, p, copy);
			uint64_t D_new = linarr::sum_edge_lengths(t, copy);

			if (D_new > D) {
				arr = copy;
				D = D_new;
			}

			stop = p == 0ull;
		}
	}
}
*/

/**
 * @brief Tries to make a maximal arrangement with a given thistle vertex of a
 * given level value
 *
 * Parameters @e arr, @e inv_arr, @e levels_per_vertex are temporary memory
 * passed as parameters to avoid constant allocations and deallocations.
 * @tparam make_arrangement Should the arrangement be returned?
 * @param t Input free tree.
 * @param thistle Thistle vertex.
 * @param thistle_level Level value for @e thistle.
 * @param is_thistle_neighbor Array to query if a vertex is a neighbor of the thisle.
 * @param thistle_side_per_vertex Side of @e thistle in which each vertex
 * have to be placed at in the first arrangement.
 * @param[out] arr Linear arrangement (used to evaluate the cost of the distribution).
 * @param[out] inv_arr Inverse linear arrangement (used to evaluate the cost of the
 * distribution).
 * @param[out] levels_per_vertex Array containing the level value of each vertex of
 * the tree.
 * @param[inout] res Contains the best arrangement produced here if it is better
 * than the one it already contains.
 */
template <bool make_arrangement>
inline void merge_arrangements(
	const graphs::free_tree& t,
	const node thistle,
	const int64_t thistle_level,
	const array<char>& is_thistle_neighbor,
	const array<char>& thistle_side_per_vertex,
	linear_arrangement& arr,
	array<node>& inv_arr,
	level_signature_per_vertex& levels_per_vertex,
	result_t<make_arrangement>& res
) noexcept
{
	const auto n = t.get_num_nodes();

#if defined DEBUG
#if defined LAL_DEBUG_DMax_1_thistle
	std::cout << "        Chosen level value: " << thistle_level << '\n';
#endif
#endif

	construct_initial_arrangement(
		t,
		thistle,
		thistle_level,
		thistle_side_per_vertex,
		levels_per_vertex,
		inv_arr
	);

#if defined DEBUG
	arr = linear_arrangement::from_inverse(inv_arr.begin(), inv_arr.end());
#if defined LAL_DEBUG_DMax_1_thistle
	print_arrangement("Initial arrangement", arr);
#endif
	assert(linarr::is_arrangement(t, arr));
	// sum of edge lengths prior to adjustments
	const uint64_t _sum_lengths_1 = linarr::sum_edge_lengths(t, arr);
#if defined LAL_DEBUG_DMax_1_thistle
	std::cout << "        _sum_lengths_1= " << _sum_lengths_1 << '\n';
#endif
#endif

	// sort the vertices of level value equal to the thistle's so that we find
	//                 (N ... N t O ... O)
	// where
	// - N denotes the neighbors of the thistle
	// - t is the thistle
	// - O are the other vertices
	sort_level_sequences(
		n, thistle, is_thistle_neighbor, levels_per_vertex, inv_arr
	);

	// Construct the arrangement object now, since it is needed for further
	// operations. Object 'inv_arr' is no longer needed.
	arr = linear_arrangement::from_inverse(inv_arr.begin(), inv_arr.end());

#if defined DEBUG
#if defined LAL_DEBUG_DMax_1_thistle
	print_arrangement("After sorting all sequences of equal level value", arr);
#endif
	assert(linarr::is_arrangement(t, arr));
	const uint64_t _sum_lengths_2 = linarr::sum_edge_lengths(t, arr);
#if defined LAL_DEBUG_DMax_1_thistle
	std::cout << "        _sum_lengths_2= " << _sum_lengths_2 << '\n';
#endif
	assert(_sum_lengths_2 == _sum_lengths_1);
#endif

	adjust_nonneighbors_of_thistle_smart(
		t, thistle_level, thistle, is_thistle_neighbor, levels_per_vertex, arr
	);

	/*
	adjust_nonneighbors_of_thistle_exhaustive
		(t, thistle, is_thistle_neighbor, arr);
	*/

#if defined DEBUG
#if defined LAL_DEBUG_DMax_1_thistle
	print_arrangement(
		"After moving thistle and readjusting other vertices", arr
	);
#endif
	assert(linarr::is_arrangement(t, arr));
#endif

	const uint64_t D = linarr::sum_edge_lengths(t, arr);

#if defined DEBUG
#if defined LAL_DEBUG_DMax_1_thistle
	std::cout << "        D= " << D << '\n';
#endif
	assert(D >= _sum_lengths_2);
#endif

	if constexpr (make_arrangement) {
		if (res.first < D) {
			res.first = D;
			res.second = std::move(arr);
		}
	}
	else {
		res = std::max(res, D);
	}
}

/**
 * @brief Tries to make a maximal arrangement with a given thistle vertex of a
 * given level value
 *
 * Parameters @e arr, @e inv_arr, @e level_per_vertex are temporary memory
 * passed as parameters to avoid constant allocations and deallocations.
 * @tparam make_arrangement Should the arrangement be returned?
 * @param t Input free tree.
 * @param thistle Thistle vertex.
 * @param is_thistle_neighbor Is a given vertex a neighbor of @e thistle?
 * @param arr Linear arrangement (used to evaluate the cost of the distribution).
 * @param inv_arr Inverse linear arrangement (used to evaluate the cost of the
 * distribution).
 * @param level_per_vertex Array containing the level value of each vertex of
 * the tree.
 * @param thistle_side_per_vertex For each vertex, the side of the thistle at which
 * they are to be placed.
 * @param[inout] res Contains the best arrangement produced here if it is better
 * than the one it already contains.
 */
template <bool make_arrangement>
inline void choose_orientations_for_thistle_neighbors(
	const graphs::free_tree& t,
	const node thistle,
	const array<char>& is_thistle_neighbor,

	linear_arrangement& arr,
	array<node>& inv_arr,
	level_signature_per_vertex& level_per_vertex,
	array<char>& thistle_side_per_vertex,

	result_t<make_arrangement>& res
) noexcept
{
	const auto thistle_deg = t.get_degree(thistle);
	const neighbourhood& thistle_neighs = t.get_neighbors(thistle);

	array<char> binary_combination(thistle_deg, 0);

#if defined LAL_DEBUG_DMax_1_thistle
	std::cout << "Thistle: " << thistle << '\n';
	int counter = 0;
#endif

#if defined DEBUG
	std::size_t num_combinations = 0;
#endif

	BFS bfs(t);
	bfs.set_process_neighbour(
		[&](const auto&, node u, node v, bool)
		{
			thistle_side_per_vertex[v] = other_side(thistle_side_per_vertex[u]);
		}
	);

	do {
#if defined LAL_DEBUG_DMax_1_thistle
		std::cout << "    Binary combination: " << counter++ << '\n';
		std::cout << "    ";
		for (std::size_t j = 0; j < thistle_deg; ++j) {
			std::cout << int(binary_combination[j]) << ' ';
		}
		std::cout << '\n';
#endif

#if defined DEBUG
		++num_combinations;
#endif

		// calculate the level of the thistle
		int64_t thistle_level = 0;
		for (std::size_t i = 0; i < thistle_neighs.size(); ++i) {
			if (binary_combination[i] == LEFT_SIDE) {
				// Neighbour of the thistle goes to the left half of the arrangement.
				--thistle_level;
				thistle_side_per_vertex[thistle_neighs[i]] = LEFT_SIDE;
			}
			else {
				// Neighbour of the thistle goes to the right half of the arrangement.
				++thistle_level;
				thistle_side_per_vertex[thistle_neighs[i]] = RIGHT_SIDE;
			}
		}

		// ignore orientations where the root is not a thistle vertex
		if (thistle_level >= 0 and to_uint64(thistle_level) != thistle_deg) {

			bfs.set_visited(thistle, true);
			for (std::size_t i = 0; i < thistle_neighs.size(); ++i) {
				bfs.start_at(thistle_neighs[i]);
			}
			bfs.clear_visited();
			bfs.clear_queue();

			// merge the arrangements and keep track of the maximum
			merge_arrangements<make_arrangement>(
				t,
				thistle,
				thistle_level,
				is_thistle_neighbor,
				thistle_side_per_vertex,
				arr,
				inv_arr,
				level_per_vertex,
				res
			);
		}
#if defined LAL_DEBUG_DMax_1_thistle
		else {
			std::cout << "        Ignore negative values and non-thistle "
						 "configurations.\n";
			std::cout << "        Level value: " << thistle_level << '\n';
			std::cout << "        Degree: " << t.get_degree(thistle) << '\n';
		}
#endif
	}
	while (next_binary(binary_combination.begin(), binary_combination.end()));

#if defined DEBUG
	assert(num_combinations == 1ull << thistle_deg);
#endif
}

} // namespace bits

/**
 * @brief Maximal non-bipartite Arrangement with exactly 1 thistle vertex.
 *
 * This function implements the algorithm in \cite Alemany2023a.
 * @tparam make_arrangement Boolean value that indicates whether or not the maximal
 * arrangement should be constructed.
 * @param t Input tree.
 * @param all_paths The set of all paths that span sequences of vertices of degree 2.
 * @param node_to_path An index array that points every degree-2 vertex to its
 * path in @e all_paths.
 * @returns The cost of a maximal non-bipartite arrangement with exactly 1 thistle
 * vertex.
 */
template <bool make_arrangement>
[[nodiscard]] inline bits::result_t<make_arrangement>
AEF(const graphs::free_tree& t,
	const std::vector<properties::branchless_path>& all_paths,
	const array<std::size_t>& node_to_path) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Alemany2023a);
#endif

#if defined DEBUG
	assert(t.is_tree());
#endif

	const auto n = t.get_num_nodes();

#if defined LAL_DEBUG_DMax_1_thistle
	{
		std::cout << "Start tree\n";
		std::cout << t << '\n';
		const auto hv = t.get_head_vector(0);
		std::cout << hv[0];
		for (std::size_t i = 1; i < hv.size(); ++i) {
			std::cout << ' ' << hv[i];
		}
		std::cout << '\n';
	}
#endif

	bits::result_t<make_arrangement> res;
	if constexpr (make_arrangement) {
		res.first = 0;
		res.second.resize(1);
	}
	else {
		res = 0;
	}

	// whether or an internal of a branchless path was already used
	array<char> internal_in_path_was_used(all_paths.size(), 0);

	// actual linear arrangement
	linear_arrangement arr(n);
	// simple inverse arrangement
	array<node> inv_arr(n);
	// the level value per vertex
	level_signature_per_vertex level_per_vertex(n);
	// the side of the thistle at which every vertex is found
	array<char> thistle_side_per_vertex(n);
	// used to query whether a vertex is neighbor of the thistle or not
	array<char> is_thistle_neighbor(n, 0);

	for (node thistle = 0; thistle < n; ++thistle) {
		const uint64_t deg_thistle = t.get_degree(thistle);

		// ignore leaves
		if (deg_thistle == 1) {
			continue;
		}

		// do we have to use this internal vertex of a branchless path as a thistle?
		if (deg_thistle == 2) {
			const std::size_t pidx = node_to_path[thistle];
			// not in this case
			if (internal_in_path_was_used[pidx] == 1) {
#if defined LAL_DEBUG_DMax_1_thistle
				std::cout << "Thistle belongs to path " << pidx
						  << " and will not be tested.\n";
#endif
				continue;
			}

			// do not use internal vertices of this branchless path anymore
			internal_in_path_was_used[pidx] = 1;
#if defined LAL_DEBUG_DMax_1_thistle
			std::cout << "Thistle vertex belongs to path " << pidx << ".\n";
#endif
		}

		const neighbourhood& neighs = t.get_neighbors(thistle);

		// set neighbors of thistle
		for (node u : neighs) {
			is_thistle_neighbor[u] = 1;
		}

		// Find best orientation for this thistle.
		bits::choose_orientations_for_thistle_neighbors<make_arrangement>(
			t,
			thistle,
			is_thistle_neighbor,
			arr,
			inv_arr,
			level_per_vertex,
			thistle_side_per_vertex,
			res
		);

		// unset neighbors of thistle
		for (node u : neighs) {
			is_thistle_neighbor[u] = 0;
		}
	}

#if defined LAL_DEBUG_DMax_1_thistle
	{
		std::cout << "Finish tree\n";
		std::cout << t << '\n';
		const auto hv = t.get_head_vector(0);
		std::cout << hv[0];
		for (std::size_t i = 1; i < hv.size(); ++i) {
			std::cout << ' ' << hv[i];
		}
		std::cout << '\n';
	}
#endif

	return res;
}

/**
 * @brief Maximal non-bipartite Arrangement with exactly 1 thistle vertex.
 *
 * This function implements the algorithm in \cite Alemany2023a.
 * @tparam make_arrangement Boolean value that indicates whether or not the maximal
 * arrangement should be constructed.
 * @param t Input tree.
 * @param all_paths The set of all paths that span sequences of vertices of degree 2.
 * @returns The cost of a maximal non-bipartite arrangement with exactly 1 thistle
 * vertex.
 * @pre Input tree @e t must be a valid tree (see @ref lal::graphs::tree::is_tree).
 */
template <bool make_arrangement>
[[nodiscard]] inline std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t>
AEF(const graphs::free_tree& t,
	const std::vector<properties::branchless_path>& all_paths) noexcept
{
#if defined DEBUG
	assert(t.is_tree());
#endif

	// assign all internal vertices a path index.
	array<std::size_t> node_to_path(t.get_num_nodes());
	for (std::size_t i = 0; i < all_paths.size(); ++i) {
		const properties::branchless_path& p = all_paths[i];
		const std::vector<node>& seq = p.get_vertex_sequence();
		for (std::size_t j = 1; j < seq.size() - 1; ++j) {
			const node u = seq[j];
			node_to_path[u] = i;
		}
	}

	return AEF<make_arrangement>(t, all_paths, node_to_path);
}

#undef lal_level_position_
#undef lal_level_vertex_

} // namespace thistle_1
} // namespace DMax
} // namespace detail
} // namespace lal
