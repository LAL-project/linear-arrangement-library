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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/tree.hpp>
#include <lal/graphs/rtree.hpp>
#include <lal/graphs/ftree.hpp>
#include <lal/utils/graphs/bfs.hpp>

namespace lal {
namespace utils {

namespace __lal {

inline uint32_t __degree_for_centre(const graphs::rtree& t, const node s)
{ return t.out_degree(s) + t.in_degree(s); }

inline uint32_t __degree_for_centre(const graphs::ftree& t, const node s)
{ return t.degree(s); }

inline node __get_only_neighbour(const graphs::rtree& t, const node s) {
	return
	(t.out_degree(s) == 0 ? t.get_in_neighbours(s)[0] : t.get_out_neighbours(s)[0]);
}

inline node __get_only_neighbour(const graphs::ftree& t, const node s) {
	return t.get_neighbours(s)[0];
}

} // -- namespace __lal

/*
 * @brief Calculate the centre of the connected component that has node @e u.
 *
 * A graph of type @ref graphs::tree may lack some edges tree so it has
 * several connected components. Vertex @e u belongs to one of these connected
 * components.
 *
 * This method finds the central nodes of the connected components node
 * 'u' belongs to.
 *
 * @param tree Input tree.
 * @param u Input node.
 * @returns Returns a tuple of two values: the nodes in the centre. If the
 * tree has a single central node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 */
template<
	class T,
	typename std::enable_if<std::is_base_of<graphs::tree, T>::value, int>::type = 0
>
std::pair<node, node> retrieve_centre(const T& tree, node u) {
	const auto n = tree.n_nodes();

	// First simple case:
	// in case the component of x has only one node (node x)...
	if (__lal::__degree_for_centre(tree, u) == 0) {
		return std::make_pair(u, n);
	}

	BFS<T> bfs(tree);
	bfs.set_use_rev_edges(tree.is_directed());

	// ------------------------------------
	// 1. find vertex 'v' farthest from 'u'

	std::vector<uint32_t> dists(tree.n_nodes());
	dists[u] = 0;

	// farthest vertex from 'u' and distance
	uint32_t max_dist = 0;
	node v;

	bfs.set_process_neighbour(
	[&](const auto&, node s, node t, bool) {
		dists[t] = dists[s] + 1;
		if (max_dist < dists[t]) {
			max_dist = dists[t];
			v = t;
		}
	}
	);
	bfs.start_at(u);

	// early termination
	if (v == u) {
		return std::make_pair(u, n);
	}

	// ------------------------------------
	// 2. find vertex 'w' farthest from 'v' and keep
	// track of the vertices from 'v' to 'w'

	std::fill(dists.begin(), dists.end(), 0);
	std::vector<std::vector<node>> paths(tree.n_nodes());
	paths[v] = std::vector<node>(1, v);

	max_dist = 0;
	std::vector<node> longest_path;
	node w;

	bfs.reset_visited();
	bfs.set_process_neighbour(
	[&](const auto&, node s, node t, bool) {
		dists[t] = dists[s] + 1;

		paths[t] = paths[s];
		paths[t].push_back(t);

		if (max_dist < dists[t]) {
			max_dist = dists[t];
			w = t;
			longest_path = paths[t];
		}
	}
	);
	bfs.start_at(v);

	// return centre

	if (longest_path.size()%2 == 0) {
		const size_t hm1 = longest_path.size()/2 - 1;
		const size_t h = longest_path.size()/2;

		const node c1 = longest_path[hm1];
		const node c2 = longest_path[h];
		return (c1 < c2 ? std::make_pair(c1,c2) : std::make_pair(c2,c1));
	}
	const size_t h = longest_path.size()/2;
	return std::make_pair(longest_path[h], n);
}

} // -- namespace utils
} // -- namespace lal
