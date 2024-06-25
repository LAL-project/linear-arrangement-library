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

// lal includes
#include <lal/basic_types.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/detail/graphs/traversal.hpp>

namespace lal {
namespace detail {

/**
 * @brief Update the Union-Find data structure after the addition of an edge.
 *
 * This function updates the Union-Find data structure of a tree after the
 * addition of an edge between vertices @e u and @e v.
 * @tparam tree_t Type of input tree. A derived class of @ref lal::graphs::free_tree
 * or a derived class of @ref lal::graphs::rooted_tree.
 * @param t Input tree
 * @param u Node
 * @param v Node
 * @param root_of Pointer to an array where @e root_of[@e s] = @e t if the root
 * of the connected component of @e s is @e t
 * @param root_size Sizes of each connected component.
 * @pre The edge \f$\{u,v\}\f$ must exist.
 */
template <class tree_t>
void update_unionfind_after_add_edge
(
	const tree_t& t,
	const node u, const node v,
	node * const root_of,
	uint64_t * const root_size
)
noexcept
{
	static_assert(
		std::is_base_of_v<graphs::free_tree, tree_t> or
		std::is_base_of_v<graphs::rooted_tree, tree_t>
	);

	// 'u' and 'v' are not connected, so they belong to
	// (different) connected components of the tree.

	// 'parent' and 'child' determine a direction to be used later.
	// 'new_root' is the new root for one of the vertices
	node parent, child, new_root;

	const node root_u = root_of[u];
	const node root_v = root_of[v];

	const uint64_t size_u = root_size[root_u];
	const uint64_t size_v = root_size[root_v];
	const uint64_t new_size = size_u + size_v;

	if (size_u < size_v) {
		root_of[root_u] = root_v;
		root_of[u] = root_v;

		new_root = root_v;
		root_size[new_root] = new_size;

		// update roots in the direction of v -> u
		parent = v; child = u;
	}
	else {
		root_of[root_v] = root_u;
		root_of[v] = root_u;

		new_root = root_u;
		root_size[new_root] = new_size;

		// update roots in the direction of u -> v
		parent = u; child = v;
	}

	// update roots of the smaller component,
	// in the direction parent -> child
	BFS<tree_t> bfs(t);
	bfs.set_use_rev_edges( BFS<tree_t>::is_graph_directed );
	bfs.set_process_current(
	[&](const BFS<tree_t>&, node w) -> void { root_of[w] = new_root; }
	);
	bfs.set_visited(parent, 1); // avoid going backwards
	bfs.start_at(child);
}

/**
 * @brief Update the Union-Find data structure after the addition of several edges.
 * @tparam tree_t Type of tree
 * @param t Input tree
 * @param edges Edges added to the tree
 * @param root_of Pointer to an array where @e root_of[@e s] = @e t if the root
 * of the connected component of @e s is @e t
 * @param root_size Sizes of each connected component.
 * @pre The edge \f$\{u,v\}\f$ must exist.
 */
template <class tree_t>
void update_unionfind_after_add_edges
(
	const tree_t& t,
	const edge_list& edges,
	node * const root_of,
	uint64_t * const root_size
)
noexcept
{
	static_assert(
		std::is_base_of_v<graphs::free_tree, tree_t> or
		std::is_base_of_v<graphs::rooted_tree, tree_t>
	);

	const auto n = t.get_num_nodes();

	// These two variables are used by the BFS object, but are initialized
	// in the for loop before 'start_at' is called.
	uint64_t size_current_root = n + 1;
	node current_root = n + 1;

	BFS<tree_t> bfs(t);
	bfs.set_use_rev_edges( BFS<tree_t>::is_graph_directed );
	bfs.set_process_current(
	[&](const auto&, node w) {
		root_of[w] = current_root;
		++size_current_root;
	}
	);

	for (const auto& [u, v] : edges) {
		if (bfs.node_was_visited(u)) { continue; }

		current_root = u;
		size_current_root = 0;
		bfs.start_at(u);
		root_size[current_root] = size_current_root;
	}
}

/**
 * @brief Update the Union-Find data structure after several edges have been
 * operated in bulk.
 * @tparam tree_t Type of tree
 * @param t Input tree
 * @param root_of Pointer to an array where @e root_of[@e s] = @e t if the root
 * of the connected component of @e s is @e t
 * @param root_size Sizes of each connected component.
 * @pre The edge \f$\{u,v\}\f$ must exist.
 */
template <class tree_t>
void update_unionfind_after_add_rem_edges_bulk
(
	const tree_t& t,
	node * const root_of,
	uint64_t * const root_size
)
noexcept
{
	static_assert(
		std::is_base_of_v<graphs::free_tree, tree_t> or
		std::is_base_of_v<graphs::rooted_tree, tree_t>
	);

	const auto n = t.get_num_nodes();

	// These two variables are used by the BFS object, but are initialized
	// in the for loop before 'start_at' is called.
	uint64_t size_current_root = n + 1;
	node current_root = n + 1;

	BFS<tree_t> bfs(t);
	bfs.set_use_rev_edges( BFS<tree_t>::is_graph_directed );
	bfs.set_process_current(
	[&](const auto&, node w) {
		root_of[w] = current_root;
		++size_current_root;
	}
	);

	for (node u = 0; u < n; ++u) {
		if (bfs.node_was_visited(u)) { continue; }

		current_root = u;
		size_current_root = 0;
		bfs.start_at(u);
		root_size[current_root] = size_current_root;
	}
}

/**
 * @brief Updates Union-Find after the removal of an edge.
 *
 * This function updates the Union-Find data structure of a tree after the
 * removal of the edge between vertices @e u and @e v.
 * @tparam tree_t Type of input tree. A derived class of @ref lal::graphs::free_tree
 * or a derived class of @ref lal::graphs::rooted_tree.
 * @param t Input tree
 * @param u Node
 * @param v Node
 * @param root_of Pointer to an array where @e root_of[@e s] = @e t if the root
 * of the connected component of @e s is @e t
 * @param root_size Sizes of each connected component.
 * @pre The edge \f$\{u,v\}\f$ must exist.
 */
template <class tree_t>
void update_unionfind_after_remove_edge
(
	const tree_t& t,
	const node u, const node v,
	node * const root_of,
	uint64_t * const root_size
)
noexcept
{
	static_assert(
		std::is_base_of_v<graphs::free_tree, tree_t> or
		std::is_base_of_v<graphs::rooted_tree, tree_t>
		);

// 'u' and 'v' are connected
#if defined DEBUG
	assert(root_of[u] == root_of[v]);
#endif

	const uint64_t size_uv = root_size[root_of[u]];

	BFS<tree_t> bfs(t);

	// --- update u's info ---

	// Update the root of the vertices reachable from 'u'.
	//   (also calculate the size of u's component)
	uint64_t size_cc_u = 0;
	bfs.set_use_rev_edges( BFS<tree_t>::is_graph_directed );
	bfs.set_process_current(
		[&](const auto&, node w) -> void {
			root_of[w] = u;
			++size_cc_u;
		}
		);
	bfs.start_at(u);
	root_of[u] = u;
	root_size[u] = size_cc_u;

	// --- update v's info ---

	// Update the root of the vertices reachable from 'v'.
	//   (there is no need to reset the BFS object)
	bfs.set_process_current(
		[&](const detail::BFS<tree_t>&, node w) -> void {
			root_of[w] = v;
		}
		);
	bfs.start_at(v);
	root_of[v] = v;
	root_size[v] = size_uv - size_cc_u;
}

/**
 * @brief Update the Union-Find data structure after the addition of several edges.
 * @tparam tree_t Type of input tree. A derived class of @ref lal::graphs::free_tree
 * or a derived class of @ref lal::graphs::rooted_tree.
 * @param t Input tree
 * @param edges Edges added to the tree
 * @param root_of Pointer to an array where @e root_of[@e s] = @e t if the root
 * of the connected component of @e s is @e t
 * @param root_size Sizes of each connected component.
 * @pre The edge \f$\{u,v\}\f$ must exist.
 */
template <class tree_t>
void update_unionfind_after_remove_edges
(
	const tree_t& t,
	[[maybe_unused]] const edge_list& edges,
	node * const root_of,
	uint64_t * const root_size
)
noexcept
{
	static_assert(
		std::is_base_of_v<graphs::free_tree, tree_t> or
		std::is_base_of_v<graphs::rooted_tree, tree_t>
	);

	const auto n = t.get_num_nodes();

	// These two variables are used by the BFS object, but are initialized
	// in the for loop before 'start_at' is called.
	uint64_t size_current_cc = n + 1;
	node current_root = n + 1;

	BFS<tree_t> bfs(t);
	bfs.set_use_rev_edges( BFS<tree_t>::is_graph_directed );
	bfs.set_process_current(
	[&](const auto&, node w) {
		root_of[w] = current_root;
		++size_current_cc;
	}
	);

	for (const auto& [u, v] : edges) {
		if (not bfs.node_was_visited(u)) {
			current_root = u;
			size_current_cc = 0;
			bfs.start_at(u);
			root_size[u] = size_current_cc;
		}

		if (not bfs.node_was_visited(v)) {
			current_root = v;
			size_current_cc = 0;
			bfs.start_at(v);
			root_size[v] = size_current_cc;
		}
	}
}

// -----------------------------------------------------------------------------

/**
 * @brief Update Union-Find after the removal of a vertex.
 *
 * This function updates the Union-Find data structure of a tree prior to the
 * removal of the edge (u,v).
 *
 * This function is called by the function
 *		lal::detail::update_unionfind_before_remove_edges_incident_to
 *
 * In particular, it updates the information associated to the vertices found
 * in the direction (u,v).
 * @tparam tree_t Type of input tree. A derived class of @ref lal::graphs::free_tree
 * or a derived class of @ref lal::graphs::rooted_tree.
 * @param bfs Breadth-First Search object.
 * @param v Node to be removed.
 * @param root_of Pointer to an array where @e root_of[@e s] = @e t if the root
 * of the connected component of @e s is @e t
 * @param root_size Sizes of each connected component.
 */
template <class tree_t>
void update_unionfind_before_remove_edges_incident_to
(
	BFS<tree_t>& bfs,
	const node v,
	node * const root_of,
	uint64_t * const root_size
)
noexcept
{
	static_assert(
		std::is_base_of_v<graphs::free_tree, tree_t> or
		std::is_base_of_v<graphs::rooted_tree, tree_t>
	);

	uint64_t size_cc_v = 0;
	bfs.set_process_current(
	[&](const auto&, node w) -> void {
		root_of[w] = v;
		++size_cc_v;
	}
	);
	bfs.start_at(v);

	root_of[v] = v;
	root_size[v] = size_cc_v;
}

/**
 * @brief Update Union-Find after a vertex removal.
 *
 * This function updates the Union-Find data structure of a tree prior to the
 * removal of the edge (u,v).
 *
 * In particular, it updates the information associated to the vertices found
 * in the direction (u,v).
 * @tparam tree_t Type of input tree. A derived class of @ref lal::graphs::free_tree
 * or a derived class of @ref lal::graphs::rooted_tree.
 * @param t Input tree.
 * @param u Node to be removed.
 * @param root_of Pointer to an array where @e root_of[@e s] = @e t if the root
 * of the connected component of @e s is @e t
 * @param root_size Sizes of each connected component.
 */
template <typename tree_t>
void update_unionfind_before_remove_edges_incident_to
(
	const tree_t& t,
	const node u,
	node * const root_of,
	uint64_t * const root_size
)
noexcept
{
	static_assert(
		std::is_base_of_v<graphs::free_tree, tree_t> or
		std::is_base_of_v<graphs::rooted_tree, tree_t>
	);

	BFS<tree_t> bfs(t);
	bfs.set_use_rev_edges( BFS<tree_t>::is_graph_directed );
	// avoid going 'backwards', we need to go 'onwards'
	bfs.set_visited(u, 1);

	if constexpr (std::is_base_of_v<graphs::free_tree, tree_t>) {
		for (node v : t.get_neighbors(u)) {
			// update size and root of the edges from v onwards
			// (onwards means "in the direction u -> v"
			update_unionfind_before_remove_edges_incident_to
				(bfs, v, root_of, root_size);
		}
	}
	else {
		for (node v : t.get_in_neighbors(u)) {
			// update size and root of the edges from v onwards
			// (onwards means "in the direction u -> v"
			update_unionfind_before_remove_edges_incident_to
				(bfs, v, root_of, root_size);
		}
		for (node v : t.get_out_neighbors(u)) {
			// update size and root of the edges from v onwards
			// (onwards means "in the direction u -> v"
			update_unionfind_before_remove_edges_incident_to
				(bfs, v, root_of, root_size);
		}
	}

	root_of[u] = u;
	root_size[u] = 1;
}

} // -- namespace detail
} // -- namespace lal
