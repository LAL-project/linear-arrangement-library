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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/detail/macros/basic_convert.hpp>
#if defined __LAL_DEBUG_DMax_Unc_BnB
#include <lal/graphs/output.hpp>
#endif

// DMax includes
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

propagation_result AEF_BnB::propagate_constraints(const node u) noexcept
{
#if defined DEBUG
	assert(is_vertex_assigned(u));
#endif

	if (has_valid_LV_prediction(u)) {
		return propagation_result::success;
	}

	if (m_t.get_degree(u) == 1) {
		propagate_LV__antenna__from_leaf(u);
		return propagation_result::success;
	}

	if (m_t.get_degree(u) == 2) {
		const std::size_t path_u_idx = m_node_to_path_idx[u];
		const auto& path_u = m_paths_in_tree[path_u_idx];
		if (path_u.is_antenna(m_t)) {
			propagate_LV__antenna__from_internal(u);
		}
		else {
			const int64_t level_u = m_node_level[u];
			if (path_u.get_lowest_lexicographic() == u) {
				if (level_u == 2 or level_u == -2) {
					return propagate_LV__bridge__from_lowest__level_pm2(u);
				}
				else {
					propagate_LV__bridge__from_lowest__level_0(u);
				}
			}
			else {
				return propagate_LV__bridge__from_internal(u);
			}
		}
		return propagation_result::success;
	}

	for (const node v : m_t.get_neighbors(u)) {
		if (m_t.get_degree(v) >= 3) {
			continue;
		}

		const std::size_t path_v_idx = m_node_to_path_idx[v];
		const auto& path_v = m_paths_in_tree[path_v_idx];

		if (path_v.is_antenna(m_t)) {
			if (m_path_info[path_v_idx].num_assigned_nodes == 0) {
				// if any vertex has ever been assigned, then propagation
				// already happened.

				// propagate freely! :)
				propagate_LV__antenna__from_hub(u, v);
			}
			else {
				// emulate the propagation --> do not actually propagate since
				// the propagation has already been done

#if defined DEBUG
				assert(has_valid_LV_prediction(v));
				assert(
					m_predicted_LV__origin[u] !=
					LV_propagation_origin::antenna_hub
				);
#endif

				const int64_t dv = to_int64(m_t.get_degree(v));

				if (not is_vertex_assigned(v)) {
					// We only need to check that its prediction is equal to
					// -(degree(v)). If it is not, then the prediction of the
					// level values of the remaining vertices will also be in
					// conflict, and the placement of 'u' is wrong.

					if (m_predicted_LV[v] != -dv) {
						return propagation_result::
							conflict_LV_emulated_propagation;
					}
				}
				else {
					// There is no need to check the level value prediction.
					// If 'v' is assigned then all the constraints were satisfied
					// including the constraint that its level value prediction
					// must match the level value it was going to be assigned.
				}
			}
		}
		else {
#if defined DEBUG
			const node h1 = path_v.get_h1();
			const node h2 = path_v.get_h2();
			assert(u == h1 or u == h2);
#endif
			const auto r = propagate_LV__bridge__from_hub(u, path_v_idx);
			if (r != propagation_result::success) {
				return r;
			}
		}
	}

	return propagation_result::success;
}

} // namespace unconstrained
} // namespace DMax
} // namespace detail
} // namespace lal
