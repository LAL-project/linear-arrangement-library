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
#pragma message("PRINT_MESSAGES_1THISTLE can only be defined when DEBUG is defined")
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

/**
 * @brief Sorts the vertices of level +1 within their own interval.
 *
 * In such a way that
 * - the neighbors of the thistle are placed to the leftmost positions,
 * - then comes the thistle,
 * - and then the remaining vertices are placed in the rightmost positions.
 *
 * This is Ok thanks to Nurse and De Vos \cite Nurse2018a \cite Nurse2019a.
 * @param thistle Thistle vertex.
 * @param p Limit of the interval.
 * @param is_thistle_neighbor Array to query if a vertex is a neighbor of the thisle.
 * @param levels_per_vertex Array of levels per vertex.
 * @param[out] inv_arr Inverse linear arrangement.
 */
inline void sort_level_p1(
	node thistle,
	const position p,
	const data_array<char>& is_thistle_neighbor,
	// the level signature of the arrangement
	const level_signature_per_vertex& levels_per_vertex,
	// the ¡inverse! linear arrangement
	data_array<node>& inv_arr
)
noexcept
{
	position left = 0;
	while (levels_per_vertex[ node_t{inv_arr[left]} ] > 1 and left < p) { ++left; }

	if (left == p) {
		// no vertices of level +1
		return;
	}

	position right = left;
	while (levels_per_vertex[ node_t{inv_arr[right]} ] == 1) { ++right; }

	sorting::counting_sort
		<node, sorting::non_decreasing_t>
		(
			inv_arr.begin() + left, inv_arr.begin() + right, 2, right - left,
			[&](const node u) {
				if (is_thistle_neighbor[u]) { return 0; }
				if (u == thistle) { return 1; }
				return 2;
			}
		);
}

/**
 * @brief Readjusts vertices to increase the cost of the arrangement.
 *
 * Moves vertices to the right starting at vertices of level +2.
 * @param t Input tree.
 * @param thistle Thistle vertex.
 * @param num_leaves_left NUmber of leaves to the left of the thistle.
 * @param p Position of the vertex to readjust.
 * @param levels_per_vertex Level signature of the arrangement -- not updated
 * @param[out] arr Actual linear arrangement
 * @returns Whether or not the current vertex is readjusted.
 * @pre Position p is that of a vertex of level >+1.
 * @post The arrangement is updated but the level signature is not.
 */
inline bool shift_vertex_to_right(
	const graphs::free_tree& t,
	const node thistle,
	const int64_t num_leaves_left,
	position_t p,

	// the level signature of the arrangement
	const level_signature_per_vertex& levels_per_vertex,

	// the actual linear arrangement
	linear_arrangement& arr
)
noexcept
{
	const node_t to_move = arr[p];
	const auto n = t.get_num_nodes();
	const auto r =
		num_leaves_left*(1 - levels_per_vertex[to_move])
		+ levels_per_vertex[node_t{thistle}]
		- levels_per_vertex[to_move];

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "        Theoretical gain: " << r << '\n';
#endif

	if (r <= 0) { return false; }

	while (p < n - 1 and arr[p + 1ull] != thistle) {
		arr.swap(p, p + 1ull);
		++p;
	}
	arr.swap(p, p + 1ull);

	return true;
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
	int64_t thistle_level,
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

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "        Chosen level value: " << thistle_level << '\n';
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
			levels_per_vertex[node_t{u}] = d;
			inv_arr[left++] = u;
		}
		else if (thistle_side_per_vertex[u] == RIGHT_SIDE) {
			levels_per_vertex[node_t{u}] = -d;
			inv_arr[right--] = u;
			min_level_value = std::min(min_level_value, -d);
		}
#if defined DEBUG
		else {
#if defined PRINT_MESSAGES_1THISTLE
			std::cout
				<< "Vertex " << u << " was not assigned a side\n";
#endif
			assert(false);
		}
#endif
	}

	// This function assumes that the thistle will never have negative (<0)
	// level value.
	levels_per_vertex[node_t{thistle}] = thistle_level;
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
				return to_uint64(levels_per_vertex[node_t{u}] - min_level_value);
			}
		);
	sorting::counting_sort
		<node, sorting::non_increasing_t>
		(
			inv_arr.begin() + right + 1, inv_arr.end(), 2*n, n,
			[&](const node u) {
				return to_uint64(levels_per_vertex[node_t{u}] - min_level_value);
			}
		);

#if defined DEBUG
	arr = linear_arrangement::from_inverse(inv_arr.begin(), inv_arr.end());
#endif

#if defined PRINT_MESSAGES_1THISTLE
	{
	const auto dir = arr.direct_as_vector();
	const auto inv = arr.inverse_as_vector();
	std::cout << "        Before sorting vertices of level +1:\n";
	std::cout << "        Direct: ";
	for (std::size_t i = 0; i < dir.size(); ++i) { std::cout << ' ' << dir[i]; }
	std::cout << '\n';
	std::cout << "        Inverse:";
	for (std::size_t i = 0; i < inv.size(); ++i) { std::cout << ' ' << inv[i]; }
	std::cout << '\n';
	}
#endif

#if defined DEBUG
	assert(linarr::is_arrangement(t, arr));

	// sum of edge lengths prior to adjustments
	const auto __D1 = linarr::sum_edge_lengths(t, arr);

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "        __D1= " << __D1 << std::endl;
#endif
#endif

	// sort the vertices of level +1 so that the leaves and the thistle are placed
	// at the leftmost side of the interval
	sort_level_p1(thistle, left, is_thistle_neighbor, levels_per_vertex, inv_arr);

	// build the actual arrangement object now
	arr = linear_arrangement::from_inverse(inv_arr.begin(), inv_arr.end());


#if defined PRINT_MESSAGES_1THISTLE
	{
	const auto dir = arr.direct_as_vector();
	const auto inv = arr.inverse_as_vector();
	std::cout << "        after sorting vertices level +1:\n";
	std::cout << "        Direct: ";
	for (std::size_t i = 0; i < dir.size(); ++i) { std::cout << ' ' << dir[i]; }
	std::cout << '\n';
	std::cout << "        Inverse:";
	for (std::size_t i = 0; i < inv.size(); ++i) { std::cout << ' ' << inv[i]; }
	std::cout << '\n';
	}
#endif


#if defined DEBUG
	assert(linarr::is_arrangement(t, arr));

	// sum of edge lengths prior to adjustments
	const auto __D2 = linarr::sum_edge_lengths(t, arr);

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "        __D2= " << __D2 << std::endl;
#endif

	assert(__D1 == __D2);
#endif


	{
	// Move the thistle to the left
	// - while the swap with its neighbor increases the cost
	// - while 'thistle' is kept an actual thistle vertex
	position_t p = arr[node_t{thistle}];
	while (
		p > 0ull and
		levels_per_vertex[node_t{arr[p - 1ull]}] <= levels_per_vertex[node_t{arr[p]}] and
		(is_thistle_neighbor[ arr[p - 1ull] ] == 0)
	)
	{
		arr.swap(p - 1ull, p);
		--p;
	}
	}

	if (thistle_level > 0) {
		// If the thistle only has leaves to its left then readjust the vertices
		// of level >1 appropriately.
		bool all_leaves_left = true;
		int64_t num_leaves_left = 0;
		for (node u : t.get_neighbours(thistle)) {
			if (thistle_side_per_vertex[u] == LEFT_SIDE) {
				all_leaves_left = all_leaves_left and (t.get_degree(u) == 1);
				num_leaves_left += t.get_degree(u) == 1;
			}
		}
		if (all_leaves_left) {
			// locate the first non-neighbor: due to the sorting step of the
			// vertices of level +1 step above, this first non-neighbor is
			// guaranteed to be a vertex of level >+2 (if it exists).

#if defined PRINT_MESSAGES_1THISTLE
			std::cout << "        The thistle only has leaves to its left\n";
			std::cout << "        Number of leaves: " << num_leaves_left << '\n';
#endif

			position_t p = arr[node_t{thistle}] - 1ull;
			while (p > 0ull and is_thistle_neighbor[ arr[p] ]) { --p; }

#if defined PRINT_MESSAGES_1THISTLE
			std::cout << "        The first non-neighbor to the left is at position "
					  << *p << '\n';
#endif

			if ((p > 0ull) or (p == 0ull and not is_thistle_neighbor[arr[p]])) {

				bool cont = true;
				while (p > 0ull and cont) {
					cont =
						shift_vertex_to_right
						(t, thistle, num_leaves_left, p, levels_per_vertex, arr);
					--p;
				}
				if (cont) {
					shift_vertex_to_right
					(t, thistle, num_leaves_left, p, levels_per_vertex, arr);
				}
			}

		}
	}

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

	const auto D = linarr::sum_edge_lengths(t, arr);
#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "          D=  " << D << '\n';
#endif

#if defined DEBUG
	assert(D >= __D2);
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
void choose_orientations_for_root(
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
							thistle_side_per_vertex[u] =
								thistle_side_per_vertex[neigh];
						}
						else {
							thistle_side_per_vertex[u] =
								other_side(thistle_side_per_vertex[neigh]);
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
detail::result_t<make_arrangement>
AEF(const graphs::free_tree& t, const properties::bipartite_graph_coloring& c)
noexcept
{
	const auto n = t.get_num_nodes();

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "New tree!\n";
	std::cout << t << '\n';
#endif

	detail::result_t<make_arrangement> res;
	if constexpr (make_arrangement) {
		res.first = 0;
		res.second.resize(1);
	}
	else {
		res = 0;
	}

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
		detail::choose_orientations_for_root<make_arrangement>(
			t, thistle, is_thistle_neighbor, nodes_subtrees, c,
			arr, inv_arr, level_per_vertex, thistle_side_per_vertex,
			res
		);

		// unset neighbors of thistle
		for (node u : neighs) {
			is_thistle_neighbor[u] = 0;
		}
	}

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
AEF(const graphs::free_tree& g) noexcept {
	const auto c = color_vertices_graph(g);
	return AEF<make_arrangement>(g, c);
}

} // -- namespace thistle_1
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
