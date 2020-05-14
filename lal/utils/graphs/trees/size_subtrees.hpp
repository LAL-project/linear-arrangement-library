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
#include <vector>

// lal includes
#include <lal/graphs/rtree.hpp>

namespace lal {
namespace utils {

/*
 * @brief Calculate the size of every subtree of tree @e t.
 *
 * The method starts calculating the sizes at node @e r.
 * The method assumes that the vector @e sizes is initialised appropriately
 * (e.g., it has the right size).
 * This method is different from @ref get_undirected_size_subtrees because
 * it follows reversed edges.
 * @param t Input rooted tree.
 * @param r Start calculating sizes of subtrees at this node.
 * @param vis Mark nodes as visited as the algorithm goes on.
 * @param sizes The size of the subtree rooted at every reachable node
 * from @e r.
 */
inline void get_size_subtrees_follow_reversed(
	const graphs::rtree& t, node r,
	std::vector<bool>& vis, std::vector<uint32_t>& sizes
)
{
	sizes[r] = 1;
	vis[r] = true;
	for (node u : t.get_neighbours(r)) {
		if (not vis[u]) {
			get_size_subtrees_follow_reversed(t, u, vis, sizes);
			sizes[r] += sizes[u];
		}
	}
	for (node u : t.get_in_neighbours(r)) {
		if (not vis[u]) {
			get_size_subtrees_follow_reversed(t, u, vis, sizes);
			sizes[r] += sizes[u];
		}
	}
}

/*
 * @brief Calculate the size of every subtree of tree @e t.
 *
 * The method starts calculating the sizes at node @e r.
 * The method assumes that the vector @e sizes is initialised appropriately
 * (e.g., it has the right size).
 * @param t Input rooted tree.
 * @param r Start calculating sizes of subtrees at this node.
 * @param vis Mark nodes as visited as the algorithm goes on.
 * @param sizes The size of the subtree rooted at every reachable node
 * from @e r.
 */
inline void get_size_subtrees(
	const graphs::rtree& t, node r,
	std::vector<bool>& vis, std::vector<uint32_t>& sizes
)
{
	sizes[r] = 1;
	vis[r] = true;
	for (node u : t.get_neighbours(r)) {
		if (not vis[u]) {
			get_size_subtrees(t, u, vis, sizes);
			sizes[r] += sizes[u];
		}
	}
}

} // -- namespace utils
} // -- namespace lal
