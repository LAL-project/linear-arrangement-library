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
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

#undef PRINT_MESSAGES_1THISTLE

#if not defined DEBUG
#if defined PRINT_MESSAGES_1THISTLE
#error "PRINT_MESSAGES_1THISTLE can only be defined when DEBUG is defined"
#endif
#endif

// C++ includes
#include <type_traits>
#include <algorithm>
#include <cstdint>
#include <vector>

#if defined PRINT_MESSAGES_1THISTLE
#include <iostream>
#endif
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/formal_constraints.hpp>
#include <lal/linarr/D/D.hpp>
#if defined DEBUG
#include <lal/linarr/formal_constraints.hpp>
#endif
#if defined PRINT_MESSAGES_1THISTLE
#include <lal/graphs/output.hpp>
#endif

#include <lal/detail/properties/bipartite_graph_colorability.hpp>
#include <lal/detail/properties/branchless_path_find.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/linarr/level_signature.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace thistle_1 {

namespace detail {

/**
 * @brief Next binary combination of 0's and 1's.
 * @tparam iterator_t Type of pointer to a sequence.
 * @param begin Start of the sequence.
 * @param end End of the sequence.
 * @returns Whether or not there are more configurations
 */
template <typename iterator_t>
bool next_binary(iterator_t begin, iterator_t end) noexcept
{
	while (begin != end and *begin == 1) {
		*begin = 0;
		++begin;
	}
	// reached the last configuration   "1......1"
	// which now has been turned into a "0......0"
	if (begin == end) { return false; }

	// the configuration is "0....01x...x"
	*begin = 1;
	// there are still more binary configurations
	return true;
}

/// Useful typedef for this algorithm
typedef std::vector<lal::node> node_set;
/// Typedef for @ref properties::bipartite_graph_coloring::blue.
static constexpr auto blue = properties::bipartite_graph_coloring::blue;
/// Typedef for @ref properties::bipartite_graph_coloring::red.
static constexpr auto red = properties::bipartite_graph_coloring::red;

/// Left side of the thistle vertex.
static constexpr char LEFT_SIDE = 0;
/// Right side of the thistle vertex.
static constexpr char RIGHT_SIDE = 1;
/// The other side. "Right" if 'side' is "left"; "left" if 'side' is "right".
static constexpr char other_side(char side) noexcept {
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
	uint64_t
>;

#define level_position(p) levels_per_vertex[node_t{inv_arr[p]}]
#define level_vertex(u)	  levels_per_vertex[node_t{u}]

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
 * @param levels_per_vertex Array of levels per vertex.
 * @param[out] inv_arr Inverse linear arrangement.
 */
inline void sort_level_sequences(
	const uint64_t n,
	const node thistle,
	const data_array<char>& is_thistle_neighbor,
	// the level signature of the arrangement
	const level_signature_per_vertex& levels_per_vertex,
	// the ¡inverse! linear arrangement
	data_array<node>& inv_arr
)
noexcept
{
	position p = 0;
	while (p < n) {

		position q = p;
		const int64_t current_level = level_position(p);
		while (q < n and level_position(q) == current_level) {
			++q;
		}

		// sort interval [p,q)

#if defined PRINT_MESSAGES_1THISTLE
		std::cout << "        Sort the interval [" << p << ", " << q << ").\n";
#endif

		sorting::counting_sort
		<node, sorting::non_decreasing_t>
		(
			inv_arr.begin() + p, inv_arr.begin() + q, 2, q - p,
			[&](const node u) {
				// 0, 1, 2
				if (is_thistle_neighbor[u] == 1) { return 0; }
				if (u == thistle) { return 1; }
				return 2;
			}
		);

		p = q;
	}
}

/**
 * @brief Readjusts vertices to increase the cost of the arrangement.
 *
 * Moves vertices to the right of the thistle. It stops when 'p' was moved to
 * the right of the thistle.
 * @param t Input tree.
 * @param thistle Thistle vertex.
 * @param p Position of the vertex to readjust.
 * @param[out] arr Actual linear arrangement
 * @post The arrangement is updated but the level signature is not.
 */
inline void shift_vertex_to_right(
	const graphs::free_tree& t,
	const node thistle,
	position_t p,

	// the actual linear arrangement
	linear_arrangement& arr
)
noexcept
{
	const auto n = t.get_num_nodes();

	while (p < n - 1 and arr[p + 1ull] != thistle) {
		arr.swap(p, p + 1ull);
		++p;
	}
	arr.swap(p, p + 1ull);
}

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
 * @param is_thistle_neighbor Is a given vertex a neighbor of @e thistle?
 * @param thistle_side_per_vertex Side of @e thistle in which each vertex
 * have to be placed at in the first arrangement.
 * @param arr Linear arrangement (used to evaluate the cost of the distribution).
 * @param inv_arr Inverse linear arrangement (used to evaluate the cost of the
 * distribution).
 * @param levels_per_vertex Array containing the level value of each vertex of
 * the tree.
 * @param[inout] res Contains the best arrangement produced here if it is better
 * than the one it already contains.
 */
template <bool make_arrangement>
void merge_arrangements(
	const graphs::free_tree& t,
	const node thistle,
	const int64_t thistle_level,
	const data_array<char>& is_thistle_neighbor,
	const data_array<char>& thistle_side_per_vertex,

	// the actual linear arrangement
	linear_arrangement& arr,
	// the ¡inverse! linear arrangement
	data_array<node>& inv_arr,
	// the level signature of the arrangement
	level_signature_per_vertex& levels_per_vertex,

	result_t<make_arrangement>& res
)
noexcept
{
	const auto n = t.get_num_nodes();

#if defined DEBUG
#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "        Chosen level value: " << thistle_level << '\n';
#endif
#endif

	// The minimum level value in the configuration. There is always
	// a negative (<0) level value, so the minimum can be initialized at 0.
	int64_t min_level_value = 0;

	position left = 0;
	position right = n - 1ull;

	// Calculate the level for each vertex and place them in the inverse
	// arrangement. The arrangement built at this step is only preliminary.
	for (node u = 0; u < n; ++u) {
		if (u == thistle) { continue; }
		const int64_t d = to_int64(t.get_degree(u));
		if (thistle_side_per_vertex[u] == LEFT_SIDE) {
			level_vertex(u) = d;
			inv_arr[left++] = u;
		}
		else if (thistle_side_per_vertex[u] == RIGHT_SIDE) {
			level_vertex(u) = -d;
			inv_arr[right--] = u;
			min_level_value = std::min(min_level_value, -d);
		}
#if defined DEBUG
		else {
#if defined PRINT_MESSAGES_1THISTLE
			std::cout << "Vertex " << u << " was not assigned a side\n";
#endif
			assert(false);
		}
#endif
	}

	// This function assumes that the thistle will never have negative (<0)
	// level value.
	level_vertex(thistle) = thistle_level;
	// The position to place the thistle is either 'left' or 'right' since,
	// at this point, their values are equal (see next assertion).
	inv_arr[left] = thistle;

#if defined DEBUG
	assert(left == right);
#endif

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "        Level per vertex:\n";
	for (node u = 0; u < n; ++u) {
		std::cout
			<< "            level[" << u << "]= "
			<< levels_per_vertex[node_t{u}]
			<< '\n';
	}
#endif

	// Sort the vertices by level (first, those with positive level. Then, those
	// with negative level). Independent tasks.
	sorting::counting_sort
		<node, sorting::non_increasing_t>
		(
			inv_arr.begin(), inv_arr.begin() + left, 2*n, n,
			[&](const node u) {
			return to_uint64(level_vertex(u) - min_level_value);
			}
		);
	sorting::counting_sort
		<node, sorting::non_increasing_t>
		(
			inv_arr.begin() + right + 1, inv_arr.end(), 2*n, n,
			[&](const node u) {
				return to_uint64(level_vertex(u) - min_level_value);
			}
		);

#if defined DEBUG
	arr = linear_arrangement::from_inverse(inv_arr.begin(), inv_arr.end());

#if defined PRINT_MESSAGES_1THISTLE
	{
	const auto dir = arr.direct_as_vector();
	const auto inv = arr.inverse_as_vector();
	std::cout << "        Initial arrangement:\n";
	std::cout << "        Direct: ";
	for (std::size_t i = 0; i < dir.size(); ++i) { std::cout << ' ' << dir[i]; }
	std::cout << '\n';
	std::cout << "        Inverse:";
	for (std::size_t i = 0; i < inv.size(); ++i) { std::cout << ' ' << inv[i]; }
	std::cout << '\n';
	}
#endif

	assert(linarr::is_arrangement(t, arr));

	// sum of edge lengths prior to adjustments
	const uint64_t __D1 = linarr::sum_edge_lengths(t, arr);

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "        __D1= " << __D1 << std::endl;
#endif
#endif

	// sort the vertices of level value equal to the thistle's so that we find
	//                 (N ... N t O ... O)
	// where
	// - N denotes the neighbors of the thistle
	// - t is the thistle
	// - O are the other vertices
	sort_level_sequences(n, thistle, is_thistle_neighbor, levels_per_vertex, inv_arr);

	arr = linear_arrangement::from_inverse(inv_arr.begin(), inv_arr.end());

#if defined DEBUG

#if defined PRINT_MESSAGES_1THISTLE
	{
	const auto dir = arr.direct_as_vector();
	const auto inv = arr.inverse_as_vector();
	std::cout << "        After sorting all sequences of equal level vertex:\n";
	std::cout << "        Direct: ";
	for (std::size_t i = 0; i < dir.size(); ++i) { std::cout << ' ' << dir[i]; }
	std::cout << '\n';
	std::cout << "        Inverse:";
	for (std::size_t i = 0; i < inv.size(); ++i) { std::cout << ' ' << inv[i]; }
	std::cout << '\n';
	}
#endif

	assert(linarr::is_arrangement(t, arr));
	const uint64_t __D2 = linarr::sum_edge_lengths(t, arr);

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "        __D2= " << __D2 << std::endl;
#endif

	assert(__D2 == __D1);
#endif

	{
	// Move the thistle to the left
	// - while the level value of the vertex to the left is less than the thistle's
	// - while the other vertex is not a neighbor of the thistle
	position_t p = arr[node_t{thistle}];
	while (
		p > 0ull and
		level_vertex(arr[p - 1ull]) <= level_vertex(arr[p]) and
		(is_thistle_neighbor[arr[p - 1ull]] == 0)
	)
	{
		arr.swap(p - 1ull, p);
		--p;
	}
	}

#if defined DEBUG

#if defined PRINT_MESSAGES_1THISTLE
	{
	const auto dir = arr.direct_as_vector();
	const auto inv = arr.inverse_as_vector();
	std::cout << "        After adjusting the thistle vertex:\n";
	std::cout << "        Direct: ";
	for (std::size_t i = 0; i < dir.size(); ++i) { std::cout << ' ' << dir[i]; }
	std::cout << '\n';
	std::cout << "        Inverse:";
	for (std::size_t i = 0; i < inv.size(); ++i) { std::cout << ' ' << inv[i]; }
	std::cout << '\n';
	}
#endif

	assert(linarr::is_arrangement(t, arr));
	const uint64_t __D3 = linarr::sum_edge_lengths(t, arr);

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "        __D3= " << __D3 << std::endl;
#endif

	assert(__D3 >= __D2);
#endif


#if defined DEBUG
	assert(arr[node_t{thistle}] != 0);
#endif

	{
	// position of thistle can never be '0'
	position p = arr[node_t{thistle}] - 1;

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "        Thistle position p= " << p+1 << '\n';
#endif

	bool stop = false;
	while (p > 0 and not stop) {

#if defined PRINT_MESSAGES_1THISTLE
		std::cout << "        __________________________________________\n";
		std::cout << "        Find next non-neighbor starting at p= " << p << '\n';
#endif

		// number of vertices between the thistle and the first non-neighbor
		int64_t j = 0;
		// sum of level values between the thistle and the first non-neighbor
		int64_t total_level_value = 0;
		position q = p;
		// find the first non-neighbor of 'thistle'
		while (q > 0 and is_thistle_neighbor[arr[position_t{q}]] == 1) {
			total_level_value += level_vertex(arr[position_t{q}]);
			++j;

#if defined PRINT_MESSAGES_1THISTLE
			std::cout << "            Vertex " << arr[position_t{q}] << " is a neighbor\n";
			std::cout << "                Position " << q << '\n';
			std::cout << "            Of level value:      " << level_vertex(arr[position_t{q}]) << '\n';
			std::cout << "            Sum of level values: " << total_level_value << '\n';
			std::cout << "            Number of vertices:  " << j << '\n';
#endif

			--q;
		}

		const node to_move = arr[position_t{q}];

		if (is_thistle_neighbor[to_move] == 1) {
			if (q == 0) {
				stop = true;
			}
			else {
				// continue one more iteration
				p = q - 1;
			}
		}
		else {
			const int64_t level_nonneigh = level_vertex(to_move);

#if defined PRINT_MESSAGES_1THISTLE
			std::cout << "            First non-neighbor: " << to_move << '\n';
			std::cout << "                   at position: " << q << '\n';
			std::cout << "                      of level: " << level_nonneigh << '\n';
			std::cout << "            Total level values: " << total_level_value << '\n';
			std::cout << "            Number of vertices: " << j << '\n';
			std::cout << "            -(j + 1)*level_nonneigh= " << -(j + 1)*level_nonneigh << '\n';
#endif

			const int64_t r = -(j + 1)*level_nonneigh + total_level_value + thistle_level;

#if defined PRINT_MESSAGES_1THISTLE
			std::cout << "        r= " << r << '\n';
#endif

			if (r > 0) {
				// move the vertex at position 'q' to the right of the thistle
				shift_vertex_to_right(t, thistle, q, arr);
				// the thistle is now at position 'q'
			}

			stop = r <= 0 or q == 0;
			if (q > 0) {
				p = arr[node_t{thistle}] - 1;
			}
		}

#if defined PRINT_MESSAGES_1THISTLE
		{
		const auto dir = arr.direct_as_vector();
		const auto inv = arr.inverse_as_vector();
		std::cout << "        After moving vertex '" << to_move << "'\n";
		std::cout << "        Direct: ";
		for (std::size_t i = 0; i < dir.size(); ++i) { std::cout << ' ' << dir[i]; }
		std::cout << '\n';
		std::cout << "        Inverse:";
		for (std::size_t i = 0; i < inv.size(); ++i) { std::cout << ' ' << inv[i]; }
		std::cout << '\n';
		}
#endif
	}
	}

#if defined DEBUG

#if defined PRINT_MESSAGES_1THISTLE
	{
	const auto dir = arr.direct_as_vector();
	const auto inv = arr.inverse_as_vector();
	std::cout << "        After moving thistle and readjusting other vertices:\n";
	std::cout << "        Direct: ";
	for (std::size_t i = 0; i < dir.size(); ++i) { std::cout << ' ' << dir[i]; }
	std::cout << '\n';
	std::cout << "        Inverse:";
	for (std::size_t i = 0; i < inv.size(); ++i) { std::cout << ' ' << inv[i]; }
	std::cout << '\n';
	}
	std::cout << "        Level values:\n";
	std::cout << "        levels_per_vertex:\n";
	for (node u = 0; u < n; ++u) {
		std::cout << "        " << u << ": " << levels_per_vertex[node_t{u}] << '\n';
	}
	std::cout << '\n';
#endif

	assert(linarr::is_arrangement(t, arr));
#endif

	const uint64_t D = linarr::sum_edge_lengths(t, arr);

#if defined DEBUG
#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "        D= " << D << '\n';
#endif
	assert(D >= __D3);
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
 * Parameters @e arr, @e inv_arr, @e levels_per_vertex are temporary memory
 * passed as parameters to avoid constant allocations and deallocations.
 * @tparam make_arrangement Should the arrangement be returned?
 * @param t Input free tree.
 * @param thistle Thistle vertex.
 * @param is_thistle_neighbor Is a given vertex a neighbor of @e thistle?
 * @param color_per_vertex Coloring of the tree.
 * @param oriented_vertices Vertices of the connected components sorted by degree
 * and color.
 * @param arr Linear arrangement (used to evaluate the cost of the distribution).
 * @param inv_arr Inverse linear arrangement (used to evaluate the cost of the
 * distribution).
 * @param levels_per_vertex Array containing the level value of each vertex of
 * the tree.
 * @param[inout] res Contains the best arrangement produced here if it is better
 * than the one it already contains.
 */
template <bool make_arrangement>
void choose_orientations_for_thistle_neighbors(
	const graphs::free_tree& t,
	const node thistle,
	const data_array<char>& is_thistle_neighbor,
	const data_array<node_set>& nodes_subtrees,
	const properties::bipartite_graph_coloring& color_per_vertex,

	linear_arrangement& arr,
	data_array<node>& inv_arr,
	level_signature_per_vertex& level_per_vertex,
	data_array<char>& thistle_side_per_vertex,

	result_t<make_arrangement>& res
)
noexcept
{
	const auto thistle_deg = t.get_degree(thistle);
	const neighbourhood& thistle_neighs = t.get_neighbours(thistle);

	data_array<char> binary_combination(thistle_deg, 0);

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "Thistle: " << thistle << '\n';
	int counter = 0;
#endif

#if defined DEBUG
	std::size_t num_combinations = 0;
#endif

	do {
#if defined PRINT_MESSAGES_1THISTLE
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
				thistle_side_per_vertex[ thistle_neighs[i] ] = LEFT_SIDE;
			}
			else {
				// Neighbour of the thistle goes to the right half of the arrangement.
				++thistle_level;
				thistle_side_per_vertex[ thistle_neighs[i] ] = RIGHT_SIDE;
			}
		}

		// ignore orientations where the root is not a thistle vertex
		if (thistle_level >= 0) {
			if (to_uint64(thistle_level) != thistle_deg) {

				// Decide the original side of the thistle at which each vertex goes.
				for (std::size_t i = 0; i < thistle_deg; ++i) {
					const node neigh = thistle_neighs[i];
					for (node u : nodes_subtrees[i]) {
						if (color_per_vertex[neigh] == color_per_vertex[u]) {
							thistle_side_per_vertex[u] = thistle_side_per_vertex[neigh];
						}
						else {
							thistle_side_per_vertex[u] = other_side(thistle_side_per_vertex[neigh]);
						}
					}
				}

#if defined PRINT_MESSAGES_1THISTLE
				std::cout << "        Level of thistle: " << thistle_level << '\n';
				std::cout << "        Oriented vertices:\n";
				for (std::size_t i = 0; i < thistle_deg; ++i) {
					const node neigh = thistle_neighs[i];
					std::cout
						<< "            "
						<< (thistle_side_per_vertex[neigh] == LEFT_SIDE ? "(L)" : "(R)")
						<< ' ' << neigh
						<< '\n';
					for (std::size_t j = 0; j < nodes_subtrees[i].size(); ++j) {
						const node other = nodes_subtrees[i][j];
						std::cout
							<< "                "
							<< (thistle_side_per_vertex[other] == LEFT_SIDE ? "(L)" : "(R)")
							<< ' ' << other
							<< '\n';
					}
					std::cout << '\n';
				}
#endif

				// merge the arrangements and keep track of the maximum
				merge_arrangements<make_arrangement>(
					t,
					thistle, thistle_level,
					is_thistle_neighbor, thistle_side_per_vertex,
					arr, inv_arr, level_per_vertex,
					res
				);
			}
#if defined PRINT_MESSAGES_1THISTLE
			else {
				std::cout << "        This subset does not make the thistle be a thistle\n";
				std::cout << "        Level value: " << thistle_level << '\n';
				std::cout << "        Degree: " << t.get_degree(thistle) << '\n';
			}
#endif
		}
#if defined PRINT_MESSAGES_1THISTLE
		else {
			std::cout << "        Ignore negative level values\n";
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

} // -- namespace detail

/**
 * @brief Maximal non-bipartite Arrangement with exactly 1 thistle vertex.
 *
 * This function implements the algorithm in \cite Alemany2023a.
 * @tparam make_arrangement Boolean value that indicates whether or not the maximal
 * arrangement should be constructed.
 * @param t Input tree.
 * @returns The cost of a maximal bipartite arrangement and possibly the arrangement
 * that attains it.
 * @pre The input graph is a bipartite graph.
 */
template <bool make_arrangement>
detail::result_t<make_arrangement> AEF(
	const graphs::free_tree& t,
	const properties::bipartite_graph_coloring& c,
	const std::vector<properties::branchless_path>& all_paths,
	const data_array<std::size_t>& node_to_path
)
noexcept
{
	const auto n = t.get_num_nodes();

#if defined PRINT_MESSAGES_1THISTLE
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

	detail::result_t<make_arrangement> res;
	if constexpr (make_arrangement) {
		res.first = 0;
		res.second.resize(1);
	}
	else {
		res = 0;
	}

	// whether or an internal of a branchless path was already used
	data_array<char> internal_in_path_was_used(all_paths.size(), 0);

	// actual linear arrangement
	linear_arrangement arr(n);
	// simple inverse arrangement
	data_array<node> inv_arr(n);
	// the level value per vertex
	level_signature_per_vertex level_per_vertex(n);
	// the side of the thistle at which every vertex is found
	data_array<char> thistle_side_per_vertex(n);
	// the set of nodes in every subtree of the tree rooted at the thistle
	data_array<detail::node_set> nodes_subtrees;
	// used to query whether a vertex is neighbor of the thistle or not
	data_array<char> is_thistle_neighbor(n, 0);

	for (node thistle = 0; thistle < n; ++thistle) {
		const uint64_t deg_thistle = t.get_degree(thistle);

		// ignore leaves
		if (t.get_degree(thistle) == 1) { continue; }

		// do we have to use this internal vertex of a branchless path as a thistle?
		if (deg_thistle == 2) {
			const std::size_t pidx = node_to_path[thistle];
			// not in this case
			if (internal_in_path_was_used[pidx] == 1) {
#if defined PRINT_MESSAGES_1THISTLE
				std::cout << "Thistle belongs to path " << pidx << " and will not be tested.\n";
#endif
				continue;
			}

			// do not use internal vertices of this branchless path anymore
			internal_in_path_was_used[pidx] = 1;
#if defined PRINT_MESSAGES_1THISTLE
			std::cout << "Thistle vertex belongs to path " << pidx << ".\n";
#endif
		}

		nodes_subtrees.clear();
		nodes_subtrees.resize(deg_thistle);
		const graphs::rooted_tree rt(t, thistle);
		const neighbourhood& neighs = rt.get_out_neighbours(thistle);

		// set neighbors of thistle
		for (node u : neighs) {
			is_thistle_neighbor[u] = 1;
		}

		// Gather nodes of the connected components.
		// The nodes in the i-th container correspond to the connected component
		// of the i-th neighbour.
		for (node u = 0; u < n; ++u) {
			if (u == thistle) { continue; }
			for (std::size_t i = 0; i < deg_thistle; ++i) {
				if (rt.subtree_contains_node(neighs[i], u)) {
					nodes_subtrees[i].push_back(u);
				}
			}
		}

		// Find best orientation for this thistle.
		detail::choose_orientations_for_thistle_neighbors<make_arrangement>(
			t, thistle, is_thistle_neighbor, nodes_subtrees, c,
			arr, inv_arr, level_per_vertex, thistle_side_per_vertex,
			res
		);

		// unset neighbors of thistle
		for (node u : neighs) {
			is_thistle_neighbor[u] = 0;
		}
	}

#if defined PRINT_MESSAGES_1THISTLE
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
 * @returns The cost of a maximal bipartite arrangement and possibly the arrangement
 * that attains it.
 * @pre The input graph is a bipartite graph.
 */
template <bool make_arrangement>
std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t
>
AEF(
	const graphs::free_tree& t,
	const properties::bipartite_graph_coloring& c,
	const std::vector<properties::branchless_path>& all_paths
)
noexcept
{
	// assign all internal vertices a path index.
	data_array<std::size_t> node_to_path(t.get_num_nodes());
	for (std::size_t i = 0; i < all_paths.size(); ++i) {
		const properties::branchless_path& p = all_paths[i];
		const std::vector<node>& seq = p.get_vertex_sequence();
		for (std::size_t j = 1; j < seq.size() - 1; ++j) {
			const node u = seq[j];
			node_to_path[u] = i;
		}
	}

	return AEF<make_arrangement>(t, c, all_paths, node_to_path);
}

#undef level_position
#undef level_vertex

} // -- namespace thistle_1
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
