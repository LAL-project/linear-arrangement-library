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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>

// lal includes
#include <lal/graphs/output.hpp>
#include <lal/basic_types.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/graphs/size_subtrees.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/pairs_utils.hpp>
#include <lal/detail/queue_array.hpp>
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

/// Is mode @e m equal to @ref lal::detail::centroid_results::only_one_centroidal.
inline constexpr bool is_m1(const centroid_results& m) noexcept {
	return m == centroid_results::only_one_centroidal;
}
/// Is mode @e m equal to @ref lal::detail::centroid_results::full_centroid.
inline constexpr bool is_m2(const centroid_results& m) noexcept {
	return m == centroid_results::full_centroid;
}
/// Is mode @e m equal to @ref lal::detail::centroid_results::full_centroid_plus_subtree_sizes.
inline constexpr bool is_m3(const centroid_results& m) noexcept {
	return m == centroid_results::full_centroid_plus_subtree_sizes;
}
/// Is mode @e m equal to @ref lal::detail::centroid_results::full_centroid_plus_edge_sizes.
inline constexpr bool is_m4(const centroid_results& m) noexcept {
	return m == centroid_results::full_centroid_plus_edge_sizes;
}

/**
 * @brief Calculates the centroid of a tree
 *
 * See @ref LAL_concepts__centre_centroid for details on what the centroid of a
 * tree is.
 *
 * If subtree sizes are to be returned, they come in an array of size the number
 * of vertices of the tree.
 *
 * @tparam mode Indicates the value to be returned by this function. If:
 * - mode == @ref lal::detail::centroid_results::only_one_centroidal, the result
 * of the function is a node.
 * - mode == @ref lal::detail::centroid_results::full_centroid, the result of the
 * function is a pair of (possibly) two nodes. The second node may have an invalid
 * value, indicating that the tree has only one centroidal vertex.
 * - mode == @ref lal::detail::centroid_results::full_centroid_plus_subtree_sizes,
 * the result of the function is a pair of (possibly) two nodes and the sizes of
 * all the subtrees with respect to the first centroidal node in the pair.
 * - mode == @ref lal::detail::centroid_results::full_centroid_plus_edge_sizes,
 * the result of the function is a pair of (possibly) two nodes and an array of
 * the form \f$(u,v, s(u,v))\f$ for all directed edges \f$(u,v)\f$ that point
 * away from the first centroidal node in the pair.
 * @tparam tree_t Type of tree.
 * @param t Input tree.
 * @param x Node belonging to a connected component whose centroid we want.
 */
template <
	centroid_results mode,
	class tree_t,
	std::enable_if_t<std::is_base_of_v<graphs::tree, tree_t>, bool> = true
>
[[nodiscard]]
conditional_list_t<
	bool_sequence<
		is_m1(mode),
		is_m2(mode),
		is_m3(mode),
		is_m4(mode)
	>,
	type_sequence<
		node,
		std::pair<node, node>,
		std::pair<std::pair<node, node>, array<uint64_t>>,
		std::pair<std::pair<node, node>, array<edge_size>>
	>
>
find_centroidal_vertex(const tree_t& t, const node x) noexcept
{
	const auto n = t.get_num_nodes();
	const auto size_cc_x = t.get_num_nodes_component(x);

	if (size_cc_x == 1) {
		if constexpr (is_m1(mode)) {
			return x;
		}
		else if constexpr (is_m2(mode)) {
			return {x,2};
		}
		else if constexpr (is_m3(mode)) {
			array<uint64_t> s(n, 0);
			s[x] = 1;
			return {{x,2}, std::move(s)};
		}
		else if constexpr (is_m4(mode)) {
			return {{x,2}, array<edge_size>{}};
		}
	}
	if (size_cc_x == 2) {
		auto only_neigh = [&]() {
			if constexpr (std::is_base_of_v<graphs::free_tree, tree_t>) {
				return t.get_neighbors(x)[0];
			}
			else {
				if (t.get_out_degree(x) == 0) { return t.get_in_neighbors(x)[0]; }
				else { return t.get_out_neighbors(x)[0]; }
			}
		}();

		const node u =                   (x < only_neigh ? x : only_neigh);
		const node v = is_m1(mode) ? 0 : (x < only_neigh ? only_neigh : x);

		if constexpr (is_m1(mode)) {
			return u;
		}
		else if constexpr (is_m2(mode)) {
			return {u, v};
		}
		else if constexpr (is_m3(mode)) {
			array<uint64_t> s(n, 0);
			s[u] = 2;
			s[v] = 1;
			return {{u, v}, std::move(s)};
		}
		else if constexpr (is_m4(mode)) {
			return {
				{u, v},
				array<edge_size>{ {{u, v}, 1} }
			};
		}
	}

	const auto ndiv2 = size_cc_x/2 + size_cc_x%2;

	// the centroidal vertices, initialized to invalid values
	node c1 = n + 1;
	node c2 = n + 1;

	// weight of every node: needed to detect the centroid.
	array<uint64_t> weight(n, 1);
	// degree of every vertex: needed to find leaves
	array<uint64_t> degree(n, 0);
	// array of pairs of edge and directional size
	array<edge_size> edge_sizes;
	std::size_t idx_edge_sizes = 0;
	if constexpr (is_m4(mode)) {
	edge_sizes.resize(size_cc_x - 1);
	}

	// queue of the traversal
	queue_array<node> queue;
	queue.init(size_cc_x);

	// push leaves of the connected component into the queue.
	if (size_cc_x < n/2) {
		BFS<tree_t> bfs(t);
		bfs.set_use_rev_edges(std::is_base_of_v<graphs::rooted_tree, tree_t>);
		bfs.set_process_current(
		[&](const auto&, const node u) {
			degree[u] = t.get_degree(u);
			// fill queue
			if (t.get_degree(u) == 1) {
				queue.push(u);
			}
		}
		);
		bfs.start_at(x);
	}
	else {
		for (node u = 0; u < n; ++u) {
			if (t.get_component_representative(u) == t.get_component_representative(x)) {
				degree[u] = t.get_degree(u);
				if (t.get_degree(u) == 1) {
					queue.push(u);
				}
			}
		}
	}

	// find centroid.
	while (queue.size() > 0) {
		// current node
		const node u = queue.pop();

		if (weight[u] >= ndiv2) {
			if (c1 >= n + 1) {
				// if the user requested just one centroidal vertex,
				// stop now, there is no need to go on.
				if constexpr (is_m1(mode)) { return u; }

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
			for (const node v : t.get_neighbors(u)) {
				if (degree[v] == 0) { continue; }

				--degree[v];
				weight[v] += weight[u];
				if (degree[v] == 1) {
					queue.push(v);
				}

				if constexpr (is_m4(mode)) {
					edge_sizes[idx_edge_sizes++] = {{v,u}, weight[u]};
				}
			}
		}
		else {
			for (const node v : t.get_in_neighbors(u)) {
				if (degree[v] == 0) { continue; }

				--degree[v];
				weight[v] += weight[u];
				if (degree[v] == 1) {
					queue.push(v);
				}

				if constexpr (is_m4(mode)) {
					edge_sizes[idx_edge_sizes++] = {{v,u}, weight[u]};
				}
			}
			for (const node v : t.get_out_neighbors(u)) {
				if (degree[v] == 0) { continue; }

				--degree[v];
				weight[v] += weight[u];
				if (degree[v] == 1) {
					queue.push(v);
				}

				if constexpr (is_m4(mode)) {
					edge_sizes[idx_edge_sizes++] = {{v,u}, weight[u]};
				}
			}
		}
	}

	if (c2 < n) {
		if (c1 > c2) {
			std::swap(c1, c2);
		}
		weight[c1] += weight[c2];

		if constexpr (is_m4(mode)) {
			edge_sizes[idx_edge_sizes++] = {{c1,c2}, weight[c2]};
		}
	}

#if defined DEBUG
	if constexpr (is_m4(mode)) {
		assert(idx_edge_sizes == edge_sizes.size());
	}
#endif

	if constexpr (is_m2(mode)) {
		return {c1, c2};
	}
	else if constexpr (is_m3(mode)) {
		return {{c1, c2}, std::move(weight)};
	}
	else if constexpr (is_m4(mode)) {
		return {{c1, c2}, std::move(edge_sizes)};
	}
}

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
[[nodiscard]] std::pair<node,node> centroidal_vertex_plus_adjacency_list
(
	const tree_t& t,
	const node x,
	std::vector< std::vector<node_size> >& L
)
noexcept
{
	// retrieve centroid and set of edges and directional size
	std::pair< std::pair<node,node>, array<edge_size> >
		centroid_subtree_sizes =
		find_centroidal_vertex<centroid_results::full_centroid_plus_edge_sizes>(t, x);

	const uint64_t n = t.get_num_nodes();

	// sort the edges by directional size
	sorting::counting_sort
		<edge_size, sorting::sort_type::non_increasing>
		(
			centroid_subtree_sizes.second.begin(),
			centroid_subtree_sizes.second.end(),
			n,
			centroid_subtree_sizes.second.size(),
			[](const edge_size& edge_pair) -> std::size_t
			{ return edge_pair.size; }
		);

	// fill (already-rooted) adjacency matrix
	L.resize(n);
	for (const auto& [uv, suv] : centroid_subtree_sizes.second) {
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
[[nodiscard]] std::pair<node, node> retrieve_centroid
(const tree_t& t, const node x = 0)
noexcept
{
	return find_centroidal_vertex<centroid_results::full_centroid>(t, x);
}

} // -- namespace detail
} // -- namespace lal
