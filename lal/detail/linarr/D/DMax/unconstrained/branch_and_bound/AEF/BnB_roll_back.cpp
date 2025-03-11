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
#if defined LAL_DEBUG_DMax_Unc_BnB
#include <lal/graphs/output.hpp>
#endif

// DMax includes
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

void AEF_BnB::roll_back_constraints(const node u) noexcept
{
#if defined DEBUG
	assert(is_vertex_assigned(u));
#endif

	if (not has_valid_LV_prediction(u)) {
		return;
	}
	if (not is_node_a_trigger_of_LV(u)) {
		return;
	}

	if (m_t.get_degree(u) == 1) {
		roll_back_LV_antenna(m_node_to_path_idx[u]);
		return;
	}

	if (m_t.get_degree(u) == 2) {
		const std::size_t path_u_idx = m_node_to_path_idx[u];
		const auto& path_u = m_paths_in_tree[path_u_idx];
		if (path_u.is_antenna(m_t)) {
			roll_back_LV_antenna(path_u_idx);
		}
		else {
			const int64_t level_u = m_node_level[u];
			if (path_u.get_lowest_lexicographic() == u) {
				if (level_u == 2 or level_u == -2) {
					roll_back_LV_bridge_from_lowest_level_pm2(u);
				}
				else {
					roll_back_LV_bridge_from_lowest_level_0(u);
				}
			}
			else {
				roll_back_LV_bridge_from_internal(u);
			}
		}
		return;
	}

	for (const node v : m_t.get_neighbors(u)) {
		if (m_t.get_degree(v) >= 3) {
			continue;
		}

		const std::size_t path_v_idx = m_node_to_path_idx[v];
		const auto& path_v = m_paths_in_tree[path_v_idx];

		if (path_v.is_antenna(m_t)) {
			if (m_predicted_LV_origin[v] ==
				LV_propagation_origin::antenna_hub) {
				roll_back_LV_antenna(path_v_idx);
			}
		}
		else {
#if defined DEBUG
			const node h1 = path_v.get_h1();
			const node h2 = path_v.get_h2();
			assert(u == h1 or u == h2);
#endif
			roll_back_LV_bridge_from_hub(u, path_v_idx);
		}
	}

	m_predicted_LV_origin[u] = LV_propagation_origin::none;
}

} // namespace unconstrained
} // namespace DMax
} // namespace detail
} // namespace lal
