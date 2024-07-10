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
#include <cassert>
#if defined __LAL_DEBUG_DMax_Unc_BnB
#include <iostream>
#endif
#endif

// lal includes
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/graphs/traversal.hpp>
#include <lal/detail/sorting/counting_sort.hpp>

#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

#define my_abs_diff(a,b) ((a) < (b) ? (b) - (a) : (a) - (b))
#define edge_sorted_by_index(u,v) (u < v ? edge{u,v} : edge{v,u})

int AEF_BnB::process_end
(const uint64_t D, const position pos)
noexcept
{
	if (pos < m_n_nodes) { return process_end_result::did_not_reach_end; }

#if defined __LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Reached end of the arrangement\n";
#endif

#if defined DEBUG
	assert(
	std::all_of(
		m_is_node_assigned.begin(), m_is_node_assigned.end(),
		[](char _p) { return _p == VERTEX_ASSIGNED; }
	)
	);

	assert(m_cut_values[m_n_nodes - 1] == 0);

	{
	uint64_t sum_cuts = 0;
	uint64_t _D = 0;
	uint64_t current_cut = 0;
	for (position_t i = 0ull; i < m_n_nodes; ++i) {
		current_cut += m_node_right_degree[m_arr[i]];
		current_cut -= m_node_left_degree[m_arr[i]];
		_D += current_cut;
		assert(m_cut_values[*i] == current_cut);
		sum_cuts += m_cut_values[*i];
	}

#if defined __LAL_DEBUG_DMax_Unc_BnB
	std::cout
		<< tab() << "sum by cuts= " << _D << '\n'
		<< tab() << "D_current=   " << D << '\n';
#endif

	assert(_D == sum_cuts);
	assert(_D == D);
	}
#endif

	m_max_arrs.add(D, m_arr);

	const bool reached_maximum = m_max_arrs.get_max_value() == D;
	if (reached_maximum) {
		return process_end_result::reached_end | process_end_result::found_max;
	}
	return process_end_result::reached_end;
}

void AEF_BnB::update_state
(const node u, const position_t pos, uint64_t& D_p, uint64_t& D_ps_m)
noexcept
{
#if defined __LAL_DEBUG_DMax_Unc_BnB
	std::cout
		<< tab()
		<< "Trying vertex " << u
		<< " at position " << pos
		<< '\n';
#endif

	// add vertex to the arrangement
	m_is_node_assigned[u] = VERTEX_ASSIGNED;
	m_arr.assign(u, pos);

	// update count of vertex colors
	m_num_assigned_nodes_blue += m_vertex_colors[u] == properties::bipartite_graph_coloring::blue;
	m_num_assigned_nodes_red += m_vertex_colors[u] == properties::bipartite_graph_coloring::red;

	// iterate over the neighbors of 'u'
	for (const node v : m_t.get_neighbors(u)) {
		const edge e = edge_sorted_by_index(u,v);

		// keep track of assigned and unassigned
		// neighbors of the neighbors of 'u'
		--m_num_unassigned_neighbors[v];
		++m_num_assigned_neighbors[v];
#if defined DEBUG
		assert(
			m_num_unassigned_neighbors[v] + m_num_assigned_neighbors[v]
			==
			m_t.get_degree(v)
		);
#endif

		if (is_vertex_assigned(v)) {
			const position pv = m_arr[node_t{v}];

#if defined DEBUG
			// just in case, you never know!
			assert(pv < pos);
#endif

			// update D_p
			D_p += *pos - pv;

			// edge is in E_p -> move it
			m_E_p.add(e);
			m_E_ps.remove(e);

			// update D_ps^-: substract the length of the edge that goes to E_p
			D_ps_m -= *pos - pv;
		}
		else {
			// update left and right directional degrees
			++m_node_right_degree[u];
			++m_node_left_degree[v];

			// edge is in E_s -> move it
			m_E_ps.add(e);
			m_E_s.remove(e);

			// add 'v' to the border vertex set, if appropriate
			m_border_nodes.add(v);
		}
	}

	// remove 'u' from the border vertex set
	m_border_nodes.remove(u);

	// update D_ps^-: add a unit of length for every edge in E_ps
	D_ps_m += m_E_ps.size();

	// update vertex's level
	m_node_level[u] =
		to_int64(m_node_right_degree[u]) - to_int64(m_node_left_degree[u]);

	// update this vertex's path information
	if (m_t.get_degree(u) <= 2) {
		const auto idx = m_node_to_path_idx[u];
		if (m_node_level[u] == 0) {
#if defined DEBUG
			// If this path was an antenna we could not possibly have this vertex
			// as a thistle at this point of the algorithm.
			assert(not m_paths_in_tree[m_node_to_path_idx[u]].is_antenna(m_t));
#endif
			++m_path_info[idx].num_thistles;
		}
		else if (m_node_level[u] == 2) {
			++m_path_info[idx].num_assigned_nodes_p2;
		}
		else if (m_node_level[u] == -2) {
			++m_path_info[idx].num_assigned_nodes_m2;
		}
		++m_path_info[idx].num_assigned_nodes;
	}

	if (pos > 0ull) {
		m_cut_values[*pos] =
			m_cut_values[*pos - 1]
			+ m_node_right_degree[u]
			- m_node_left_degree[u];
	}
	else {
		m_cut_values[*pos] = m_t.get_degree(u);
	}

#if defined DEBUG
	for (node v = 0; v < m_n_nodes; ++v) {

#if defined __LAL_DEBUG_DMax_Unc_BnB
		if (is_vertex_assigned(v)) {
			std::cout << tab()
					  << "    "
					  << "degree[" << v << "]: "
					  << m_t.get_degree(v)
					  << " (-"
					  << m_node_left_degree[v]
					  << ",+"
					  << m_node_right_degree[v]
					  << ")"
					  << (is_vertex_thistle(v) ? "  thistle!" : "")
					  << '\n';
		}
#endif

		if (is_vertex_assigned(v)) {
			assert(
				m_node_left_degree[v] + m_node_right_degree[v]
				==
				m_t.get_degree(v)
			);
		}
	}

	for (position_t p = 1ull; p <= pos; ++p) {
		assert(m_node_level[m_arr[p - 1ull]] >= m_node_level[m_arr[p]]);
	}
#endif
}

void AEF_BnB::recover_state(const position_t pos) noexcept {
	const node u = m_arr[pos];

#if defined __LAL_DEBUG_DMax_Unc_BnB
	std::cout
		<< tab()
		<< "Remove vertex " << u
		<< " from position " << pos
		<< '\n';
#endif

	// update count of vertex colors
	m_num_assigned_nodes_blue -= m_vertex_colors[u] == properties::bipartite_graph_coloring::blue;
	m_num_assigned_nodes_red -= m_vertex_colors[u] == properties::bipartite_graph_coloring::red;

	// update this vertex's path information
	if (m_t.get_degree(u) <= 2) {
		const auto idx = m_node_to_path_idx[u];
		if (m_node_level[u] == 0) {
			--m_path_info[idx].num_thistles;
		}
		else if (m_node_level[u] == 2) {
			--m_path_info[idx].num_assigned_nodes_p2;
		}
		else if (m_node_level[u] == -2) {
			--m_path_info[idx].num_assigned_nodes_m2;
		}
		--m_path_info[idx].num_assigned_nodes;
	}

	// update vertex's level
	m_node_level[u] = 0;
	m_node_right_degree[u] = 0;

	for (node v : m_t.get_neighbors(u)) {
		const edge e = edge_sorted_by_index(u,v);

		// keep track of assigned and unassigned
		// neighbors of the neighbors of 'u'
		++m_num_unassigned_neighbors[v];
		--m_num_assigned_neighbors[v];
#if defined DEBUG
		assert(
			m_num_unassigned_neighbors[v] + m_num_assigned_neighbors[v]
			==
			m_t.get_degree(v)
		);
#endif

		if (is_vertex_assigned(v)) {

#if defined DEBUG
			// just in case, you never know!
			const position pv = m_arr[node_t{v}];
			assert(pv < pos);
			assert(m_node_right_degree[v] > 0);
#endif

			// edge is in E_p -> move it
			m_E_p.remove(e);
			m_E_ps.add(e);
		}
		else {
#if defined DEBUG
			assert(m_node_left_degree[v] > 0);
#endif

			// update directional degree
			--m_node_left_degree[v];

			// edge is in E_s -> move it
			m_E_ps.remove(e);
			m_E_s.add(e);

			// remove 'v' from the border vertex set, if appropriate
			if (m_num_assigned_neighbors[v] == 0) {
				m_border_nodes.remove(v);
			}
		}
	}

	// add 'u' to the border vertex set, if appropriate
	if (m_num_assigned_neighbors[u] > 0) {
		m_border_nodes.add(u);
	}

	// remove vertex from the arrangement
	m_is_node_assigned[u] = VERTEX_UNASSIGNED;

#if defined DEBUG
	for (node v = 0; v < m_n_nodes; ++v) {
#if defined __LAL_DEBUG_DMax_Unc_BnB
		if (is_vertex_assigned(v)) {
			std::cout
				<< tab()
				<< "    "
				<< "degree[" << v << "]: "
				<< m_t.get_degree(v)
				<< " (-"
				<< m_node_left_degree[v]
				<< ",+"
				<< m_node_right_degree[v]
				<< ")\n";
		}
#endif

		if (is_vertex_assigned(v)) {
			assert(
				m_node_left_degree[v] + m_node_right_degree[v]
				==
				m_t.get_degree(v)
			);
		}
	}
#endif
}

} // -- namespace unconstrained
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
