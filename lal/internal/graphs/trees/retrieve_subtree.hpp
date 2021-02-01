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

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/internal/graphs/traversal.hpp>

namespace lal {
namespace internal {

/*
 * @brief Retrieves the edges of a subtree
 *
 * @param T Input tree.
 * @param u Root of the subtree.
 * @param relabel Relabel the vertices? If so, vertex 'u' is relabelled to 0
 * @param subsizes Store in an array the sizes of the subtrees of the subtree
 * rooted at 'u'.
 *
 * @pre The tree is a valid rooted tree
 * @pre The tree has vertex 'u'
 * @post The function has NO ownership of the raw pointer returned in the pair.
 * @post The pointer returned is not nullptr only when T.size_subtrees_valid()
 * and the boolean parameter sizes are both true.
 */
std::pair<std::vector<edge>, uint32_t *>
get_edges_subtree(
	const graphs::rooted_tree& T, node u,
	bool relabel, bool subsizes
)
{
#if defined DEBUG
	assert(T.is_rooted_tree());
	assert(T.has_node(u));
	if (subsizes) {
		assert(relabel);
	}
#endif

	std::vector<edge> es;
	uint32_t *sizes = nullptr;

	const uint32_t n = T.n_nodes();
	if (n <= 1) { return {es, sizes}; }

	// reserve some space for the vector edges
	// initialise the array of sizes if needed
	bool update_sizes = false;
	if (T.size_subtrees_valid()) {
		es.reserve(T.n_nodes_subtree(u));
		if (subsizes) {
			// The caller wants this function to retrieve the sizes of
			// the subtrees. This can be done because the sizes are valid.

			// Use only the space that is strictly necessary.
			sizes = new uint32_t[T.n_nodes_subtree(u)]{0};
			update_sizes = true;
		}
	}
	else {
		es.reserve(n/2);
		sizes = nullptr; // reiterate that we shouldn't do this here
	}

	// data structures for node relabelling
	node *relabelling = new node[n];
	std::fill(&relabelling[0], &relabelling[n], n+1);

	// relabel 'u' to '0' and make it the root
	relabelling[u] = 0;
	node next_label = 1;
	if (update_sizes) {
		sizes[relabelling[u]] = T.n_nodes_subtree(u);
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
					sizes[relabelling[s]] = T.n_nodes_subtree(s);
				}
			}
			e.first = relabelling[s];

			// relabel second node
			if (relabelling[t] >= n) {
				relabelling[t] = next_label;
				++next_label;
				if (update_sizes) {
					sizes[relabelling[t]] = T.n_nodes_subtree(t);
				}
			}
			e.second = relabelling[t];

			es.push_back(e);
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
				sizes[s] = T.n_nodes_subtree(s);
				sizes[t] = T.n_nodes_subtree(t);
			}
			es.push_back(edge(s,t));
		}
		);
	}
	bfs.start_at(u);

	// free local memory
	delete[] relabelling;
	return {es, sizes};
}

} // -- namespace internal
} // -- namespace lal
