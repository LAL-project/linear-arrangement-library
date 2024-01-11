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

#define BUG

// C++ includes
#include <type_traits>
#include <algorithm>
#include <cstdint>
#include <vector>

#if defined PRINT_MESSAGES_1THISTLE || defined BUG
#include <iostream>
#endif
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/output.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/formal_constraints.hpp>
#include <lal/linarr/D/D.hpp>

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


template <bool make_arrangement>
using result_t = std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t
>;

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
 * @param color_per_vertex Coloring of the tree.
 * @param side_of_thistle Side of @e thistle in which the neighbors of the thistle
 * have to be placed at.
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
void merge_arrangements(
	const graphs::free_tree& t,
	const node thistle,
	int64_t thistle_level,
	const data_array<char>& is_thistle_neighbor,
	const properties::bipartite_graph_coloring& color_per_vertex,
	const data_array<char>& side_of_thistle,
	const data_array<node_set>& oriented_vertices,

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

	// The minimum level value in the configuration. There is always
	// a negative (<0) level value, so the minimum can be initialized at 0.
	int64_t min_level_value = 0;

	position left = 0;
	position right = n - 1ull;

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "        Chosen level value: " << thistle_level << '\n';
#endif

	for (std::size_t i = 0; i < oriented_vertices.size(); ++i) {
		for (node u : oriented_vertices[i]) {
			// calculate the level of this vertex.
			const int64_t d = to_int64(t.get_degree(u));

			// Case of single-vertex components
			if (oriented_vertices[i].size() == 1) {
				levels_per_vertex[node_t{u}] = (side_of_thistle[i] == LEFT_SIDE ? d : -d);
			}
			else {
				// Case where there is more than one vertex.
				// Here it is guaranteed that there will be more than one color
				const auto color_cur_vertex = color_per_vertex[u];
				const auto color_first_vertex = color_per_vertex[ oriented_vertices[i][0] ];
				levels_per_vertex[node_t{u}] = (color_cur_vertex == color_first_vertex ? d : -d);
			}

			// dump the vertices into 'inv_arr' and sort them later.
			if (levels_per_vertex[node_t{u}] > 0) {
				// std::cout << "    inv_arr[" << left << "]= " << u << '\n';
				inv_arr[left++] = u;
			}
			else {
				// std::cout << "    inv_arr[" << right << "]= " << u << '\n';
				inv_arr[right--] = u;
			}

			min_level_value = std::min(min_level_value, levels_per_vertex[node_t{u}]);
		}
	}
	inv_arr[left] = thistle;

#if defined DEBUG
	assert(left == right);
#endif

	levels_per_vertex[node_t{thistle}] = thistle_level;
	min_level_value = std::min(min_level_value, levels_per_vertex[node_t{thistle}]);

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "        Level per vertex:\n";
	for (node u = 0; u < n; ++u) {
		std::cout << "            level[" << u << "]= " << levels_per_vertex[node_t{u}] << '\n';
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

	// build the actual arrangement object now
	arr = linear_arrangement::from_inverse(inv_arr.begin(), inv_arr.end());

#if defined DEBUG
	// sum of edge lengths prior to adjustments
	const auto __D = linarr::sum_edge_lengths(t, arr);
#endif

#if defined PRINT_MESSAGES_1THISTLE
	{
	const auto dir = arr.direct_as_vector();
	const auto inv = arr.inverse_as_vector();
	std::cout << "        Before moving thistle:\n";
	std::cout << "        Direct: ";
	for (std::size_t i = 0; i < dir.size(); ++i) { std::cout << ' ' << dir[i]; }
	std::cout << '\n';
	std::cout << "        Inverse:";
	for (std::size_t i = 0; i < inv.size(); ++i) { std::cout << ' ' << inv[i]; }
	std::cout << '\n';
	}
#endif

#if defined DEBUG
	if (not linarr::is_arrangement(t, arr)) {
		// The arrangement is malformed. This should not happen.
		assert(false);
	}
#endif

	{
	// move the thistle to the left until the level sequence is right
	// while keeping 'thistle' an actual thistle vertex
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

#if defined PRINT_MESSAGES_1THISTLE
	{
	const auto dir = arr.direct_as_vector();
	const auto inv = arr.inverse_as_vector();
	std::cout << "        After moving thistle:\n";
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
	std::cout << "        __D= " << __D << '\n';
#endif

#if defined DEBUG
	assert(D >= __D);
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

	result_t<make_arrangement>& res
)
noexcept
{
	const auto deg_thistle = t.get_degree(thistle);

	data_array<char> side_of_thistle(deg_thistle, 0);
	data_array<node_set> oriented_verts(deg_thistle);

#if defined PRINT_MESSAGES_1THISTLE
	std::cout << "Thistle: " << thistle << '\n';
	int counter = 0;
#endif

#if defined DEBUG
	std::size_t num_combinations = 0;
#endif

	do {
#if defined PRINT_MESSAGES_1THISTLE
		std::cout << "    Binary: " << counter++ << '\n';
		std::cout << "    ";
		for (std::size_t j = 0; j < deg_thistle; ++j) {
			std::cout << int(side_of_root[j]) << ' ';
		}
		std::cout << '\n';
#endif

#if defined DEBUG
		++num_combinations;
#endif

		int64_t level_thistle = 0;
		for (std::size_t i = 0; i < deg_thistle; ++i) {
			if (side_of_thistle[i] == LEFT_SIDE) {
				// Neighbour of the thistle goes to the left half of the arrangement.
				--level_thistle;
			}
			else {
				// Neighbour of the thistle goes to the right half of the arrangement.
				++level_thistle;
			}
		}

		// ignore orientations where the root is not a thistle vertex
		if (level_thistle >= 0 and to_uint64(level_thistle) != deg_thistle) {

			// the sets of vertices in 'nodes_subtrees' are oriented so that
			// the neighbours of the thistle are always on the left half of the
			// arrangement
			for (std::size_t i = 0; i < deg_thistle; ++i) {
				oriented_verts[i] = nodes_subtrees[i];
				if (side_of_thistle[i] == LEFT_SIDE) {
					// Neighbour of the root goes to the
					// left half of the arrangement.
					// Nothing to do.
				}
				else {
					// Neighbour of the root goes to the
					// right half of the arrangement.
					// Reverse the arrangement.
					std::reverse(oriented_verts[i].begin(), oriented_verts[i].end());
				}
			}

#if defined PRINT_MESSAGES_1THISTLE
			std::cout << "        Level of thistle: " << level_thistle << '\n';
			std::cout << "        Oriented vertices:\n";
			for (std::size_t i = 0; i < deg_thistle; ++i) {
				std::cout
					<< "            "
					<< (side_of_root[i] == LEFT_SIDE ? "(L)" : "(R)")
					<< ' ' << i << ')';
				for (std::size_t j = 0; j < oriented_verts[i].size(); ++j) {
					std::cout << ' ' << oriented_verts[i][j];
				}
				std::cout << '\n';
			}
#endif

			// merge the arrangements and keep track of the maximum
			merge_arrangements<make_arrangement>(
				t,
				thistle, level_thistle, is_thistle_neighbor,
				color_per_vertex, side_of_thistle,
				oriented_verts,
				arr, inv_arr, level_per_vertex,
				res
			);
		}
#if defined PRINT_MESSAGES_1THISTLE
		else {
			std::cout << "        This subset does not make the thistle be a thistle\n";
		}
#endif

	}
	while (next_binary(side_of_thistle.begin(), side_of_thistle.end()));

#if defined DEBUG
	assert(num_combinations == 1ull << deg_thistle);
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
#if defined BUG
	std::cerr << "Warning! This function contains unfixed bugs. Sorry :(\n";
#endif

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

	linear_arrangement arr(n);
	data_array<node> inv_arr(n);
	level_signature_per_vertex level_per_vertex(n);

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

		// Sort the nodes in every connected component.
		for (std::size_t i = 0; i < nodes_subtrees.size(); ++i) {
			auto& nodes = nodes_subtrees[i];

			// Sort the vertices so that "blue" vertices are always
			// arranged on the left half of the arrangement and "red"
			// vertices arranged to the right half.
			std::sort(
				nodes.begin(), nodes.end(),
				// u < v?
				[&](node u, node v) {
					const auto du = t.get_degree(u);
					const auto dv = t.get_degree(v);
					// same color vertices sorted by degree
					if (c.get_color_of(u) == c.get_color_of(v)) {
						// if the color is blue, sort by non-decreasing degree.
						// if the color is red, sort by non-increasing degree.
						return (c.get_color_of(u) == detail::red ? du < dv : du > dv);
					}
					// when the colors are different, sort blue to the left
					return (c.get_color_of(u) == detail::blue ? true : false);
				}
			);

			// Prepare the bipartite arrangements so that the neighbours of
			// the chosen thistle vertex always lie on the left half of the
			// arrangement.

			if (c.get_color_of(thistle) == detail::blue) {
				// Neighbours are red, and are currently placed in the right half
				// of the set. These are to be placed in the left half.
				// We have to reverse the ordering.
				std::reverse(nodes.begin(), nodes.end());
			}
			else if (c.get_color_of(thistle) == detail::red) {
				// Neighbours are blue, and these are placed in the left half
				// of the arrangement. Nothing to do.
			}
		}

		// Find best orientation for this thistle.
		detail::choose_orientations_for_root<make_arrangement>(
			t, thistle, is_thistle_neighbor, nodes_subtrees, c,
			arr, inv_arr, level_per_vertex,
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
