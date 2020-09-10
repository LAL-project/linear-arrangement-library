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
#include <vector>

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/internal/graphs/traversal.hpp>
#include <lal/internal/graphs/trees/size_subtrees.hpp>
#include <lal/internal/sorting/counting_sort.hpp>

namespace lal {
namespace internal {

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
	const T& t, node x, std::vector<std::pair<edge, uint32_t>>& sizes_edge
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

	// empty the vector
	{
	std::vector<std::pair<edge, uint32_t>> empty;
	sizes_edge.swap(empty);
	}

	// calculate s(u,v) with H&S algorithm (lemma 8)
	calculate_suvs(t,n, x, sizes_edge);

	{
	// sort all tuples in sizes_edge using the sizes
	typedef std::pair<edge,uint32_t> t2;
	typedef std::vector<t2>::iterator t2_vec_it;
	internal::counting_sort<t2_vec_it, t2, false>(
		sizes_edge.begin(), sizes_edge.end(), n,
		[](const t2& edge_pair) -> size_t { return edge_pair.second; }
	);
	}

	// put the s(u,v) into an adjacency list
	// M[u] : adjacency list of vertex u sorted increasingly according
	// to the sizes of the subtrees.
	std::vector<std::vector<std::pair<node,uint32_t>>> M(N);
	for (const auto& edge_value : sizes_edge) {
		const edge& e = edge_value.first;
		const node u = e.first;
		const node v = e.second;
		const uint32_t suv = edge_value.second;
		M[u].push_back(std::make_pair(v,suv));
	}

	// find the first centroidal vertex
	node c1 = N + 1;
	bool some_greater = true;
	node u = x;
	do {
		some_greater = false;
		for (const auto& p : M[u]) {
			const node v = p.first;
			const uint32_t suv = p.second;
			if (suv > n/2) {
				some_greater = true;
				u = v;
				break;
			}
		}
	}
	while (some_greater);
	c1 = u;

#if defined DEBUG
	assert(c1 < N);
#endif

	// find the second centroidal vertex
	node c2 = N + 1;
	for (const auto& p : M[c1]) {
		const node v = p.first;
		if (M[v][0].second <= n/2) {
			c2 = v;
			break;
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
	std::vector<std::pair<edge, uint32_t>> sizes_edge;
	return retrieve_centroid(t, x, sizes_edge);
}

} // -- namespace internal
} // -- namespace lal
