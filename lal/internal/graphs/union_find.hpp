/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#ifdef DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/definitions.hpp>
#include <lal/internal/graphs/traversal.hpp>

namespace lal {
namespace internal {

template<class T>
void UnionFind_update_roots_add(
	const T& t, node u, node v,
	node *root_of,
	uint32_t *root_size
)
{
	// 'u' and 'v' are not connected, so they belong to
	// (different) connected components of the tree.

	// 'parent' and 'child' determine a direction to be used later.
	// 'new_root' is the new root for one of the vertices
	node parent, child, new_root;

	const node root_u = root_of[u];
	const node root_v = root_of[v];

	const uint32_t size_u = root_size[root_u];
	const uint32_t size_v = root_size[root_v];
	const uint32_t new_size = size_u + size_v;

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
	internal::BFS<T> bfs(t);
	bfs.set_use_rev_edges(t.is_directed());
	bfs.set_process_current(
	[&](const auto&, node w) -> void { root_of[w] = new_root; }
	);
	bfs.set_visited(parent); // avoid going backwards
	bfs.start_at(child);
}

template<class T>
void UnionFind_update_roots_remove(
	const T& t, node u, node v,
	node *root_of,
	uint32_t *root_size
)
{
	// 'u' and 'v' are connected
#ifdef DEBUG
	assert(root_of[u] == root_of[v]);
#endif

	const uint32_t size_uv = root_size[root_of[u]];

	internal::BFS<T> bfs(t);

	// --- update u's info ---

	// Update the root of the vertices reachable from 'u'.
	//   (also calculate the size of u's component)
	uint32_t size_u = 0;
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
	[&](const auto&, node w) -> void { root_of[w] = v; }
	);
	bfs.start_at(v);
	root_of[v] = v;
	root_size[v] = size_uv - size_u;
}

} // -- namespace internal
} // -- namespace lal
