/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
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
 *     Juan Luis Esteban (esteban@cs.upc.edu)
 *         LOGPROG: Logics and Programming Research Group
 *         Office 110, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://www.cs.upc.edu/~esteban/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// DMax includes
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>
#include <lal/detail/macros/basic_convert.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

reason_discard AEF_BnB::check_propagation_node_to_node(
	const node u, const int64_t level_u, const node v, const int64_t level_v
) const noexcept
{
	const bool both_negative = level_u < 0 and level_v < 0;
	const bool both_positive = level_u > 0 and level_v > 0;
	if (both_positive or both_negative) {
		if (m_vertex_colors[u] != m_vertex_colors[v]) {
			return reason_discard::placement_fails_level_propagation;
		}
	}
	else {
		if (m_vertex_colors[u] == m_vertex_colors[v]) {
			return reason_discard::placement_fails_level_propagation;
		}
	}
	return reason_discard::none;
}

reason_discard AEF_BnB::discard_node__degree_2__bridge__level_0(const node u
) const noexcept
{
	const auto path_idx = m_node_to_path_idx[u];
	const auto& path = m_paths_in_tree[path_idx];
	const node w = path.get_lowest_lexicographic();

#if defined DEBUG
	assert(
		is_vertex_assigned(m_t.get_neighbors(u)[0]) or
		is_vertex_assigned(m_t.get_neighbors(u)[1])
	);
	assert(
		not(is_vertex_assigned(m_t.get_neighbors(u)[0]) and
			is_vertex_assigned(m_t.get_neighbors(u)[1]))
	);

	assert(not m_paths_in_tree[path_idx].is_antenna(m_t));
#endif

	if (w != u) {
		return reason_discard::thistle_in_bridge_is_not_the_lowest;
	}

#if defined DEBUG
	const std::size_t pw = path.get_position(w);
	const node wm1 = path[pw - 1];
	const node wp1 = path[pw + 1];
	if (has_valid_LV_prediction(wm1) and has_valid_LV_prediction(wp1)) {
		// this should never be reached
		assert(has_valid_LV_prediction(w));
		assert(m_predicted_LV[w] == 0);
		assert(false);
	}
#endif

	return reason_discard::none;
}

reason_discard AEF_BnB::discard_node__degree_2__bridge__level_pm2(
	const node u, const int64_t level_u
) const noexcept
{
#if defined DEBUG
	assert(std::abs(level_u) == 2);
	assert(not has_valid_LV_prediction(u));
#endif

	const auto path_idx = m_node_to_path_idx[u];
#if defined DEBUG
	const auto& info = m_path_info[path_idx];
#endif
	const auto& path = m_paths_in_tree[path_idx];

	// w:= loWest lexicographic
	const node w = path.get_lowest_lexicographic();
	const std::size_t pw = path.get_position(w);

	const node wm1 = path[pw - 1];
	const bool valid_prediction_wm1 = has_valid_LV_prediction(wm1);
	const bool valid_prediction_w = has_valid_LV_prediction(w);
	const node wp1 = path[pw + 1];
	const bool valid_prediction_wp1 = has_valid_LV_prediction(wp1);

	// simple case
	if (u == w) {
		if (valid_prediction_wm1 and m_predicted_LV[wm1] != -level_u) {
			return reason_discard::placement_fails_level_propagation;
		}
		if (valid_prediction_wp1 and m_predicted_LV[wp1] != -level_u) {
			return reason_discard::placement_fails_level_propagation;
		}

#if defined DEBUG
		if (not valid_prediction_wm1 and not valid_prediction_wp1) {
			assert(info.num_assigned_nodes == 0);
		}
#endif

		return reason_discard::none;
	}

#if defined DEBUG
	assert(u != w);
	assert(not has_valid_LV_prediction(u));
#endif

	// we cannot decide anything if the lowest has no valid prediction
	if (not valid_prediction_w) {
		return reason_discard::none;
	}

#if defined DEBUG
	assert(m_predicted_LV[w] == 0);
#endif

	const std::size_t pu = path.get_position(u);
	if (pu < pw) {
#if defined DEBUG
		assert(not valid_prediction_wm1);
#endif

		if (not valid_prediction_wp1) {
			return reason_discard::none;
		}

#if defined DEBUG
		assert(std::abs(m_predicted_LV[wp1]) == 2);
#endif

		const int64_t predicted_level_wm1 = m_predicted_LV[wp1] == 2 ? -2 : 2;
		return check_propagation_node_to_node(
			u, level_u, wm1, predicted_level_wm1
		);
	}

	if (pw < pu) {
#if defined DEBUG
		assert(not valid_prediction_wp1);
#endif

		if (not valid_prediction_wm1) {
			return reason_discard::none;
		}

#if defined DEBUG
		assert(std::abs(m_predicted_LV[wm1]) == 2);
#endif

		const int64_t predicted_level_wp1 = m_predicted_LV[wm1] == 2 ? -2 : 2;
		return check_propagation_node_to_node(
			u, level_u, wp1, predicted_level_wp1
		);
	}

	return reason_discard::none;
}

reason_discard AEF_BnB::discard_node_degree_2(
	const node u, const int64_t level_u
) const noexcept
{
	const std::size_t path_idx = m_node_to_path_idx[u];
	const auto& path = m_paths_in_tree[path_idx];

	if (path.is_antenna(m_t)) {
		// no need to check level values +-2
		if (level_u == 0) {
			return reason_discard::node_of_antenna_as_thistle;
		}
	}
	else {
		if (level_u == 2 or level_u == -2) {
			const reason_discard r =
				discard_node__degree_2__bridge__level_pm2(u, level_u);
			if (r != reason_discard::none) {
				return r;
			}
		}
		else {
#if defined DEBUG
			assert(level_u == 0);
#endif
			const reason_discard r = discard_node__degree_2__bridge__level_0(u);
			if (r != reason_discard::none) {
				return r;
			}
		}
	}
	return reason_discard::none;
}

reason_discard AEF_BnB::discard_node_degree_3(
	const node u, const int64_t level_u
) const noexcept
{
	for (const node v : m_t.get_neighbors(u)) {
		if (m_t.get_degree(v) >= 3) {
			continue;
		}

		const std::size_t path_v_idx = m_node_to_path_idx[v];
		const auto& path_v = m_paths_in_tree[path_v_idx];
		const std::size_t N = path_v.get_num_nodes();

		if (path_v.is_antenna(m_t)) {
			if (m_path_info[path_v_idx].num_assigned_nodes == 0) {
				if ((N == 2 and level_u <= -1) or (N == 3 and level_u <= 0) or
					(N >= 4 and level_u <= 1)) {
					return reason_discard::hub_disallows_placement_of_antennas;
				}
			}
			else {
#if defined DEBUG
				assert(has_valid_LV_prediction(v));
#endif

				if (not is_vertex_assigned(v) and (m_predicted_LV[v] != -2)) {
					return reason_discard::placement_fails_level_propagation;
				}
			}
		}
		else {
			if (not is_vertex_assigned(v) and has_valid_LV_prediction(v) and
				v != path_v.get_lowest_lexicographic() and
				(m_predicted_LV[v] != -2)) {
				return reason_discard::placement_fails_level_propagation;
			}
		}
	}
	return reason_discard::none;
}

reason_discard
AEF_BnB::discard_vertex(const node u, const position_t pos) const noexcept
{
#if defined DEBUG
	assert(pos > 0ull);
#endif

	{
		// do not let the arrangement become bipartite
		const auto color_u = m_vertex_colors[u];
		const auto new_num_assigned_nodes_blue =
			m_num_assigned_nodes_blue +
			(color_u == properties::bipartite_graph_coloring::blue);
		const auto new_num_assigned_nodes_red =
			m_num_assigned_nodes_red +
			(color_u == properties::bipartite_graph_coloring::red);

		if ((new_num_assigned_nodes_blue == m_num_nodes_blue and
			 new_num_assigned_nodes_red == 0) or
			(new_num_assigned_nodes_red == m_num_nodes_red and
			 new_num_assigned_nodes_blue == 0)) {
			return reason_discard::will_produce_bipartite_arrangement;
		}
	}

	// Calculate the level of an unassigned node. Since it is unassigned,
	// we do not know its right directional degree. Calculate it with
	//   left + right = degree  =>  right = degree - left
	//   level = right - left
	//         = degree - 2*left
	const int64_t level_u =
		to_int64(m_t.get_degree(u)) - 2 * to_int64(m_node_left_degree[u]);

	const node previous_node = m_arr[pos - 1ull];
	const int64_t previous_level = m_node_level[previous_node];
	if (previous_level < level_u) {
		return reason_discard::level_signature_will_not_be_nonincreasing;
	}

	if (m_node_level[previous_node] == level_u and previous_node > u) {
		return reason_discard::nodes_of_equal_level_disobey_lexicographic_order;
	}

	for (node v : m_t.get_neighbors(u)) {
		if (is_vertex_assigned(v)) {
			if (m_node_level[v] <= level_u) {
				return reason_discard::adjacent_vertices_with_equal_level_value;
			}
		}
		else {
			const auto dv = to_int64(m_t.get_degree(v));
			if (level_u < -dv) {
				return reason_discard::node_disallows_placement_of_neighbors;
			}
		}
	}

	if (has_valid_LV_prediction(u) and level_u != m_predicted_LV[u]) {
		return reason_discard::placement_is_in_conflict_with_level_prediction;
	}

	if (level_u < -2) {
		// -3, -4, -5, ...
		for (std::size_t i = 0; i < m_paths_in_tree.size(); ++i) {
			const properties::branchless_path& p = m_paths_in_tree[i];
			const path_info& info = m_path_info[i];

			if (info.num_assigned_nodes == 0) {
				return reason_discard::missing_entire_path;
			}

			if (p.is_antenna(m_t)) {
				// check leaf is assigned
				const node h1 = p.get_h1();
				const node h2 = p.get_h2();
				const node leaf = m_t.get_degree(h1) == 1 ? h1 : h2;
				if (not is_vertex_assigned(leaf)) {
					return reason_discard::missing_degree1;
				}

				// check enough +2 are assigned
				if ((info.nodes_p2_to_assign.has_value() and
					 info.nodes_p2_to_assign > info.num_assigned_nodes_p2) or
					(info.num_assigned_nodes_p2 < info.min_pm_two)) {
					return reason_discard::missing_degree2_lp2;
				}

				// check enough -2 are assigned
				if ((info.nodes_m2_to_assign.has_value() and
					 info.nodes_m2_to_assign > info.num_assigned_nodes_m2) or
					(info.num_assigned_nodes_m2 < info.min_pm_two)) {
					return reason_discard::missing_degree2_lm2;
				}
			}
			else {
				// check enough +2 are assigned
				if (info.num_assigned_nodes_p2 < info.min_pm_two) {
					return reason_discard::missing_degree2_lp2;
				}

				// check enough -2 are assigned
				if (info.num_assigned_nodes_m2 < info.min_pm_two) {
					return reason_discard::missing_degree2_lm2;
				}
			}
		}
	}
	else if (level_u < -1) {
		// -1, -2, -3, -4, -5, ...
		const std::size_t path_u_idx = m_node_to_path_idx[u];
		for (std::size_t i = 0; i < m_paths_in_tree.size(); ++i) {
			if (i == path_u_idx) {
				continue;
			}

			const properties::branchless_path& p = m_paths_in_tree[i];
			const path_info& info = m_path_info[i];

			if (p.is_antenna(m_t)) {

				// check leaf is assigned
				const node h1 = p.get_h1();
				const node h2 = p.get_h2();
				const node leaf = m_t.get_degree(h1) == 1 ? h1 : h2;
				if (not is_vertex_assigned(leaf)) {
					return reason_discard::missing_degree1;
				}

				// check enough +2 are assigned
				if ((info.nodes_p2_to_assign.has_value() and
					 info.nodes_p2_to_assign > info.num_assigned_nodes_p2) or
					(info.num_assigned_nodes_p2 < info.min_pm_two)) {
					return reason_discard::missing_degree2_lp2;
				}
			}
			else {
				// check enough +2 are assigned
				if (info.num_assigned_nodes_p2 < info.min_pm_two) {
					return reason_discard::missing_degree2_lp2;
				}
			}
		}
	}
	else if (level_u < 2) {
		// +1, 0, -1, -2, -3, -4, -5, ...
		const std::size_t path_u_idx = m_node_to_path_idx[u];
		for (std::size_t i = 0; i < m_paths_in_tree.size(); ++i) {
			if (i == path_u_idx) {
				continue;
			}

			const properties::branchless_path& p = m_paths_in_tree[i];
			const path_info& info = m_path_info[i];

			if (p.is_antenna(m_t)) {

				// check enough +2 are assigned
				if ((info.nodes_p2_to_assign.has_value() and
					 info.nodes_p2_to_assign > info.num_assigned_nodes_p2) or
					(info.num_assigned_nodes_p2 < info.min_pm_two)) {
					return reason_discard::missing_degree2_lp2;
				}
			}
			else {
				// check enough +2 are assigned
				if (info.num_assigned_nodes_p2 < info.min_pm_two) {
					return reason_discard::missing_degree2_lp2;
				}
			}
		}
	}

	if (not has_valid_LV_prediction(u)) {
		if (m_t.get_degree(u) == 2) {
			const reason_discard r = discard_node_degree_2(u, level_u);
			if (r != reason_discard::none) {
				return r;
			}
		}
		else if (m_t.get_degree(u) >= 3) {
			const reason_discard r = discard_node_degree_3(u, level_u);
			if (r != reason_discard::none) {
				return r;
			}
		}
	}

	if (previous_level > 0 and level_u <= 0 and pos < m_n_nodes - 1) {
		// the current level is past the largest cut.
		const uint64_t largest_cut = m_cut_values[*pos - 1];
		if (largest_cut < (m_n_nodes - 1) / 2) {
			return reason_discard::largest_cut_below_minimum;
		}
	}

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

		if (not all_lower_leaves_assigned) {
			return reason_discard::node_leaves_disobey_lexicographic_order;
		}
	}

	// avoid symmetries in isomorphic subtrees
	if (m_rt.get_num_nodes() > 0 and m_rt.get_in_degree(u) > 0) {
		const std::size_t idx_orbit_u = m_node_to_orbit[u];
		const auto& orbit_u = m_orbits[idx_orbit_u];

		// check that 'u' is the lowest-indexed node
		// among its isomorphic siblings

		bool all_lower_nodes_assigned = true;
		for (node w : orbit_u) {
			// ensure that 'u' and 'w' are siblings
			if (not m_rt.are_nodes_siblings(u, w)) {
				continue;
			}

			if (w < u and not is_vertex_assigned(w)) {
				all_lower_nodes_assigned = false;
				break;
			}
		}

		// ignore if some lower-indexed vertex has not yet been assigned
		if (not all_lower_nodes_assigned) {
			return reason_discard::
				roots_of_isomorphic_subtrees_disobey_lexicographic_order;
		}
	}

	return reason_discard::none;
}

} // namespace unconstrained
} // namespace DMax
} // namespace detail
} // namespace lal
