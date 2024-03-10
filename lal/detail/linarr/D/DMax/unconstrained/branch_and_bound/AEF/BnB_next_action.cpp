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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
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
#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
#include <iostream>
#endif
#endif

// lal includes
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/sorting/counting_sort.hpp>

// DMax includes
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>

/* number of nodes in the suffix:
 * Since p ranges in the interval [0,n), the number of nodes is:
 *
 *		n - (p + 1) + 1 = n - p
 */
#define nodes_in_suffix(n,p) (n - p)

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

uint64_t AEF_BnB::upper_bound_generic
(const uint64_t D_1, const uint64_t D_12_m, const position_t pos)
noexcept
{
#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << tab() << "Calculate an upper bound\n";
#endif

	uint64_t D_upper = 0;

	// upper bound on E_12
	{
#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << tab() << "Upper bound E_12\n";
	std::cout << tab() << "    Vertices with some neighbor assigned:\n";
#endif

	const std::size_t idx = m_border_vertices.size();

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	for (std::size_t i = 0; i < idx; ++i) {
		const auto ui = m_border_vertices[i];
		std::cout
			<< tab() << "        "
			<< ui << " -> "
			<< m_num_assigned_neighbors[ui]
			<< '\n';
	}
#endif

#if defined DEBUG
	for (std::size_t i = 0; i < idx; ++i) {
		const auto ui = m_border_vertices[i];
		assert(m_border_vertices.position(ui) == i);
		assert(m_num_assigned_neighbors[ui] > 0);
		assert(not is_vertex_assigned(ui));
		assert(m_border_vertices.exists(ui));
	}
	for (node u = 0; u < m_n_nodes; ++u) {
		if (not m_border_vertices.exists(u) and not is_vertex_assigned(u)) {
			assert(m_num_assigned_neighbors[u] == 0);
		}
	}
#endif

	{
	// sort the vector of border vertices
	sorting::counting_sort
	<node, sorting::non_increasing_t>
	(
		m_border_vertices.begin_values(),
		m_border_vertices.begin_values() + idx,
		m_n_nodes, idx,
		[&](node u) { return m_num_assigned_neighbors[u]; }
	);
	}

	// fix (as in 'correct', 'repair') the positions of the vertices...
	for (std::size_t i = 0; i < idx; ++i) {
		const auto ui = m_border_vertices[i];
		m_border_vertices.begin_position()[ui] = i;
	}

#if defined DEBUG
	for (std::size_t i = 0; i < idx; ++i) {
		const auto ui = m_border_vertices[i];
		assert(m_border_vertices.position(ui) == i);
	}
#endif

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << tab() << "    Vertices with some neighbor assigned ordered by degree:\n";
	for (std::size_t i = 0; i < idx; ++i) {
		const node ui = m_border_vertices[i];
		std::cout
			<< tab() << "        "
			<< ui << " -> "
			<< m_num_assigned_neighbors[ui]
			<< '\n';
	}
#endif

	uint64_t D_upper_E_12_p = 0;
	uint64_t current_length = m_n_nodes - (*pos + 1);
	for (std::size_t i = 0; i < idx; ++i) {
		const node ui = m_border_vertices[i];
		const uint64_t k = m_num_assigned_neighbors[ui];
		D_upper_E_12_p += current_length*k;
		--current_length;
	}

	const uint64_t second_D_upper_E_12 = D_12_m + D_upper_E_12_p;
#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << tab() << "    D_12_m=             " << D_12_m << '\n';
	std::cout << tab() << "    upper bound D_12_p= " << D_upper_E_12_p << '\n';
	std::cout << tab() << "    upper bound E_12=   " << second_D_upper_E_12 << '\n';
#endif

	D_upper += second_D_upper_E_12;
	}

	// upper bound on E_2
	{
#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << tab() << "Upper bound E_2\n";
#endif

	const uint64_t n = nodes_in_suffix(m_n_nodes, *pos);
	const uint64_t m = m_E_s.size();
#if defined DEBUG
	// if we have that m=n then the graph induced by E_2 is not a tree,
	// but this can never happen because the input graph *is* a tree.
	assert(m < n);
#endif
	// The upper bound is calculated in the launcher.
	// The values are stored in this matrix 'm_upper_bound_E_2'
	// to avoid calculation of the same values over and over
	// again.
	const uint64_t D_upper_E_2 = (4*n*m + (m%2) - m*m - 4*m)/4;
	D_upper += D_upper_E_2;
#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << tab() << "    n=               " << n << '\n';
	std::cout << tab() << "    m=               " << m << '\n';
	std::cout << tab() << "    upper bound E_2= " << D_upper_E_2 << '\n';
#endif
	}

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << tab() << "D_1= " << D_1 << '\n';
	std::cout << tab() << "Upper bounds:\n";
	std::cout << tab() << "    D_upper= " << D_upper << '\n';
#endif

	return D_upper + D_1;
}

AEF_BnB::next_action AEF_BnB::what_to_do_next
(const uint64_t D_1, const uint64_t D_12_m, const position_t pos)
noexcept
{
#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << tab() << "Deciding what to do next...\n";
#endif

	{
	// calculate generic upper bound
	const uint64_t D_upper = upper_bound_generic(D_1, D_12_m, pos);

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	std::cout << tab() << "D_upper_generic= " << D_upper << '\n';
	std::cout << tab() << "DMax_current=    " << m_max_arrs.get_max_value()
			  << '\n';
#endif

	if (D_upper < m_max_arrs.get_max_value()) { return next_action::bound; }
	}

	// the remaining set of (unassigned) vertices is an independent set
	if (m_E_s.size() == 0) {

		// If the generic upper bound did not fail then it means that,
		// in the case of E_2 being empty, the remaining vertices can
		// be rearranged so that the result is a potential new maximum.

		// decide whether or not the remaining have all degree 1
		bool all_are_leaves = true;
		for (std::size_t i = 0; i < m_E_ps.size() and all_are_leaves; ++i) {
			const auto [u,v] = m_E_ps[i];
			const auto du = m_t.get_degree(u);
			const auto dv = m_t.get_degree(v);
			if (is_vertex_assigned(u)) {
				all_are_leaves = all_are_leaves and (dv == 1);
			}
			else if (is_vertex_assigned(v)) {
				all_are_leaves = all_are_leaves and (du == 1);
			}
		}

		// we might reach the maximum, therefore the show must go on
		return (
			all_are_leaves ?
				next_action::continue_independent_set_leaves :
				next_action::continue_independent_set);
	}

	return next_action::continue_normally;
}

} // -- namespace unconstrained
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
