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
#include <lal/graphs/rtree.hpp>
#include <lal/graphs/ftree.hpp>
#include <lal/utils/graphs/traversal.hpp>
#include <lal/utils/graphs/trees/centre_centroid_utils.hpp>

namespace lal {
namespace utils {

/*
 * @brief Calculate the centre of the connected component that has node @e x.
 *
 * Here, "centre" should NOT be confused with "centroid". The center is the set
 * of (at most) two vertices that have minimum eccentricity. The centroid is the
 * set of (at most) two vertices that have minimum weight, where the weight is
 * the maximum size of the subtrees rooted at that vertex.
 *
 * A graph of type @ref graphs::tree may lack some edges tree so it has
 * several connected components. Vertex @e x belongs to one of these connected
 * components.
 *
 * This method finds the central nodes of the connected components node
 * 'x' belongs to.
 *
 * @param t Input tree.
 * @param x Input node.
 * @returns Returns a tuple of two values: the nodes in the centre. If the
 * tree has a single central node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 */
template<
	class G,
	typename std::enable_if<std::is_base_of<graphs::tree, G>::value, int>::type = 0
>
std::pair<node, node> retrieve_centre(const G& T, node x) {
	const auto n = T.n_nodes();

	// First simple case:
	// in case the component of x has only one node (node x)...
	if (__lal::__degree(T, x) == 0) {
		return std::make_pair(x, n);
	}

	BFS<G> bfs(T);

	// leaves of the orginal tree's connected component
	std::vector<node> tree_leaves;
	// full degree of every node of the connected component
	std::vector<uint32_t> trimmed_degree(n, 0);
	// number of nodes in the connected_component
	uint32_t size_trimmed = 0;

	// leaves left to process
	//   l0: leaves in the current tree
	uint32_t l0 = 0;
	//   l1: leaves produced after having trimmed all the l0 leaves
	uint32_t l1 = 0;

	// ---------------------------------------------------
	// Initialise data:
	// 1. calculate number of nodes in the current connected component
	//    ('size_trimmed')
	// 2. fill in 'trimmed_degree' values
	// 3. retrieve connected component's leaves ('tree_leaves')
	// 4. calculate amount of leaves left to process ('l0')
	bfs.set_process_current(
	[&](const auto&, node s) -> void {
		++size_trimmed;
		trimmed_degree[s] = __lal::__degree(T, s);
		if (trimmed_degree[s] == 1) {
			tree_leaves.push_back(s);
			++l0;
		}
	}
	);
	bfs.set_use_rev_edges(T.is_directed());
	bfs.start_at(x);

	// Second simple case:
	// if the connected component has two nodes then
	if (size_trimmed == 2) {
		// case component_size==1 is actually the first simple case
		const node v1 = x;
		const node v2 = __lal::__only_neighbour(T, x);
		return (v1 < v2 ? std::make_pair(v1, v2) : std::make_pair(v2, v1));
	}

	// Third case: the component has three nodes or more...

	// ---------------------------------------------------
	bfs.reset();

	// ---------------------------------------------------
	// retrieve the centre of the connected component

	bfs.set_terminate(
	[&](const auto&, node) -> bool {
		// Meaning of every condition:
		// --> l0 == 1 or l0 == 2
		//     The trimmmed tree has 1 or 2 leaves left.
		// --> l1 == 0
		//     After trimming once, the trimmed tree can't be trimmed any further.
		// --> size_trimmed <= 2
		//     Note that a (trimmed) linear tree (or path graph) has two leaves.
		//     This means that the conditions so far are true. However, this
		//     does not mean we have calculated the centre because there still
		//     is a big amount of leaves to trim. Therefore, we need a trimmed
		//     tree of at most two nodes to finish.
		return (l0 == 1 or l0 == 2) and l1 == 0 and size_trimmed <= 2;
	}
	);

	// does the connected component have unique centre?
	bool has_single_center = false;
	node single_center = n + 1;

	bfs.set_process_visited_neighbours(true);
	bfs.set_process_neighbour(
	[&](const auto&, node s, node t, bool) -> void
	{
		// ignore the edge if one of its nodes has already been trimmed out.
		if (trimmed_degree[s] == 0) { return; }
		if (trimmed_degree[t] == 0) { return; }

		// trim node 's':
		//  1) its degree is set to null, 2) node 't' loses a neighbour, so
		//  its degree is reduced by 1. 3) the size of the trimmed tree
		//  decreases by 1.
		trimmed_degree[s] = 0;
		--trimmed_degree[t];
		--size_trimmed;

		if (trimmed_degree[t] == 0) {
			has_single_center = true;
			single_center = t;
		}

		// leaves left to process in the current trimmed tree
		--l0;
		// leaves left to process in the next trimmed tree
		if (trimmed_degree[t] == 1) {
			++l1;
			if (l0 == 0) {
				// l0 <- l1
				// l1 <- 0
				std::swap(l0, l1);
			}
		}
	}
	);

	// add the next node only if its degree
	// (in the trimmed tree) is exactly one.
	bfs.set_node_add(
	[&](const auto&, node s) -> bool { return (trimmed_degree[s] == 1); }
	);

	// do the bfs from the leaves inwards
	bfs.set_use_rev_edges(T.is_directed());
	bfs.start_at(tree_leaves);

	if (has_single_center) {
#if defined DEBUG
		assert(size_trimmed == 1);
#endif
		return std::make_pair(single_center, n);
	}

	// in case the 'has_single_center' boolean is false
	// the variable 'size_trimmed' must equal 2.
#if defined DEBUG
	assert(size_trimmed == 2);
#endif

	// ---------------------------------------------------
	// retrieve the two central nodes

	// -- reset the bfs
	bfs.reset();
	bfs.set_use_rev_edges(T.is_directed());

	node v1, v2;
	v1 = v2 = n;

	// Traverse the connected component of 'x' in order to find the central
	// nodes. NOTE: we could use a "for" loop through the 'n' nodes of the
	// tree, but this BFS-traversal might be faster (due to the fewer
	// amount of vertices in the connected component).
	bfs.set_process_current(
	[&](const auto&, node u) -> void {
		if (trimmed_degree[u] == 1) {
			if (v1 == n) { v1 = u; }
			else { v2 = u; }
		}
	}
	);
	bfs.start_at(x);

	// return the nodes in the right order according to index values
	return (v1 < v2 ? std::make_pair(v1, v2) : std::make_pair(v2, v1));
}

} // -- namespace utils
} // -- namespace lal
