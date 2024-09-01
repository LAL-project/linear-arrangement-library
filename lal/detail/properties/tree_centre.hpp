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
#include <vector>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/detail/graphs/traversal.hpp>

namespace lal {
namespace detail {

/**
 * @brief Calculate the centre of the connected component that has node @e x.
 *
 * See @ref LAL_concepts__centre_centroid for details on what the centre of a
 * tree is.
 *
 * A graph of type @ref lal::graphs::tree may lack some edges tree so it has
 * several connected components. Vertex @e x belongs to one of these connected
 * components.
 *
 * This method finds the central nodes of the connected components node
 * 'x' belongs to.
 *
 * @tparam tree_t type of tree.
 * @param t Input tree.
 * @param X Input node.
 * @pre Input tree @e t may be a forest.
 * @returns A tuple of the two nodes in the centre. If the tree has a single
 * central node, only the first node is valid and the second is assigned an
 * invalid vertex index. It is guaranteed that the first vertex has smaller
 * index value than the second.
 */
template <
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool > = true
>
[[nodiscard]] std::pair<node, node> retrieve_centre
(const tree_t& t, const node X)
noexcept
{
	// number of nodes of the whole tree
	const auto n = t.get_num_nodes();

	// First simple case:
	// in case the component of x has only one node (node x)...
	if (t.get_num_nodes_component(X) == 1) {
		return {X, n+1};
	}

	// Second simple case:
	// if the connected component has two nodes then
	if (t.get_num_nodes_component(X) == 2) {
		// case component_size==1 is actually the first simple case
		const node v1 = X;

		// only neighbour of X
		node v2;
		if constexpr (std::is_base_of_v<graphs::free_tree, tree_t>) {
			v2 = t.get_neighbors(X)[0];
		}
		else {
			v2 = (t.get_out_degree(X) == 0 ?
				t.get_in_neighbors(X)[0] : t.get_out_neighbors(X)[0]
			);
		}
		return (v1 < v2 ? std::make_pair(v1, v2) : std::make_pair(v2, v1));
	}

	BFS<tree_t> bfs(t);

	// leaves of the orginal tree's connected component
	std::vector<node> tree_leaves;
	tree_leaves.reserve(t.get_num_nodes_component(X) - 1);
	// full degree of every node of the connected component
	array<uint64_t> trimmed_degree(n, 0);
	// number of nodes in the connected component
	uint64_t size_trimmed = t.get_num_nodes_component(X);

#if defined DEBUG
	uint64_t __size_trimmed = 0; // for debugging purposes only
#endif

	// leaves left to process
	//   l0: leaves in the current tree
	uint64_t l0 = 0;
	//   l1: leaves produced after having trimmed all the l0 leaves
	uint64_t l1 = 0;

	// ---------------------------------------------------
	// Initialize data:
	// 1. fill in 'trimmed_degree' values
	// 2. retrieve connected component's leaves ('tree_leaves')
	// 3. calculate amount of leaves left to process ('l0')
	bfs.set_process_current(
	[&](const auto&, node u) -> void {
#if defined DEBUG
		++__size_trimmed;
#endif

		// 'trimmed_degree' must be the degree of the vertex
		// in the underlying undirected graph!
		trimmed_degree[u] = t.get_degree(u);

		if (trimmed_degree[u] == 1) {
			tree_leaves.push_back(u);
			++l0;
		}
	}
	);

	if constexpr (std::is_base_of_v<graphs::free_tree, tree_t>) {
		bfs.set_use_rev_edges(false);
	}
	else {
		bfs.set_use_rev_edges(true);
	}

	bfs.start_at(X);

#if defined DEBUG
	// make sure that the method n_nodes_component returns a correct value
	assert(__size_trimmed == size_trimmed);
#endif

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

	bfs.set_process_visited_neighbors(true);
	bfs.set_process_neighbour(
	[&](const auto&, node u, node v, bool) -> void
	{
		// ignore the edge if one of its nodes has already been trimmed out.
		if (trimmed_degree[u] == 0) { return; }
		if (trimmed_degree[v] == 0) { return; }

		// trim node 's':
		//  1) its degree is set to null, 2) node 't' loses a neighbour, so
		//  its degree is reduced by 1. 3) the size of the trimmed tree
		//  decreases by 1.
		trimmed_degree[u] = 0;
		--trimmed_degree[v];
		--size_trimmed;

		if (trimmed_degree[v] == 0) {
			has_single_center = true;
			single_center = v;
		}

		// leaves left to process in the current trimmed tree
		--l0;
		// leaves left to process in the next trimmed tree
		if (trimmed_degree[v] == 1) {
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
	[&](const auto&, node, node v, bool) -> bool { return trimmed_degree[v] == 1; }
	);

	// do the bfs from the leaves inwards
	bfs.set_use_rev_edges(t.is_directed());
	bfs.start_at(tree_leaves);

	if (has_single_center) {
#if defined DEBUG
		assert(size_trimmed == 1);
#endif
		return {single_center, n + 1};
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
	bfs.set_use_rev_edges(t.is_directed());

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
	bfs.start_at(X);

	// return the nodes in the right order according to index values
	return (v1 < v2 ? std::make_pair(v1, v2) : std::make_pair(v2, v1));
}

} // -- namespace detail
} // -- namespace lal
