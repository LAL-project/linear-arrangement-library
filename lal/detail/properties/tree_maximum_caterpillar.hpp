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

// C++ includes
#include <tuple>

// lal includes
#include <lal/graphs/tree_type.hpp>
#include <lal/graphs/tree.hpp>
#include <lal/detail/graphs/traversal.hpp>
#include <lal/detail/data_array.hpp>
#include <lal/detail/linear_queue.hpp>
#include <lal/detail/type_traits/conditional_list.hpp>

namespace lal {
namespace detail {
namespace maximum_subtrees {
namespace caterpillar {

/// What is to be calculated when finding the maximum spanning caterpillar.
enum result {
	/// Only the caterpillar distance.
	distance,
	/**
	 * @brief Caterpillar distance plus caterpillar structure's nodes.
	 *
	 * The only vector represents the set of nodes that belong to the caterpillar
	 * in the form of a bitset.
	 */
	distance_vertices,
	/**
	 * @brief Caterpillar distance plus the caterpillar's structure.
	 *
	 * The second vector contains the caterpillar backbone in the form of an
	 * "ordered" path. It is guaranteed that the path starts at the first
	 * vertex and finishes at the last vertex of the second vector.
	 *
	 * The third vector represents the set of nodes that belong to the caterpillar
	 * in the form of a bitset.
	 */
	distance_structure
};

/**
 * @brief Find the farthest vertex from @e start_at in the tree.
 *
 * Distance is defined as the number of vertices in the path plus the number
 * of vertices neighbouring the path.
 * @tparam tree_t Type of tree.
 * @param t Input tree.
 * @param start_at Starting vertex.
 * @param bfs Traversal object.
 * @param num_vertices_in_path Function memory.
 * @param weight Function memory.
 * @returns The farthest vertex from @e start_at
 */
template <class tree_t>
node find_farthest_vertex(
	const tree_t& t,
	node start_at,
	BFS<tree_t>& bfs,
	data_array<uint64_t>& num_vertices_in_path,
	data_array<uint64_t>& weight
)
noexcept
{
	const auto n = t.get_num_nodes();

	bfs.reset();
	bfs.set_use_rev_edges( BFS<tree_t>::is_graph_directed );

	num_vertices_in_path.fill(0);
	num_vertices_in_path[start_at] = 1;
	uint64_t max_num_vertices = 0;

	node farthest = n + 1;

	bfs.set_process_neighbour(
	[&](const auto&, node u, node v, bool) {
		num_vertices_in_path[v] = num_vertices_in_path[u] + weight[u] + 1;
		if (max_num_vertices < num_vertices_in_path[v]) {
			max_num_vertices = num_vertices_in_path[v];
			farthest = v;
		}
	}
	);
	bfs.start_at(start_at);

	return farthest;
}

/**
 * @brief Calculate the maximum spanning caterpillar of a tree.
 * @tparam ret_type Specify the return type.
 * @tparam tree_t Tree type
 * @param t Input tree
 * @returns If:
 * - @e ret_type is @ref result::distance, returns an integer value,
 * - @e ret_type is @ref result::distance_vertices, returns an integer value and a
 * vector of char indicating what vertices are in the caterpillar tree,
 * - @e ret_type is @ref result::distance_structure, returns an integer value, a
 * vector of nodes listing all nodes from one end to other of the maximum
 * caterpillar's backbone, and a vector of char indicating what vertices are in
 * the caterpillar tree,
 */
template <
	result ret_type,
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool > = true
>
conditional_list_t<
	bool_sequence<
		ret_type == result::distance,
		ret_type == result::distance_vertices,
		ret_type == result::distance_structure
	>,
	type_sequence<
		uint64_t,
		std::tuple<uint64_t, std::vector<char>>,
		std::tuple<uint64_t, std::vector<node>, std::vector<char>>
	>
>
max_subtree(const tree_t& t) noexcept
{
	typedef std::vector<node> path;

	const auto n = t.get_num_nodes();

	// the easiest case: we know the tree is a caterpillar
	if (t.is_tree_type_valid() and t.is_of_tree_type(graphs::tree_type::caterpillar)) {
		if constexpr (ret_type == result::distance) {
			return 0;
		}
		else if constexpr (ret_type == result::distance_vertices) {
			return {0, std::vector<char>(n, 1)};
		}

		// if the value to return contains the caterpillar's backbone, then
		// we basically need to repeat the code below.
	}

	// more difficult case: we have no clue what the input tree is.

	if (n == 1) {
		if constexpr (ret_type == result::distance) {
			return 0;
		}
		else if constexpr (ret_type == result::distance_vertices) {
			return {0, {1}};
		}
		else {
			return {0, {0}, {1}};
		}
	}
	if (n == 2) {
		if constexpr (ret_type == result::distance) {
			return 0;
		}
		else if constexpr (ret_type == result::distance_vertices) {
			return {0, {1,1}};
		}
		else {
			return {0, {0,1}, {1,1}};
		}
	}

	// -------------------------------------------------------------------------
	// initialize data

	data_array<uint64_t> weight(n);
	for (node u = 0; u < n; ++u) {
		weight[u] = (t.get_degree(u) == 1 ? 0 : t.get_degree(u) - 2);
	}

	// the traversal object
	BFS bfs(t);

	// distance to every vertex from source
	data_array<uint64_t> num_vertices_in_path(n, 0);

	// -------------------------------------------------------------------------
	// do the two traversals

	// v_star: farthest from source
	// w_star: farthest from v_star

	// traverse the tree and find the first 'farthest' vertex (v_star)
	const node v_star = find_farthest_vertex(t, 0, bfs, num_vertices_in_path, weight);

	// traverse the tree again and find the second 'farthest' vertex (w_star)
	const node w_star = find_farthest_vertex(t, v_star, bfs, num_vertices_in_path, weight);

	// calculate the caterpillar distance
	const auto caterpillar_distance = n - num_vertices_in_path[w_star];

	// The result is just the caterpillar distance.
	// Regardless of the type of input tree, return 'caterpillar_distance'
	if constexpr (ret_type == result::distance) {
		return caterpillar_distance;
	}

	// We detected that the input tree is a caterpillar tree.
	// In case the caterpillar distance is 0, the return result
	// may be easy to calculate.
	if (caterpillar_distance == 0) {
		if constexpr (ret_type == result::distance_vertices) {
			return {caterpillar_distance, std::vector<char>(n,1)};
		}

		// Unfortunately, when (ret_type == max_caterpillar_result::distance_structure)
		// we have to compute the backbone
	}

	/* We have to do all of this even if
	 *
	 * (ret_type == max_caterpillar_result::distance_vertices)
	 */

	// sequence of vertices that define the caterpillar's backbone
	path maximum_caterpillar_backbone;

	// the set of nodes in the maximum spanning caterpillar
	std::vector<char> is_node_in_maximum_caterpillar(n, 0);

	// path to the current vertex in the traversal
	path path_to_current;

	// initialize the queue
	linear_queue<path> path_queue;
	path_queue.init(n);
	path_queue.push({w_star});

	// set up the BFS
	bfs.reset();
	bfs.set_use_rev_edges( BFS<tree_t>::is_graph_directed );

	bfs.set_process_current(
	[&](const auto&, node) {
		path_to_current = path_queue.pop();
	});

	bfs.set_terminate(
	[&](const auto&, node u) {
		if (u == v_star) {
			maximum_caterpillar_backbone = std::move(path_to_current);
		}
		return u == v_star;
	});

	bfs.set_process_neighbour(
	[&](const auto&, node, node v, bool) {
		auto path_to_v = path_to_current;
		path_to_v.push_back(v);
		path_queue.push(std::move(path_to_v));
	});

	// retrieve the backbone
	bfs.start_at(w_star);

	// find all vertices in the caterpillar by traversing the backbone
	for (node u : maximum_caterpillar_backbone) {
		is_node_in_maximum_caterpillar[u] = 1;
		if constexpr (not BFS<tree_t>::is_graph_directed) {
			for (node v : t.get_neighbours(u)) {
				is_node_in_maximum_caterpillar[v] = 1;
			}
		}
		else {
			for (node v : t.get_in_neighbours(u)) {
				is_node_in_maximum_caterpillar[v] = 1;
			}
			for (node v : t.get_out_neighbours(u)) {
				is_node_in_maximum_caterpillar[v] = 1;
			}
		}
	}

	if constexpr (ret_type == result::distance_vertices) {
		return {
			caterpillar_distance,
			std::move(is_node_in_maximum_caterpillar)
		};
	}
	else if constexpr (ret_type == result::distance_structure) {
		return {
			caterpillar_distance,
			std::move(maximum_caterpillar_backbone),
			std::move(is_node_in_maximum_caterpillar)
		};
	}
	else {
		// never reached;
#if defined DEBUG
		assert(false);
#endif
		return 0;
	}
}

} // -- namespace caterpillar
} // -- namespace maximum_subtrees
} // -- namespace detail
} // -- namespace lal
