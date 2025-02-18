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
#if defined __LAL_DEBUG_DMax_Unc_BnB
#include <iostream>
#endif
#endif

// lal includes
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/graphs/output.hpp>

// DMax includes
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

void AEF_BnB::roll_back_LV__antenna(const std::size_t path_idx) noexcept
{
#if defined __LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Roll back through an antenna...\n";
#endif

	const auto& path = m_paths_in_tree[path_idx];
	const std::size_t N = path.get_num_nodes();

	m_path_info[path_idx].nodes_m2_to_assign.reset();
	m_path_info[path_idx].nodes_p2_to_assign.reset();

	const node h1 = path.get_h1();
	if (m_t.get_degree(h1) == 1) {
#if defined DEBUG
		assert(has_valid_LV_prediction(h1));
#endif
		m_predicted_LV__origin[h1] = LV_propagation_origin::none;
	}

	for (std::size_t i = 1; i < N - 1; ++i) {
		const node w = path[i];
#if defined DEBUG
		assert(has_valid_LV_prediction(w));
#endif
		m_predicted_LV__origin[w] = LV_propagation_origin::none;
	}

	const node h2 = path.get_h2();
	if (m_t.get_degree(h2) == 1) {
#if defined DEBUG
		assert(has_valid_LV_prediction(h2));
#endif
		m_predicted_LV__origin[h2] = LV_propagation_origin::none;
	}
}

} // namespace unconstrained
} // namespace DMax
} // namespace detail
} // namespace lal
