/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
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

#pragma once

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <iostream>
#include <vector>

// lal includes
#include <lal/graphs/output.hpp>
#include <lal/basic_types.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/graphs/size_subtrees.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/pairs_utils.hpp>
#include <lal/detail/linear_queue.hpp>
#include <lal/detail/graphs/traversal.hpp>
#include <lal/detail/type_traits/conditional_list.hpp>

namespace lal {
namespace detail {

/// The different types of results
enum class centroid_results {
	// 1
	/// Returns only one centroidal vertex. No weights.
	only_one_centroidal,
	// 2
	/// Returns the full centroid of the tree. No weights.
	full_centroid,
	// 3
	/// Returns the full centroid of the tree. Also returns the weights.
	full_centroid_plus_subtree_sizes,
	// 4
	/// Returns the full centroid of the tree. Also returns the edge_size array.
	full_centroid_plus_edge_sizes
};

#define m1(mode) (mode == centroid_results::only_one_centroidal)
#define m2(mode) (mode == centroid_results::full_centroid)
#define m3(mode) (mode == centroid_results::full_centroid_plus_subtree_sizes)
#define m4(mode) (mode == centroid_results::full_centroid_plus_edge_sizes)
#define node_pair std::pair<lal::node,lal::node>
#define P std::pair

/**
 * @brief Calculates the centroid of a tree
 *
 * If subtree sizes are to be returned, they come in an array of size the number
 * of vertices of the tree.
 *
 * @tparam mode Indicates the value to be returned by this function. If:
 * - mode == @ref centroid_results::only_one_centroidal, the result of the function
 * is a node.
 * - mode == @ref centroid_results::full_centroid, the result of the function
 * is a pair of (possibly) two nodes. The second node may have an invalid value,
 * indicating that the tree has only one centroidal vertex.
 * - mode == @ref centroid_results::full_centroid_plus_subtree_sizes, the result
 * of the function is a pair of (possibly) two nodes and the sizes of all the
 * subtrees with respect to the first centroidal node in the pair.
 * - mode == @ref centroid_results::full_centroid_plus_edge_sizes, the result
 * of the function is a pair of (possibly) two nodes and an array of the form
 * \f$(u,v, s(u,v))\f$ for all directed edges \f$(u,v)\f$ that point away from
 * the first centroidal node in the pair.
 * @tparam tree_t Type of tree.
 * @param t Input tree.
 * @param x Node belonging to a connected component whose centroid we want.
 */
template <
	centroid_results mode,
	class tree_t,
	std::enable_if_t<std::is_base_of_v<graphs::tree, tree_t>, bool> = true
>
conditional_list_t<
	bool_sequence<
		m1(mode),
		m2(mode),
		m3(mode),
		m4(mode)
	>,
	type_sequence<
		node,
		std::pair<lal::node,lal::node>,
		std::pair<std::pair<lal::node,lal::node>, data_array<uint64_t>>,
		std::pair<std::pair<lal::node,lal::node>, data_array<edge_size>>
	>
>
find_centroidal_vertex(const tree_t& t, node x) noexcept
{
	const auto N = t.get_num_nodes();
	const auto n = t.get_num_nodes_component(x);

	if (n == 1) {
		if constexpr (m1(mode)) {
			return x;
		}
		else if constexpr (m2(mode)) {
			return {x,2};
		}
		else if constexpr (m3(mode)) {
			data_array<uint64_t> s(N, 0);
			s[x] = 1;
			return {{x,2}, std::move(s)};
		}
		else if constexpr (m4(mode)) {
			return {{x,2}, data_array<edge_size>{}};
		}
	}
	if (n == 2) {
		auto only_neigh = [&]() {
			if constexpr (std::is_base_of_v<graphs::free_tree, tree_t>) {
				return t.get_neighbours(x)[0];
			}
			else {
				if (t.get_out_degree(x) == 0) { return t.get_in_neighbours(x)[0]; }
				else { return t.get_out_neighbours(x)[0]; }
			}
		}();

		if (x > only_neigh) { std::swap(x, only_neigh); }
		if constexpr (m1(mode)) {
			return x;
		}
		else if constexpr (m2(mode)) {
			return {x,only_neigh};
		}
		else if constexpr (m3(mode)) {
			data_array<uint64_t> s(N, 0);
			s[x] = 2;
			s[only_neigh] = 1;
			return {{x,only_neigh}, std::move(s)};
		}
		else if constexpr (m4(mode)) {
			return {
				{x,only_neigh},
				data_array<edge_size>{ {{x, only_neigh}, 1} }
			};
		}
	}

	const auto ndiv2 = n/2 + n%2;

	// the centroidal vertices, initialized to invalid values
	node c1 = N + 1;
	node c2 = N + 1;

	// weight of every node: needed to detect the centroid.
	data_array<uint64_t> weight(N, 1);
	// degree of every vertex: needed to find leaves
	data_array<uint64_t> degree(N, 0);
	// array of pairs of edge and directional size
	data_array<edge_size> edge_sizes;
	std::size_t idx_edge_sizes = 0;
	if constexpr (m4(mode)) {
	edge_sizes.resize(n - 1);
	}

	// queue of the traversal
	linear_queue<node> queue;
	queue.init(n);

	// push leaves of the connected component into the queue.
	{
	BFS<tree_t> bfs(t);
	bfs.set_use_rev_edges(std::is_base_of_v<graphs::rooted_tree, tree_t>);
	bfs.set_process_current(
	[&](const auto&, node u) {
		degree[u] = t.get_degree(u);
		// fill queue
		if (t.get_degree(u) == 1) {
			queue.push(u);
		}
	}
	);
	bfs.start_at(x);
	}

	// find centroid.
	while (queue.size() > 0) {
		// current node
		const node u = queue.pop();

		if (weight[u] >= ndiv2) {
			if (c1 >= n) {
				// if the user requested just one centroidal vertex,
				// stop now, there is no need to go on.
				if constexpr (m1(mode)) { return u; }

				c1 = u;
			}
			else {
				c2 = u;
			}
			continue;
		}

		// "delete" vertex u
		--degree[u];
#if defined DEBUG
		assert(degree[u] == 0);
#endif

		// append a new leaf to the queue
		if constexpr (std::is_base_of_v<graphs::free_tree, tree_t>) {
			for (node v : t.get_neighbours(u)) {
				if (degree[v] == 0) { continue; }

				--degree[v];
				weight[v] += weight[u];
				if (degree[v] == 1) {
					queue.push(v);
				}

				if constexpr (m4(mode)) {
					edge_sizes[idx_edge_sizes++] = {{v,u}, weight[u]};
				}
			}
		}
		else {
			for (node v : t.get_in_neighbours(u)) {
				if (degree[v] == 0) { continue; }

				--degree[v];
				weight[v] += weight[u];
				if (degree[v] == 1) {
					queue.push(v);
				}

				if constexpr (m4(mode)) {
					edge_sizes[idx_edge_sizes++] = {{v,u}, weight[u]};
				}
			}
			for (node v : t.get_out_neighbours(u)) {
				if (degree[v] == 0) { continue; }

				--degree[v];
				weight[v] += weight[u];
				if (degree[v] == 1) {
					queue.push(v);
				}

				if constexpr (m4(mode)) {
					edge_sizes[idx_edge_sizes++] = {{v,u}, weight[u]};
				}
			}
		}
	}

	if (c2 < N) {
		if (c1 > c2) {
			std::swap(c1, c2);
		}
		weight[c1] += weight[c2];

		if constexpr (m4(mode)) {
			edge_sizes[idx_edge_sizes++] = {{c1,c2}, weight[c2]};
		}
	}

#if defined DEBUG
	if constexpr (m4(mode)) {
		assert(idx_edge_sizes == edge_sizes.size());
	}
#endif

	if constexpr (m2(mode)) {
		return {c1, c2};
	}
	else if constexpr (m3(mode)) {
		return {{c1, c2}, std::move(weight)};
	}
	else if constexpr (m4(mode)) {
		return {{c1, c2}, std::move(edge_sizes)};
	}
}

#undef P
#undef node_pair
#undef m4
#undef m3
#undef m2
#undef m1

/**
 * @brief Calculates the centroid and the corresponding rooted adjacency list
 *
 * @tparam t Treer type.
 * @param t Input tree.
 * @param x Node belonging to a connected component whose centroid we want.
 * @param[out] L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, s_u(v))\f$ where \f$v\f$ is a neighbour (with respect to a
 * fictional root taken to be a centroidal vertex of the tree) of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 */
template <
	class tree_t,
	std::enable_if_t<std::is_base_of_v<graphs::tree, tree_t>, bool> = true
>
std::pair<node,node> centroidal_vertex_plus_adjacency_list(
	const tree_t& t, node x,
	std::vector< std::vector<node_size> >& L
)
noexcept
{
	// retrieve centroid and set of edges and directional size
	std::pair< std::pair<node,node>, data_array<edge_size> >
		centroid_subtree_sizes =
		find_centroidal_vertex<centroid_results::full_centroid_plus_edge_sizes>(t, x);

	auto sizes_edge = std::move(centroid_subtree_sizes.second);

	const uint64_t n = t.get_num_nodes();

	// sort the edges by directional size
	detail::sorting::counting_sort
		<edge_size, sorting::non_increasing_t>
		(
			sizes_edge.begin(), sizes_edge.end(), n, sizes_edge.size(),
			[](const edge_size& edge_pair) -> std::size_t
			{ return edge_pair.size; }
		);

	// fill (already-rooted) adjacency matrix
	L.resize(n);
	for (const auto& [uv, suv] : sizes_edge) {
		const auto [u, v] = uv;
		L[u].push_back({v, suv});
	}

	return centroid_subtree_sizes.first;
}

// -----------------------------------------------------------------------------

/**
 * @brief Calculate the centroid of the connected component that has node @e x.
 *
 * For details on the parameters and return value see documentation of the
 * function above.
 * @tparam tree_t Type of tree.
 * @param t Input tree
 * @param x Node belonging to a connected component whose centroid we want.
 * @returns A tuple of two values: the nodes in the centroid. If the
 * tree has a single centroidal node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 */
template <
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool > = true
>
std::pair<node, node> retrieve_centroid(const tree_t& t, const node x)
noexcept
{
	return find_centroidal_vertex<centroid_results::full_centroid>(t, x);
}

/**
 * @brief Calculate the centroid of the tree @e t.
 *
 * See page @ref LAL_concepts__centre_centroid for a definition of centre and
 * centroid.
 *
 * @tparam tree_t Type of the input tree.
 * @param t Input tree.
 * @returns A tuple of two values: the nodes in the centroid. If the
 * tree has a single centroidal node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 */
template <
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool > = true
>
std::pair<node, node> retrieve_centroid(const tree_t& t)
noexcept
{
	return find_centroidal_vertex<centroid_results::full_centroid>(t, 0);
}

} // -- namespace detail
} // -- namespace lal
