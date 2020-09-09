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
#if defined DEBUG
#include <cassert>
#endif
#include <cstring>
#include <cstdlib>
#include <vector>

// lal includes
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace internal {

namespace __lal {

/*
 * @brief Calculate the size of every subtree of the tree @e t.
 *
 * @param t Input tree.
 * @param r The first call to this method, node @e r is interpreted as its
 * hypothetical root.
 * @param vis Mark nodes as visited as the algorithm goes on.
 * @param sizes The size of the subtree rooted at every reachable node
 * from @e r.
 * @pre Parameter @e sizes has size the number of vertices.
 */
template<
	class T,
	typename std::enable_if<std::is_base_of<graphs::tree, T>::value, int>::type = 0
>
void get_size_subtrees(
	const T& t, node r, char *vis, uint32_t *sizes
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
	if constexpr (std::is_same<graphs::rooted_tree, T>::value) {
	for (const node u : t.get_in_neighbours(r)) {
		if (vis[u] == 0) {
			get_size_subtrees(t, u, vis, sizes);
			sizes[r] += sizes[u];
		}
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
 * @param sizes The size of the subtree rooted at every reachable node from @e r.
 * @pre Parameter @e sizes has size the number of vertices.
 */
template<
	class T,
	typename std::enable_if<std::is_base_of<graphs::tree, T>::value, int>::type = 0
>
void get_size_subtrees(
	const T& t, node r, uint32_t *sizes
)
{
	// visited vertices
	char *vis = static_cast<char *>(malloc(t.n_nodes()*sizeof(char)));
	memset(vis, 0, t.n_nodes()*sizeof(char));
	__lal::get_size_subtrees(t, r, vis, sizes);
	free(vis);
}

namespace __lal {

/*
 * @brief Calculates the values \f$s(u,v)\f$ for the edges \f$(u,v)\f$ reachable
 * from \$(u,v)\f$.
 *
 * This function calculates the 'map' relating each edge \f$(u, v)\f$ with the
 * size of the subtree rooted at \f$v\f$ with respect to the hypothetical root
 * \f$u\f$. This is an implementation of the algorithm described in
 * \cite Hochberg2003a (proof of lemma 8 (page 63), and the beginning of
 * section 6 (page 65)).
 *
 * Notice that the values are not stored in an actual map (std::map, or similar),
 * but in a vector.
 * @param t Input tree.
 * @param n Size of the connected component to which edge \f$(u,v)\f$ belongs to.
 * @param u First vertex of the edge.
 * @param v Second vertex of the edge.
 * @param[out] sizes_edge The vector of tuples.
 * @pre Vertices @e u and @e v belong to the same connected component.
 */
template<
	class T,
	typename std::enable_if<std::is_base_of<graphs::tree, T>::value, int>::type = 0
>
uint32_t calculate_suvs(
	const T& t, uint32_t n, node u, node v,
	std::vector<std::pair<edge, uint32_t>>& sizes_edge
)
{
	uint32_t r = 1;
	for (node w : t.get_out_neighbours(v)) {
		if (w != u) {
			r += calculate_suvs(t,n, v, w, sizes_edge);
		}
	}

	if constexpr (std::is_same<graphs::rooted_tree, T>::value) {
	for (node w : t.get_in_neighbours(v)) {
		if (w != u) {
			r += calculate_suvs(t,n, v, w, sizes_edge);
		}
	}
	}

	sizes_edge.push_back(std::make_pair(edge(u,v), r));
	sizes_edge.push_back(std::make_pair(edge(v,u), n - r));
	return r;
}

} // -- namespace __lal

/*
 * @brief Calculates the values \f$s(u,v)\f$ for the edges \f$(u,v)\f$ reachable
 * from vertex @e x.
 *
 * See @ref __lal::calculate_suvs for details.
 */
template<
	class T,
	typename std::enable_if<std::is_base_of<graphs::tree, T>::value, int>::type = 0
>
void calculate_suvs(
	const T& t, uint32_t n, node x,
	std::vector<std::pair<edge, uint32_t>>& sizes_edge
)
{
	for (node y : t.get_out_neighbours(x)) {
		__lal::calculate_suvs(t,n, x, y, sizes_edge);
	}
	if constexpr (std::is_same<graphs::rooted_tree, T>::value) {
	for (node y : t.get_in_neighbours(x)) {
		if (y != x) {
			__lal::calculate_suvs(t,n, x, y, sizes_edge);
		}
	}
	}

#if defined DEBUG
	assert(sizes_edge.size() <= 2*(t.n_nodes()) - 1);
#endif
}

} // -- namespace internal
} // -- namespace lal
