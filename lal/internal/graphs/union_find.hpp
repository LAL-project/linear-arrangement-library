/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
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

#pragma once

// C++ includes
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/internal/graphs/traversal.hpp>

namespace lal {
namespace internal {

/* This function updates the union-find data structure of a tree after the
 * addition of the edge between the edges 'u' and 'v'.
 */
template<typename T>
inline
void UnionFind_update_roots_after_add
(
	const T& t, const node u, const node v,
	node * const root_of,
	uint64_t * const root_size
)
noexcept
{
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
	BFS<T> bfs(t);
	bfs.set_use_rev_edges(t.is_directed());
	bfs.set_process_current(
	[&](const BFS<T>&, node w) -> void { root_of[w] = new_root; }
	);
	bfs.set_visited(parent, 1); // avoid going backwards
	bfs.start_at(child);
}

/* This function updates the union-find data structure of a tree after the
 * removal of the edge between the edges 'u' and 'v'.
 */
template<typename T>
inline
void UnionFind_update_roots_after_remove
(
	const T& t, const node u, const node v,
	node * const root_of,
	uint64_t * const root_size
)
noexcept
{
	// 'u' and 'v' are connected
#if defined DEBUG
	assert(root_of[u] == root_of[v]);
#endif

	const uint64_t size_uv = root_size[root_of[u]];

	BFS<T> bfs(t);

	// --- update u's info ---

	// Update the root of the vertices reachable from 'u'.
	//   (also calculate the size of u's component)
	uint64_t size_u = 0;
	bfs.set_use_rev_edges(t.is_directed());
	bfs.set_process_current(
	[&](const auto&, node w) -> void { root_of[w] = u; ++size_u; }
	);
	bfs.start_at(u);
	root_of[u] = u;
	root_size[u] = size_u;

	// --- update v's info ---

	// Update the root of the vertices reachable from 'v'.
	//   (there is no need to reset the BFS object)
	bfs.set_process_current(
	[&](const internal::BFS<T>&, node w) -> void { root_of[w] = v; }
	);
	bfs.start_at(v);
	root_of[v] = v;
	root_size[v] = size_uv - size_u;
}

// -----------------------------------------------------------------------------

namespace __lal {

/* This function updates the union-find data structure of a tree prior to the
 * removal of the edge (u,v).
 *
 * This function is called by the function
 *		lal::internal::UnionFind_update_roots_before_remove_all_incident_to
 *
 * In particular, it updates the information associated to the vertices found
 * in the direction (u,v).
 */
template<typename T>
inline
void UnionFind_update_roots_before_remove_all_incident_to
(
	const T& t, node u, node v,
	node * const root_of,
	uint64_t * const root_size
)
noexcept
{
	BFS<T> bfs(t);
	bfs.set_use_rev_edges(t.is_directed());
	// avoid going 'backwards', we need to go 'onwards'
	bfs.set_visited(u, 1);

	uint64_t size_cc_v = 0;
	bfs.set_process_current(
	[&](const auto&, node w) -> void { root_of[w] = v; ++size_cc_v; }
	);
	bfs.start_at(v);

	root_of[v] = v;
	root_size[v] = size_cc_v;
}

} // -- namespace __lal

/* This function updates the union-find data structure of a tree prior to the
 * removal of the edges incidents to vertex 'u'.
 */
template<typename T>
inline
void UnionFind_update_roots_before_remove_all_incident_to
(
	const T& t, node u,
	node * const root_of,
	uint64_t * const root_size
)
noexcept
{
	if constexpr (std::is_base_of_v<graphs::free_tree, T>) {
		for (node v : t.get_neighbours(u)) {
			// update size and root of the edges from v onwards
			// (onwards means "in the direction u -> v"
			__lal::UnionFind_update_roots_before_remove_all_incident_to(
				t, u, v, root_of, root_size
			);
		}
	}
	else {
		for (node v : t.get_in_neighbours(u)) {
			// update size and root of the edges from v onwards
			// (onwards means "in the direction u -> v"
			__lal::UnionFind_update_roots_before_remove_all_incident_to(
				t, u, v, root_of, root_size
			);
		}
		for (node v : t.get_out_neighbours(u)) {
			// update size and root of the edges from v onwards
			// (onwards means "in the direction u -> v"
			__lal::UnionFind_update_roots_before_remove_all_incident_to(
				t, u, v, root_of, root_size
			);
		}
	}

	root_of[u] = u;
	root_size[u] = 1;
}

} // -- namespace internal
} // -- namespace lal
