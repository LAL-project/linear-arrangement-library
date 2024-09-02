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
#if defined __LAL_DEBUG_DMax_Unc_BnB
#include <string_view>
#include <iostream>
#include <iomanip>
#endif
#endif

// lal includes
#include <lal/iterators/E_iterator.hpp>
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>

#if defined __LAL_DEBUG_DMax_Unc_BnB

//static constexpr std::string_view WHITE = "1";
//static constexpr std::string_view GRAY = "2";
static constexpr std::string_view RED = "31";
//static constexpr std::string_view GREEN = "32";
//static constexpr std::string_view YELLOW = "33";
static constexpr std::string_view BLUE = "34";
//static constexpr std::string_view PURPLE = "35";
//static constexpr std::string_view CYAN = "36";
//static constexpr std::string_view LIGHT_GRAY = "37";

#define COLOR(c)	\
std::string("\033[m\033[1;") + std::string(c) + std::string("m")

/*
#define DIM_COLOR(c)	\
	std::string("\033[m\033[2;") + std::string(c) + std::string("m")

#define ITALICS_COLOR(c)	\
	std::string("\033[m\033[3;") + std::string(c) + std::string("m")

#define BLINKING_COLOR(c)	\
	std::string("\033[m\033[6;") + std::string(c) + std::string("m")

#define HIGHLIGHT_COLOR(c)	\
	std::string("\033[m\033[7;") + std::string(c) + std::string("m")
*/

static constexpr std::string_view END_COLOR = "\033[0m";

#endif

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

static constexpr uint64_t column_width = 2;
#define set_col_width std::setw(column_width)

#if defined __LAL_DEBUG_DMax_Unc_BnB

void AEF_BnB::output_edge_list() const noexcept {
	iterators::E_iterator it(m_t);
	while (not it.end()) {
		const edge e = it.yield_edge();
		std::cout << e.first << ' ' << e.second << '\n';
	}
}

void AEF_BnB::output_arrangement() const noexcept {
	for (node_t u = 0ull; u < m_n_nodes; ++u) {
		std::cout << ' ';
		if (not is_vertex_assigned(*u)) {
			std::cout << set_col_width << 'x';
		}
		else {
			std::cout << set_col_width << m_arr[u];
		}
	}
}

void AEF_BnB::output_invarr(const position p) const noexcept {
	for (position_t i = 0ull; i < p; ++i) {
		std::cout
			<< ' '
			<< (COLOR(m_vertex_colors[m_arr[i]] == 0 ? BLUE : RED))
			<< set_col_width << m_arr[i]
			<< END_COLOR;
	}
	for (position i = p; i < m_n_nodes; ++i) {
		std::cout << ' ' << set_col_width << 'x';
	}
}

void AEF_BnB::output_degree_sequence(const position p) const noexcept {
	for (position_t i = 0ull; i < p; ++i) {
		std::cout
			<< ' '
			<< set_col_width << m_t.get_degree(m_arr[i]);
	}
	for (position i = p; i < m_n_nodes; ++i) {
		std::cout << ' ' << set_col_width << 'x';
	}
}

void AEF_BnB::output_left_degree_sequence(const position p) const noexcept {
	for (position_t i = 0ull; i < p; ++i) {
		std::cout
			<< ' '
			<< set_col_width << m_node_left_degree[m_arr[i]];
	}
	for (position i = p; i < m_n_nodes; ++i) {
		std::cout << ' ' << set_col_width << 'x';
	}
}

void AEF_BnB::output_right_degree_sequence(const position p) const noexcept {
	for (position_t i = 0ull; i < p; ++i) {
		std::cout
			<< ' '
			<< set_col_width << m_node_right_degree[m_arr[i]];
	}
	for (position i = p; i < m_n_nodes; ++i) {
		std::cout << ' ' << set_col_width << 'x';
	}
}

void AEF_BnB::output_level_sequence(const position p) const noexcept {
	for (position_t i = 0ull; i < p; ++i) {
		std::cout
			<< ' '
			<< set_col_width << m_node_level[m_arr[i]];
	}
	for (position i = p; i < m_n_nodes; ++i) {
		std::cout << ' ' << set_col_width << 'x';
	}
}

void AEF_BnB::output_cut_signature(const position p) const noexcept {
	for (position_t i = 0ull; i < p; ++i) {
		std::cout
			<< ' '
			<< set_col_width << m_cut_values[*i];
	}
	for (position i = p; i < m_n_nodes; ++i) {
		std::cout << ' ' << set_col_width << 'x';
	}
}

void AEF_BnB::output_num_assigned_neighbors() const noexcept {
	for (node u = 0; u < m_n_nodes; ++u) {
		std::cout
			<< ' '
			<< set_col_width << m_num_assigned_neighbors[u];
	}
}

void AEF_BnB::output_num_unassigned_neighbors() const noexcept {
	for (node u = 0; u < m_n_nodes; ++u) {
		std::cout
			<< ' '
			<< set_col_width << m_num_unassigned_neighbors[u];
	}
}

void AEF_BnB::output_border_nodes() const noexcept {
	std::cout << tab() << "    Vertices: ";
	for (std::size_t i = 0; i < m_border_nodes.size(); ++i) {
		std::cout << ' ' << m_border_nodes[i];
	}
	std::cout << '\n';

	std::cout << tab() << "    Positions:";
	for (std::size_t i = 0; i < m_border_nodes.size(); ++i) {
		const auto ui = m_border_nodes[i];
		std::cout << ' ' << m_border_nodes.position(ui);
	}
	std::cout << '\n';

	std::cout << tab() << "    In border:";
	for (node u = 0; u < m_n_nodes; ++u) {
		std::cout << ' ' << (m_border_nodes.exists(u) ? 'o' : 'x');
	}
	std::cout << '\n';
}

void AEF_BnB::output_predicted_level_values() const noexcept {
	array<int> widths(m_n_nodes, 0);

	std::cout << tab() << "    Vertices:  ";
	for (node u = 0; u < m_n_nodes; ++u) {
		widths[u] = 1;
		if (has_valid_LV_prediction(u)) {
			widths[u] = static_cast<int>(std::to_string(m_predicted_LV[u]).length());
		}
		widths[u] =
			std::max(
				widths[u],
				static_cast<int>(std::to_string(u).length())
			);
		widths[u] =
			std::max(
				widths[u],
				static_cast<int>(LV_propagation_origin_to_short_string(m_predicted_LV__origin[u]).length())
			);
		std::cout << ' ' << std::setw(widths[u]) << u;
	}
	std::cout << '\n';

	std::cout << tab() << "    Prediction:";
	for (node u = 0; u < m_n_nodes; ++u) {
		std::cout << ' ' << std::setw(widths[u]);
		if (has_valid_LV_prediction(u)) {
			std::cout << m_predicted_LV[u];
		}
		else {
			std::cout << ' ';
		}
	}
	std::cout << '\n';
	std::cout << tab() << "    Origin:    ";
	for (node u = 0; u < m_n_nodes; ++u) {
		std::cout
			<< ' ' << std::setw(widths[u])
			<< LV_propagation_origin_to_short_string(m_predicted_LV__origin[u]);
	}
	std::cout << '\n';

	std::cout << tab() << "    Triggers:  ";
	for (node u = 0; u < m_n_nodes; ++u) {
		std::cout
			<< ' '
			<< std::setw(widths[u])
			<< (is_node_a_trigger_of_LV(u) ? 'x' : ' ');
	}
	std::cout << '\n';
}

void AEF_BnB::output_path_info() const noexcept {
	const std::size_t num_paths = m_paths_in_tree.size();

	array<std::size_t> widths(num_paths, 0);
	for (std::size_t i = 0; i < num_paths; ++i) {
		widths[i] = std::max(
			widths[i],
			std::to_string(int(m_path_info[i].num_assigned_nodes)).length()
		);

		widths[i] = std::max(
			widths[i],
			std::to_string(int(m_path_info[i].num_thistles)).length()
		);
	}

	std::cout << tab() << "    Assigned vertices:                  ";
	for (std::size_t i = 0; i < num_paths; ++i) {
		std::cout
			<< ' '
			<< std::setw(to_int32(widths[i]))
			<< int(m_path_info[i].num_assigned_nodes);
	}
	std::cout << '\n';
	std::cout << tab() << "    Assigned vertices (+2):             ";
	for (std::size_t i = 0; i < num_paths; ++i) {
		std::cout
			<< ' '
			<< std::setw(to_int32(widths[i]))
			<< int(m_path_info[i].num_assigned_nodes_p2);
	}
	std::cout << '\n';
	std::cout << tab() << "    Assigned vertices (-2):             ";
	for (std::size_t i = 0; i < num_paths; ++i) {
		std::cout
			<< ' '
			<< std::setw(to_int32(widths[i]))
			<< int(m_path_info[i].num_assigned_nodes_m2);
	}
	std::cout << '\n';

	std::cout << tab() << "    Amount thistles per antenna/bridge: ";
	for (std::size_t i = 0; i < num_paths; ++i) {
		std::cout
			<< ' '
			<< std::setw(to_int32(widths[i]))
			<< int(m_path_info[i].num_thistles);
	}
	std::cout << '\n';
}

void AEF_BnB::display_all_info(
	const uint64_t D_p, const uint64_t D_ps_m,
	const position pos
)
noexcept
{
	/*
	std::cout << tab() << "                      ";
	{
	for (std::size_t i = 0; i < m_t.get_num_nodes(); ++i) {
		std::cout
			<< ' '
			<< (COLOR(m_vertex_colors[i] == 0 ? BLUE : RED))
			<< set_col_width << i
			<< END_COLOR;
	}
	std::cout << '\n';
	}
	*/

	std::cout << tab() << "SUMMARY\n";
	std::cout << tab() << "Arrangement properties:\n";
	std::cout << tab() << "    Arrangement:          ";   output_arrangement(); std::cout << '\n';
	std::cout << tab() << "    Inverse arrangement:  ";   output_invarr(pos); std::cout << '\n';
	std::cout << tab() << "    Degree sequence:      ";   output_degree_sequence(pos); std::cout << '\n';
	std::cout << tab() << "    Left degree sequence: ";   output_left_degree_sequence(pos); std::cout << '\n';
	std::cout << tab() << "    Right degree sequence:";   output_right_degree_sequence(pos); std::cout << '\n';
	std::cout << tab() << "    Level sequence:       ";   output_level_sequence(pos); std::cout << '\n';
	std::cout << tab() << "    Cut signature:        ";   output_cut_signature(pos); std::cout << '\n';
	std::cout << tab() << "Vertex properties:\n";
	std::cout << tab() << "    Assigned neighbors:   ";   output_num_assigned_neighbors(); std::cout << '\n';
	std::cout << tab() << "    Unassigned neighbors: ";   output_num_unassigned_neighbors(); std::cout << '\n';
	std::cout << tab() << "Border vertices:\n";
	output_border_nodes();
	std::cout << tab() << "Predicted level values:\n";
	output_predicted_level_values();
	std::cout << tab() << "Paths\n";
	output_path_info();
	std::cout << tab() << "Lengths:\n";
	std::cout << tab() << "    D_p=    " << D_p << '\n';
	std::cout << tab() << "    D_ps^-= " << D_ps_m << '\n';
	std::cout << tab() << "Edge sets:\n";
	std::cout << tab() << "    E_p: ";
	for (std::size_t i = 0; i < m_E_p.size(); ++i) {
		const auto [u,v] = m_E_p[i];
		std::cout << " (" << u << "," << v << ")";
	}
	std::cout << '\n';
	std::cout << tab() << "    E_ps:";
	for (std::size_t i = 0; i < m_E_ps.size(); ++i) {
		const auto [u,v] = m_E_ps[i];
		std::cout << " (" << u << "," << v << ")";
	}
	std::cout << '\n';
	std::cout << tab() << "    E_s: ";
	for (std::size_t i = 0; i < m_E_s.size(); ++i) {
		const auto [u,v] = m_E_s[i];
		std::cout << " (" << u << "," << v << ")";
	}
	std::cout << '\n';
}
#endif

} // -- namespace unconstrained
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
