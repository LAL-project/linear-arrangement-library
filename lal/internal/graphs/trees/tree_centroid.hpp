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
#include <map>

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/internal/graphs/traversal.hpp>
#include <lal/internal/graphs/trees/size_subtrees.hpp>

namespace lal {
namespace internal {

namespace __lal {

/*
 * @brief Returns whether the vertex @e u is a centroidal tree
 *
 * @param t Input tree.
 * @param n Number of vertices of the connected component to which vertex @e u belongs.
 * @param u Vertex to be classified as centroidal.
 * @param sizes See documentation of method internal::calculate_suvs.
 * @return Returns whether vertex @e u is centroidal or not.
 */
template<
	class T,
	typename std::enable_if<std::is_base_of<graphs::tree, T>::value, int>::type = 0
>
bool is_centroidal(
	const T& t, uint32_t n, node u, const std::map<edge, uint32_t>& sizes
)
{
	for (node v : t.get_out_neighbours(u)) {
		const edge e(u,v);
		const auto it = sizes.find(e);
		if (it->second > n/2) { return false; }
	}
	if constexpr (std::is_same<graphs::rooted_tree,T>::value) {
	for (node v : t.get_in_neighbours(u)) {
		const edge e(u,v);
		const auto it = sizes.find(e);
		if (it->second > n/2) { return false; }
	}
	}
	return true;
}

} // -- namespace __lal

/*
 * @brief Calculate the centroid of the connected component that has node @e x.
 *
 * Here, "centroid" should NOT be confused with "centre". The centre is the set
 * of (at most) two vertices that have minimum eccentricity. The centroid is the
 * set of (at most) two vertices that have minimum weight, where the weight is
 * the maximum size of the subtrees rooted at that vertex. In both case, if
 * the set has two vertices then they are adjacent in the tree. See \cite Harary1969a
 * for further details.
 *
 * A graph of type @ref graphs::tree may lack some edges tree so it can have
 * several connected components. Vertex @e x belongs to one of these connected
 * components. So, this method finds the centroidal nodes of the connected
 * component node @e x belongs to.
 *
 * This function uses @ref internal::calculate_suvs, an algorithm described
 * in \cite Hochberg2003a (see function's documentation for details).
 * @param t Input tree.
 * @param x Input node.
 * @param[out] sizes_edge See documentation of method internal::calculate_suvs.
 * @returns Returns a tuple of two values: the nodes in the centroid. If the
 * tree has a single centroidal node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 */
template<
	class T,
	typename std::enable_if<std::is_base_of<graphs::tree, T>::value, int>::type = 0
>
std::pair<node, node> retrieve_centroid(
	const T& t, node x, std::map<edge, uint32_t>& sizes_edge
)
{
	// actual number of vertices of the tree
	const uint32_t N = t.n_nodes();

	// calculate the size of the connected component
	uint32_t n = 0;
	{
	BFS<T> bfs(t);
	bfs.set_use_rev_edges(t.is_directed());
	bfs.set_process_current( [&](const auto&, node) -> void { ++n; } );
	bfs.start_at(x);
	}
	if (n == 1) {
		return std::make_pair(x, t.n_nodes());
	}

	{
	// empty the map
	std::map<edge, uint32_t> empty;
	sizes_edge.swap(empty);
	}

	for (node y : t.get_out_neighbours(x)) {
		calculate_suvs(t,n, x, y, sizes_edge);
	}
	if constexpr (std::is_same<graphs::rooted_tree, T>::value) {
	for (node y : t.get_in_neighbours(x)) {
		if (y != x) {
			calculate_suvs(t,n, x, y, sizes_edge);
		}
	}
	}

	node c1 = N + 1;
	node c2 = N + 1;
	BFS<T> bfs(t);

	// find the first centroidal vertex
	bfs.set_terminate( [&](const auto&, node) -> bool { return c1 < n; } );
	bfs.set_node_add(
	[&](const auto&, node u, node v) -> bool {
		// add vertex 'v' to the queue only if the size s(u,v) > n/2
		const edge e(u,v);
		const auto it = sizes_edge.find(e);
		return it->second > n/2;
	}
	);
	bfs.set_process_current(
	[&](const auto&, node u) -> void {
		if (__lal::is_centroidal(t,n, u, sizes_edge)) {
			c1 = u;
		}
	}
	);
	bfs.set_use_rev_edges(t.is_directed());
	bfs.start_at(x);

#if defined DEBUG
	assert(c1 < N);
#endif

	// inspect neighbours of the centroidal vertex
	// found to find (quite likely) a second
	for (node u : t.get_out_neighbours(c1)) {
		if (__lal::is_centroidal(t,n, u, sizes_edge)) {
			c2 = u;
			break;
		}
	}
	if constexpr (std::is_same<graphs::rooted_tree, T>::value) {
	if (c2 >= N) {
		for (node u : t.get_in_neighbours(c1)) {
			if (__lal::is_centroidal(t,n, u, sizes_edge)) {
				c2 = u;
				break;
			}
		}
	}
	}

	return (c1 < c2 ? std::make_pair(c1, c2) : std::make_pair(c2, c1));
}

/*
 * @brief Calculate the centroid of the connected component that has node @e x.
 *
 * For details on the parameters and return value see documentation of the
 * function above.
 */
template<
	class T,
	typename std::enable_if<std::is_base_of<graphs::tree, T>::value, int>::type = 0
>
std::pair<node, node> retrieve_centroid(const T& t, node x) {
	std::map<edge, uint32_t> sizes_edge;
	return retrieve_centroid(t, x, sizes_edge);
}

} // -- namespace internal
} // -- namespace lal
