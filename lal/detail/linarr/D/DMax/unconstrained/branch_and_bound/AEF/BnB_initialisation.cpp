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

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

AEF_BnB::AEF_BnB(
	const graphs::free_tree& t,
	const array<std::vector<node>>& leaves,
	// colors of vertices
	const properties::bipartite_graph_coloring& colors,
	const uint64_t num_verts_blue,
	const uint64_t num_verts_red,
	// paths
	const std::vector<properties::branchless_path>& paths_in_tree,
	const array<std::size_t>& node_to_path_idx,
	const array<std::vector<node>>& incident_antennas,
	// orbits
	const std::vector<std::vector<node>>& orbits,
	const array<std::size_t>& vertex_to_orbit
)
noexcept :
	m_t(t),
	m_max_arrs(t),
	m_n_nodes(t.get_num_nodes()),
	m_leaves(leaves),
	// colors of vertices
	m_vertex_colors(colors),
	m_num_nodes_blue(num_verts_blue),
	m_num_nodes_red(num_verts_red),
	// paths
	m_paths_in_tree(paths_in_tree),
	m_node_to_path_idx(node_to_path_idx),
	m_incident_antennas(incident_antennas),
	// orbits
	m_orbits(orbits),
	m_node_to_orbit(vertex_to_orbit)
{
}

void AEF_BnB::initialize
(const std::pair<uint64_t, linear_arrangement>& initial_DMax)
noexcept
{
	m_arr.resize(m_n_nodes);

	m_max_arrs.init();
	m_max_arrs.add(initial_DMax.first, initial_DMax.second);

	// -------------------------------------------------------------------------
	// Data used for upper bounds

	m_degree_count.resize(m_n_nodes, 0);
	m_num_assigned_neighbors.resize(m_n_nodes, 0);
	m_num_unassigned_neighbors.resize(m_n_nodes);
	for (node u = 0; u < m_n_nodes; ++u) {
		m_num_unassigned_neighbors[u] = m_t.get_degree(u);
	}

	m_border_nodes.init(m_n_nodes, m_n_nodes);
	m_sorting_memory.count.resize(m_n_nodes + 1);
	m_sorting_memory.output.resize(m_n_nodes);

	// -------------------------------------------------------------------------
	// Algorithm control

	m_num_assigned_nodes_blue = 0;
	m_num_assigned_nodes_red = 0;
	m_path_info.resize(m_paths_in_tree.size(), {0,0,0,0,0,0,{},{}});
	for (std::size_t i = 0; i < m_path_info.size(); ++i) {
		const auto& path = m_paths_in_tree[i];
		const uint64_t N_2 = path.get_num_nodes() - 2;
		if (path.is_antenna(m_t)) {
			m_path_info[i].min_pm_two = N_2/2;
			m_path_info[i].max_pm_two = N_2/2 + N_2%2;
		}
		else {
			if (N_2 == 0) {
				m_path_info[i].min_pm_two = 0;
				m_path_info[i].max_pm_two = 0;
			}
			else if (N_2 <= 2) {
				m_path_info[i].min_pm_two = 0;
				m_path_info[i].max_pm_two = 1;
			}
			else {
				m_path_info[i].min_pm_two = (N_2 + 1)/2 - 1;
				m_path_info[i].max_pm_two = (N_2 + 1)/2;
			}
		}
	}

	m_is_node_assigned.resize(m_n_nodes, VERTEX_UNASSIGNED);
	m_node_left_degree.resize(m_n_nodes, 0);
	m_node_right_degree.resize(m_n_nodes, 0);
	m_node_level.resize(m_n_nodes, 0);
	m_cut_values.resize(m_n_nodes, 0);

	m_predicted_LV.resize(m_n_nodes);
	m_predicted_LV__origin.resize(m_n_nodes, LV_propagation_origin::none);

	indexer_edge I_p, I_ps, I_s;
	I_p.init(m_n_nodes);
	I_ps.init(m_n_nodes);
	I_s.init(m_n_nodes);
	m_E_p.init(m_n_nodes, m_n_nodes*m_n_nodes, std::move(I_p));
	m_E_ps.init(m_n_nodes, m_n_nodes*m_n_nodes, std::move(I_ps));
	m_E_s.init(m_n_nodes, m_n_nodes*m_n_nodes, std::move(I_s));

	for (node u = 0; u < m_n_nodes; ++u) {
		// -----------
		// edges in E_s
		for (node v : m_t.get_neighbors(u)) {
			if (u < v) { m_E_s.add( {u,v} ); }
		}
	}
}

} // -- namespace unconstrained
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
