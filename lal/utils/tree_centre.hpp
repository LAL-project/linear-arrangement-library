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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
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
#include <lal/utils/bfs.hpp>
#include <lal/utils/std_utils.hpp>

namespace lal {
namespace utils {

/*
 * @brief Calculate the centre of the connected component that has vertex @e s.
 *
 * A graph of type @ref graphs::tree t may not be a complete tree so it has
 * several connected components. Vertex 's' belongs to one of these connected
 * components.
 *
 * This method finds the central vertices of the connected components vertex
 * 'x' belongs to.
 *
 * @param t Input tree.
 * @param x Input vertex.
 * @returns Returns a tuple of three values. The first is a numerical value
 * that indicates how many vertices are in the centre of the tree. The other
 * two are the vertices in the centre. If the has a single central vertex,
 * only the first vertex is valid.
 */
template<
	class G,
	typename std::enable_if<std::is_base_of<graphs::tree, G>::value, int>::type = 0
>
std::pair<node, node> retrieve_centre(const G& T, node x) {
	const auto n = T.n_nodes();
	BFS<G> bfs(T);
	bfs.set_use_rev_edges(T.is_directed());

	// leaves of the orginal tree's connected component
	std::vector<node> tree_leaves;
	// full degree of every vertex of the tree
	std::vector<uint32_t> trimmed_degree(n, 0);
	// number of vertices in the trimmed tree
	uint32_t size_trimmed = n;
	// number of vertices in the connected component
	uint32_t component_size = 0;

	// leaves left to process
	//   l0: leaves in the current tree
	uint32_t l0 = 0;
	//   l1: leaves produced after having trimmed all the l0 leaves
	uint32_t l1 = 0;

	// ---------------------------------------------------
	// Initialise data:
	// 1. calculate number of vertices in the current tree
	//    (actually, connected component)
	// 2. fill in 'degree' values
	// 3. retrieve connected component's leaves
	// 4. calculate amount of leaves left to process
	bfs.set_process_current(
	[&](const auto&, node s) -> void {
		++component_size;
		trimmed_degree[s] = T.degree(s);
		if (T.degree(s) == 1) {
			tree_leaves.push_back(s);
			++l0;
		}
	}
	);
	bfs.start_at(x);

	// ---------------------------------------------------
	// -- reset the bfs
	bfs.reset();
	bfs.set_use_rev_edges(T.is_directed());

	// ---------------------------------------------------
	// retrieve the centre of the connected component

	bfs.set_terminate(
	[&](const auto&, node) -> bool {
		// Meaning of every condition:
		// --> ll0 == 1 or ll0 == 2
		//     The trimmmed tree has 1 or 2 leaves left.
		// --> ll1 == 0
		//     After trimming once, the trimmed tree can't be trimmed
		//     any further.
		// --> size_trimmed <= 2
		//     Note that a (trimmed) linear tree (or path graph) has two
		//     leaves. This means that the first condition is true. However,
		//     this does not mean we have calculated the centre because
		//     there still is a big amount of leaves to trim. Therefore, we
		//     need a trimmed tree of at most 2 vertices to finish.
		return (l0 == 1 or l0 == 2) and l1 == 0 and size_trimmed <= 2;
	}
	);

	// unique centre
	bool has_single_center = false;
	node single_center = n + 1;

	bfs.set_process_visited_neighbours(true);
	bfs.set_process_neighbour(
	[&](const auto&, node s, node t, bool) -> void {
		// ignore the edge if one of its vertices has already been trimmed out.
		if (trimmed_degree[s] == 0) { return; }
		if (trimmed_degree[t] == 0) { return; }

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
				// ll0 <- ll1
				// ll1 <- 0
				std::swap(l0, l1);
			}
		}
	}
	);

	// add the next vertex only if its degree
	// (in the trimmed tree) is exactly one.
	bfs.set_vertex_add(
	[&](const auto&, node s) -> bool { return (trimmed_degree[s] == 1); }
	);

	// do the bfs from the leaves inwards
	bfs.start_at(tree_leaves);

	if (has_single_center) {
#if defined DEBUG
		assert(size_trimmed == 1);
#endif
		return std::make_pair(single_center, n);
	}

	// ---------------------------------------------------
	// -- reset the bfs
	bfs.reset();
	bfs.set_use_rev_edges(T.is_directed());

	// retrieve the two central vertices

	node v1, v2;
	v1 = v2 = n;

	bfs.set_process_current(
	[&](const auto&, node u) -> void {
		if (trimmed_degree[u] == 1) {
			if (v1 == n) { v1 = u; }
			else { v2 = u; }
		}
	}
	);
	bfs.start_at(x);
#if defined DEBUG
	assert(size_trimmed == 2);
#endif
	return std::make_pair(v1, v2);
}

} // -- namespace utils
} // -- namespace lal
