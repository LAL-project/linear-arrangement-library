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
#if defined __LAL_DEBUG_DMax_Unc_BnB
#include <iostream>
#endif
#endif

// lal includes
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/linarr/D/D.hpp>

// DMax includes
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

AEF_BnB::exe_result_type
AEF_BnB::exe_independent_set
(const uint64_t D_p, position pos)
noexcept
{
	// number of remaining vertices
	const std::size_t num_remain_verts = m_n_nodes - pos;

#if defined DEBUG
	assert(num_remain_verts > 0);
#endif

#if defined __LAL_DEBUG_DMax_Unc_BnB
	std::cout
		<< tab()
		<< "The remaining vertices (" << num_remain_verts << ") "
		<< "make up an various-degree independent set.\n";
#endif

	array<node> remain_verts(num_remain_verts);
	{
	std::size_t i = 0;
	uint64_t max_deg = 0;
	for (node u = 0; u < m_n_nodes; ++u) {
		if (not is_vertex_assigned(u)) {
			remain_verts[i++] = u;
			max_deg = std::max(max_deg, m_t.get_degree(u));
		}
	}

	sorting::counting_sort
	<node, sorting::non_decreasing_t>
	(
		remain_verts.begin(), remain_verts.end(),
		max_deg, num_remain_verts,
		[&](const node u) { return m_t.get_degree(u); }
	);

#if defined DEBUG
	assert(i == num_remain_verts);
#endif
	}

	// ATTENTION! Moving away from the actual nature of a Branch & Bound
	// algorithm, we no longer generate all arrangements since all of them
	// will yield the same level signature.

	uint64_t D = D_p;
	for (std::size_t i = 0; i < remain_verts.size(); ++i) {
		const auto u = remain_verts[i];
		m_arr.assign(u, pos);

		for (node_t v : m_t.get_neighbors(u)) {
			D += pos - m_arr[v];
		}

		++pos;
	}

	// We do not call process_end() to avoid the assertions involving
	// levels since these were not computed. Recall that levels are updated
	// in the function update_state() which is not called here since it
	// is not needed.

	m_max_arrs.add(D, m_arr);

#if defined __LAL_DEBUG_DMax_Unc_BnB
	const bool reached_maximum = m_max_arrs.get_max_value() == D;
	return reached_maximum;
#else
	return;
#endif
}

// -----------------------------------------------------------------------------

AEF_BnB::exe_result_type AEF_BnB::exe_independent_set_leaves
(const uint64_t D_p, position pos)
noexcept
{
	/*
	 * The independent set is a set of leaves. If the algorithm has reached this
	 * point then there are chances of finding a maximum arrangement. By construction,
	 * the unassigned vertices all have degree 1 and:
	 * - they are all attached to the same parent
	 *		- a possible case is a star tree: in this case we can actually find
	 *        a maximum arrangement,
	 *		- the other cases cannot produce an arrangement that is actually
	 *        maximum and will never reach this function.
	 * - they are attached to different parents
	 *		- the resulting arrangement cannot be maximum because it will not
	 *		  produce an arrangement with non-increasing level sequence.
	 *
	 * Therefore, making just one permutation of the vertices will suffice to
	 * keep the correctness of the Branch & Bound, which is finding all
	 * non-isomorphic solutions (w.r.t. level sequences).
	 */

	uint64_t D = D_p;
	for (node u = 0; u < m_n_nodes; ++u) {
		if (is_vertex_assigned(u)) { continue; }

#if defined DEBUG
		assert(m_t.get_degree(u) == 1);
#endif

		m_arr.assign(u, pos);

		const node_t only_neighbour = leaf_parent(u);
		D += pos - m_arr[only_neighbour];

		++pos;
	}

	// We do not call process_end() to avoid the assertions involving
	// levels since these were not computed. Recall that levels are updated
	// in the function update_state() which is not called here since it
	// is not needed.

	m_max_arrs.add(D, m_arr);

#if defined __LAL_DEBUG_DMax_Unc_BnB
	const bool reached_maximum = m_max_arrs.get_max_value() == D;
	return reached_maximum;
#else
	return;
#endif
}

} // -- namespace unconstrained
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
