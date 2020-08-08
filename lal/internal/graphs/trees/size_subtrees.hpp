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
#include <cstring>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace internal {

namespace __lal {

/*
 * @brief Calculate the size of every subtree of the free tree @e t.
 *
 * @param t Input rooted tree.
 * @param r The first call to this method, node @e r is interpreted as its
 * hypothetical root.
 * @param vis Mark nodes as visited as the algorithm goes on.
 * @param sizes The size of the subtree rooted at every reachable node
 * from @e r.
 * @pre Parameter @e sizes has size the number of vertices.
 */
inline void get_size_subtrees(
	const graphs::free_tree& t, node r, char *vis, uint32_t *sizes
)
{
	sizes[r] = 1;
	vis[r] = 1;
	for (const node u : t.get_neighbours(r)) {
		if (vis[u] == 0) {
			get_size_subtrees(t, u, vis, sizes);
			sizes[r] += sizes[u];
		}
	}
}

/*
 * @brief Calculate the size of every subtree of the free tree @e t.
 *
 * @param t Input rooted tree.
 * @param r The first call to this method, node @e r is interpreted as its
 * hypothetical root.
 * @param vis Mark nodes as visited as the algorithm goes on.
 * @param sizes The size of the subtree rooted at every reachable node
 * from @e r.
 * @pre Parameter @e sizes has size the number of vertices.
 */
inline void get_size_subtrees(
	const graphs::rooted_tree& t, node r, char *vis, uint32_t *sizes
)
{
	sizes[r] = 1;
	vis[r] = 1;
	for (const node u : t.get_neighbours(r)) {
		if (vis[u] == 0) {
			get_size_subtrees(t, u, vis, sizes);
			sizes[r] += sizes[u];
		}
	}
	for (const node u : t.get_in_neighbours(r)) {
		if (vis[u] == 0) {
			get_size_subtrees(t, u, vis, sizes);
			sizes[r] += sizes[u];
		}
	}
}

} // -- namespace __lal

/*
 * @brief Calculate the size of every subtree of tree @e t.
 *
 * The method starts calculating the sizes at node @e r. Since rooted trees
 * have directed edges, starting at a node different from the tree's root
 * may not calculate every subtree's size.
 * @param t Input rooted tree.
 * @param r Start calculating sizes of subtrees at this node.
 * @param vis Mark nodes as visited as the algorithm goes on.
 * @param sizes The size of the subtree rooted at every reachable node
 * from @e r.
 * @pre Parameter @e sizes has size the number of vertices.
 */
inline void get_size_subtrees(
	const graphs::rooted_tree& t, node r, uint32_t *sizes
)
{
	// visited vertices
	char *vis = static_cast<char *>(malloc(t.n_nodes()*sizeof(char)));
	memset(vis, 0, t.n_nodes()*sizeof(char));
	__lal::get_size_subtrees(t, r, vis, sizes);
	free(vis);
}

/*
 * @brief Calculate the size of every subtree of the free tree @e t.
 *
 * The method starts calculating the sizes at node @e r, that is, calculates
 * the sizes of every subtree considering @e r as its root.
 * @param t Input rooted tree.
 * @param r Hypothetical root of the free tree.
 * @param vis Mark nodes as visited as the algorithm goes on.
 * @param sizes The size of the subtree rooted at every reachable node
 * from @e r.
 * @pre Parameter @e sizes has size the number of vertices.
 */
inline void get_size_subtrees(
	const graphs::free_tree& t, node r, uint32_t *sizes
)
{
	// visited vertices
	char *vis = static_cast<char *>(malloc(t.n_nodes()*sizeof(char)));
	memset(vis, 0, t.n_nodes()*sizeof(char));
	__lal::get_size_subtrees(t, r, vis, sizes);
	free(vis);
}

} // -- namespace internal
} // -- namespace lal
