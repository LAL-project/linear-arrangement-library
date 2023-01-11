/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2023
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <utility>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/graphs/traversal.hpp>
#include <lal/detail/data_array.hpp>

namespace lal {
namespace detail {

/**
 * @brief Retrieves the edges of a subtree
 * @tparam get_subsizes Should the result also keep the sizes of the subtrees?
 * @param T Input rooted tree.
 * @param u Root of the subtree.
 * @param relabel Relabel the vertices? If so, vertex 'u' is relabelled to 0
 * @returns A pair consisting of the list of edges and, optionally, a raw pointer
 * to memory containing the sizes of the subtrees.
 * @pre The tree is a valid rooted tree.
 * @pre The tree has vertex 'u'.
 * @post The function has NO ownership of the raw pointer returned in the pair.
 * @post The pointer returned is not nullptr only when T.size_subtrees_valid()
 * AND the boolean parameter sizes are BOTH true.
 */
template <bool get_subsizes>
std::pair<std::vector<edge>, uint64_t *> get_edges_subtree
(const graphs::rooted_tree& T, node u, bool relabel)
noexcept
{
#if defined DEBUG
	assert(T.is_rooted_tree());
	assert(T.has_node(u));
	if constexpr (get_subsizes) {
		assert(relabel);
	}
#endif

	std::vector<edge> es;
	uint64_t *sizes = nullptr;

	const uint64_t n = T.get_num_nodes();
	if (n <= 1) { return {std::move(es), sizes}; }

	// reserve some space for the vector edges
	// initialise the array of sizes if needed
	bool update_sizes = false;
	if (T.are_size_subtrees_valid()) {
		es.reserve(T.get_num_nodes_subtree(u));
		if constexpr (get_subsizes) {
			// The caller wants this function to retrieve the sizes of
			// the subtrees. This can be done because the sizes are valid.

			// Use only the space that is strictly necessary.
			sizes = new uint64_t[T.get_num_nodes_subtree(u)]{0};
			update_sizes = true;
		}
	}
	else {
		es.reserve(n/2);
		sizes = nullptr; // reiterate that we shouldn't do this here
	}

	// data structures for node relabelling
	data_array<node> relabelling(n, n + 1);

	// relabel 'u' to '0' and make it the root
	relabelling[u] = 0;
	node next_label = 1;
	if (update_sizes) {
		sizes[relabelling[u]] = T.get_num_nodes_subtree(u);
	}

	BFS<graphs::rooted_tree> bfs(T);
	bfs.set_use_rev_edges(false);
	// retrieve edges and relabel them at the same time
	if (relabel) {
		bfs.set_process_neighbour(
		[&](const auto&, node s, node t, bool left_to_right) -> void {
			// change the orientation of the edge whenever appropriate
			// left_to_right: true  ---> "s->t"
			// left_to_right: false ---> "t->s"
			if (not left_to_right) { std::swap(s,t); }

			edge e;
			// relabel first node
			if (relabelling[s] >= n) {
				relabelling[s] = next_label;
				++next_label;
				if (update_sizes) {
					sizes[relabelling[s]] = T.get_num_nodes_subtree(s);
				}
			}
			e.first = relabelling[s];

			// relabel second node
			if (relabelling[t] >= n) {
				relabelling[t] = next_label;
				++next_label;
				if (update_sizes) {
					sizes[relabelling[t]] = T.get_num_nodes_subtree(t);
				}
			}
			e.second = relabelling[t];

			es.emplace_back(e);
		}
		);
	}
	else {
		bfs.set_process_neighbour(
		[&](const auto&, node s, node t, bool left_to_right) -> void {
			// change the orientation of the edge whenever appropriate
			// dir: true  ---> "s->t"
			// dir: false ---> "t->s"
			if (not left_to_right) { std::swap(s,t); }
			if (update_sizes) {
				sizes[s] = T.get_num_nodes_subtree(s);
				sizes[t] = T.get_num_nodes_subtree(t);
			}
			es.emplace_back(s,t);
		}
		);
	}
	bfs.start_at(u);

	return {std::move(es), sizes};
}

} // -- namespace detail
} // -- namespace lal
