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

// C++ includes
#if defined DEBUG
#if defined __LAL_DEBUG_DMax_Unc_BnB
#include <iostream>
#endif
#endif
#include <vector>

// DMax includes
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>
#include <lal/detail/macros/basic_convert.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

bool AEF_BnB::discard_vertex
([[maybe_unused]] const node u, [[maybe_unused]] const position_t pos)
const noexcept
{
	if (is_vertex_assigned(u)) { return true; }

	{
	// do not let the arrangement become bipartite
	const auto color_u = m_vertex_colors[u];
	const auto new_num_assigned_verts_blue =
		m_num_assigned_verts_blue + (color_u == properties::bipartite_graph_coloring::blue);
	const auto new_num_assigned_verts_red =
		m_num_assigned_verts_red + (color_u == properties::bipartite_graph_coloring::red);

	if (
		(new_num_assigned_verts_blue == m_num_verts_blue
		 and
		 new_num_assigned_verts_red == 0)
		or
		(new_num_assigned_verts_red == m_num_verts_red
		 and
		 new_num_assigned_verts_blue == 0)
	)
	{
#if defined __LAL_DEBUG_DMax_Unc_BnB
		std::cout << tab() << "A bipartite arrangement is to be constructed.\n";
		std::cout << tab() << "But we already have one! Bound!!\n";
#endif
		return true;
	}
	}

	// REFERENCES:
	// [0] Our own contribution
	// [obvious] Our own contribution, but it's obvious. Unsaucable
	// [1] Nurse's paper and/or Master thesis
	//

	/*
	 * ensure that:
	 * (1) [1] the level sequence is non-increasing and that
	 *
	 * (2) [1] there are no adjacent vertices (neighbors in the tree!) with
	 * the same level value
	 *
	 * (3) [1] vertices of equal level value should be sorted by index
	 * (any permutation whatsoever will yield the same value)
	 *
	 * (4) [1] the largest cust must be at least (n - 1)/2
	 *
	 * (5) [0] all (k,h,g)-path (where d(h) >= 1 and d(g) >= 1) have at most one
	 * thistle vertex (level 0)
	 *
	 * (5.1) [0] all (k,h,g)-path (where d(h) >= 1 and d(g) >= 1) have at most one
	 * thistle vertex (level 0) and that vertex can be any of the internal
	 * vertices in the path; we choose the lowest in the lexicographical order.
	 *
	 * (6) [0] all (k,h,g)-path (where d(h) >= 1 and d(g) == 1) have zero thistle
	 * vertex (level 0)
	 *
	 * (7) [obvious] Every vertex u must satisfy the following inequality
	 *
	 *		min_{v neighbour to the left of u} { level(v) }
	 *			>
	 *		level(u)
	 *			>
	 *		max_{v neighbour to the right of u} { level(v) }
	 */
	if (pos > 0ull) {
		// Calculate the level of an unassigned node. Since it is unassigned,
		// we do not know its right directional degree. Calculate it with
		//   left + right = degree  =>  right = degree - left
		//   level = right - left
		//         = degree - 2*left
		const int64_t level_u =
			to_int64(m_t.get_degree(u)) - 2*to_int64(m_node_left_degree[u]);

		// (5), (6)
		if (m_t.get_degree(u) == 2 and level_u == 0) {
			const auto idx = m_node_to_path_idx[u];
			const auto& path_of_u = m_paths_in_tree[idx];
			const uint64_t min_deg_hubs =
				std::min(
					m_t.get_degree(path_of_u.get_h1()),
					m_t.get_degree(path_of_u.get_h2())
				);

			// (6) --> trying to place a vertex of a leaf-ended path
			// as a thistle of level 0! Do not let this happen!
			if (min_deg_hubs == 1) { return true; }

			// 'min_deg_hubs' cannot be equal to 2!

			if (min_deg_hubs >= 3) {
				// (5) --> trying to place a vertex of a non-leaf-ended path
				// as a thistle of level 0. Do not let this happen if the
				// path already contains a thistle.
				if (m_num_thistle_per_path[idx] == 1) { return true; }

				// (5.1) --> trying to place a vertex of a non-leaf-ended path
				// as a thistle of level 0. Allow only the lowest vertex in
				// the lexicographical order.
				if (m_num_thistle_per_path[idx] == 0) {
					// Since 'u' has degree 2, its path cannot be empty and
					// so the 'lowest_lexicographic' exists.
					// Check: if 'u' is not the first vertex, then discard it.
					if (path_of_u.get_lowest_lexicographic() != u) { return true; }
				}
			}
		}

		// (1)
		const node previous_node = m_arr[pos - 1ull];
		const int64_t previous_level = m_node_level[previous_node];
		if (previous_level < level_u) { return true; }

		// (4)
		if (previous_level > 0 and level_u <= 0 and pos < m_n_nodes - 1) {
			// the current level is past the largest cut.

			const uint64_t largest_cut = m_cut_values[*pos - 1];
			if (largest_cut < (m_n_nodes - 1)/2) { return true; }
		}

		// (3)
		if (m_node_level[previous_node] == level_u) {
			if (previous_node > u) { return true; }
		}

		// (2,7)
		for (node v : m_t.get_neighbors(u)) {

			if (is_vertex_assigned(v)) {

				// (2,7)
				// Notice that (>) is already satisfied by ensuring that the
				// previous constraints are satisfied.
				// We cannot possible have (<) otherwise a previous check
				// would have failed. Nevertheless, it is added for safety.
				if (m_node_level[v] <= level_u) { return true; }
			}
			else {
				// (7)
				// If there is no possible configuration of the neighbors of
				// vertex 'u' to make a level value that is less than 'level_u'
				// then vertex 'u' is misplaced in the arrangement.
				const auto dv = to_int64(m_t.get_degree(v));
				if (level_u < -dv) { return true; }
			}
		}
	}

	// --------------------
	// SYMMETRY CONSTRAINTS

	if (m_t.get_degree(u) == 1) {
		// Vertex 'u' is a leaf. Here we are constraining the order of the
		// leaves in the arrangement so that they are placed in increasing
		// order of label because every permutation yield the same sum of
		// edge lengths

		const node p = leaf_parent(u);

		bool all_lower_leaves_assigned = true;
		for (node l : m_leaves[p]) {
			if (l < u and not is_vertex_assigned(l)) {
				all_lower_leaves_assigned = false;
				break;
			}
		}

		if (not all_lower_leaves_assigned) { return true; }
	}

	// avoid symmetries in isomorphic subtrees
	if (m_rt.get_num_nodes() > 0 and m_rt.get_in_degree(u) > 0) {
		const std::size_t idx_orbit_u = m_vertex_to_orbit[u];
		const auto& orbit_u = m_orbits[idx_orbit_u];

		// check that 'u' is the lowest-indexed node
		// among its isomorphic siblings

		bool all_lower_nodes_assigned = true;
		for (node w : orbit_u) {
			// ensure that 'u' and 'w' are siblings
			if (not m_rt.are_nodes_siblings(u, w)) { continue; }

			if (w < u and not is_vertex_assigned(w)) {
				all_lower_nodes_assigned = false;
				break;
			}
		}

		// ignore if some lower-indexed vertex has not yet been assigned
		if (not all_lower_nodes_assigned) { return true; }
	}

	return false;
}

} // -- namespace unconstrained
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
