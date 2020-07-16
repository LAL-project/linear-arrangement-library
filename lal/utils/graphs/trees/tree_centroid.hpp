/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Linear Arrangement Library.
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

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/rtree.hpp>
#include <lal/graphs/ftree.hpp>
#include <lal/utils/graphs/trees/tree_centre.hpp>
#include <lal/utils/graphs/trees/size_subtrees.hpp>
#include <lal/utils/graphs/traversal.hpp>
#include <lal/utils/graphs/trees/centre_centroid_utils.hpp>

namespace lal {
namespace utils {

namespace __lal {

inline bool is_centroidal(
	const graphs::rtree& t, uint32_t size_cc, node u, char *vis, uint32_t *sizes
)
{
	memset(sizes, 0, t.n_nodes()*sizeof(uint32_t));
	memset(vis, 0, t.n_nodes()*sizeof(char));
	get_size_subtrees(t, u, vis, sizes);
	for (const node v : t.get_neighbours(u)) {
		if (sizes[v] > size_cc/2) {
			return false;
		}
	}
	for (const node v : t.get_in_neighbours(u)) {
		if (sizes[v] > size_cc/2) {
			return false;
		}
	}
	return true;
}

inline bool is_centroidal(
	const graphs::ftree& t, uint32_t size_cc, node u, char *vis, uint32_t *sizes
)
{
	memset(sizes, 0, t.n_nodes()*sizeof(uint32_t));
	memset(vis, 0, t.n_nodes()*sizeof(char));
	__lal::get_size_subtrees(t, u, vis, sizes);
	for (const node v : t.get_neighbours(u)) {
		if (sizes[v] > size_cc/2) {
			return false;
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
 * the set has two vertices then they are adjacent in the tree.
 *
 * A graph of type @ref graphs::tree may lack some edges tree so it has several
 * connected components. Vertex @e x belongs to one of these connected
 * components.
 *
 * This method finds the central nodes of the connected
 * components node 'x' belongs to.
 *
 * @param t Input tree.
 * @param x Input node.
 * @returns Returns a tuple of two values: the nodes in the centroid. If the
 * tree has a single centroidal node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 */
template<
	class T,
	typename std::enable_if<std::is_base_of<graphs::tree, T>::value, int>::type = 0
>
std::pair<node, node> retrieve_centroid(const T& t, node x) {
	const uint32_t n = t.n_nodes();

	// allocate and initialise memory
	char *vis = static_cast<char *>(malloc(n*sizeof(char))); // not used for G=ftree
	uint32_t *sizes = static_cast<uint32_t *>(malloc(n*sizeof(uint32_t)));
	const auto dealloc = [&]() -> void { free(vis); free(sizes); };

	// size of the connected component
	uint32_t size_cc = 0;
	BFS<T> bfs(t);
	bfs.set_use_rev_edges(t.is_rooted());
	bfs.set_process_current(
	[&](const auto&, node) -> void { ++size_cc; }
	);
	bfs.start_at(x);

	if (size_cc == 1) {
		dealloc();
		return std::make_pair(x, n);
	}
	if (size_cc == 2) {
		dealloc();
		const node u = x;
		const node v = __lal::__only_neighbour(t,x);
		return (u < v ? std::make_pair(u,v) : std::make_pair(v,u));
	}

	// retrieve the centre of the tree
	const auto centre = retrieve_centre(t, x);

	bfs.reset();
	bfs.set_use_rev_edges(t.is_rooted());

	// --
	// find the centroid of the tree
	node ct1, ct2;
	ct1 = ct2 = n;

	// early termination of the procedure
	bfs.set_terminate(
	[&](const auto&, node) -> bool { return ct2 != n; }
	);
	// finding the centroidal vertices
	bfs.set_process_current(
	[&](const auto&, node s) -> void {
		// process only internal vertices
		if (__lal::__degree(t, s) > 1) {

			const bool is = __lal::is_centroidal(t, size_cc, s, vis, sizes);
			if (is) {
				if (ct1 == n) { ct1 = s; }
				else { ct2 = s; }
			}
		}
	}
	);
	// start at one of the central vertices
	bfs.start_at(centre.first);

	dealloc();
	return (ct1 < ct2 ? std::make_pair(ct1,ct2) : std::make_pair(ct2,ct1));
}

} // -- namespace utils
} // -- namespace lal
