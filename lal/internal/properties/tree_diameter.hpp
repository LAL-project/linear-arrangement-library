/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
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
#include <lal/graphs/tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/internal/graphs/traversal.hpp>
#include <lal/internal/data_array.hpp>

namespace lal {
namespace internal {

template<
	class T,
	std::enable_if_t<
		std::is_base_of_v<graphs::free_tree, T> ||
		std::is_base_of_v<graphs::rooted_tree, T>,
	bool> = true
>
uint32_t tree_diameter(const T& t) {
	const uint32_t n = t.get_num_nodes();

	BFS<T> bfs(t);

	if constexpr (std::is_base_of_v<graphs::rooted_tree, T>) {
	bfs.set_use_rev_edges(true);
	}
	else {
	bfs.set_use_rev_edges(false);
	}

	// find a leaf
	node leaf = 0;
	while (leaf < n and t.get_degree(leaf) > 1) { ++leaf; }
#if defined DEBUG
	// a leaf must exist in 't'
	assert(leaf < n);
#endif

	uint32_t diameter = 0;
	data_array<uint32_t> distance_from_leaf(n, 0);

	// calculate the maximum distance (in edges)
	bfs.set_process_neighbour(
	[&](const auto&, node u, node v, bool) {
		distance_from_leaf[v] = distance_from_leaf[u] + 1;
		diameter = std::max(diameter, distance_from_leaf[v]);
	}
	);
	bfs.start_at(leaf);

	return diameter;
}

} // -- namespace internal
} // -- namespace lal
