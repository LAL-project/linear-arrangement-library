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
#if defined LAL_DEBUG_DMax_Unc_BnB
#include <iostream>
#endif
#endif

// lal includes
#if defined LAL_DEBUG_DMax_Unc_BnB
#include <lal/graphs/output.hpp>
#endif

// DMax includes
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

AEF_BnB::exe_result_type AEF_BnB::exe(
	const uint64_t D_p, const uint64_t D_ps_m, const position pos
) noexcept
{
#if defined LAL_DEBUG_DMax_Unc_BnB
	display_all_info(D_p, D_ps_m, pos);
#endif

	// Did the algorithm complete an arrangement?
	{
		const auto res = process_end(D_p, pos);
		if (did_reach_end(res)) {
#if defined LAL_DEBUG_DMax_Unc_BnB
			return did_find_max(res);
#else
			return;
#endif
		}
	}

	{
		// what should we do next? ...
		const auto next = what_to_do_next(D_p, D_ps_m, pos);

#if defined LAL_DEBUG_DMax_Unc_BnB
		std::cout << tab() << "Decided: " << '\''
				  << next_action_type_to_string(next) << '\'' << '\n';
#endif

		if (next == next_action::bound) {
#if defined LAL_DEBUG_DMax_Unc_BnB
			// did not reach a maximum
			return false;
#else
			return;
#endif
		}

		if (next == next_action::continue_independent_set) {
#if defined LAL_DEBUG_DMax_Unc_BnB
			const bool reached_maximum =
#endif
				exe_independent_set(D_p, pos);

#if defined LAL_DEBUG_DMax_Unc_BnB
			return reached_maximum;
#else
			return;
#endif
		}

		if (next == next_action::continue_independent_set_leaves) {
#if defined LAL_DEBUG_DMax_Unc_BnB
			const bool reached_maximum =
#endif
				exe_independent_set_leaves(D_p, pos);

#if defined LAL_DEBUG_DMax_Unc_BnB
			return reached_maximum;
#else
			return;
#endif
		}
	}

#if defined LAL_DEBUG_DMax_Unc_BnB
	bool reached_max = false;
#endif

	// for every remaining vertex u
	for (node u = 0; u < m_n_nodes; ++u) {
		if (is_vertex_assigned(u)) {
			continue;
		}

#if defined LAL_DEBUG_DMax_Unc_BnB
		std::cout << tab() << "Trying vertex '" << u << "' at position '" << pos
				  << "'\n";
#endif

		// if the filtering says 'do not use this vertex', then move
		// on to the next vertex
		const reason_discard discard = discard_vertex(u, pos);

		if (discard != reason_discard::none) {
#if defined LAL_DEBUG_DMax_Unc_BnB
			std::cout << tab() << "    Vertex was discarded because: '"
					  << reason_discard_to_string(discard) << "'\n";
#endif
			continue;
		}

		// assign vertex 'u' at position 'p'
		uint64_t D_p_next = D_p;
		uint64_t D_ps_m_next = D_ps_m;
		update_state(u, pos, D_p_next, D_ps_m_next);

#if defined LAL_DEBUG_DMax_Unc_BnB
		std::cout << tab() << "Propagating...\n";
#endif

		const propagation_result propagation = propagate_constraints(u);

#if defined LAL_DEBUG_DMax_Unc_BnB
		if (propagation != propagation_result::success) {
			std::cout << tab() << "Result of propagation: '"
					  << propagation_result_to_string(propagation) << "'\n";
		}
#endif

		if (propagation == propagation_result::success) {
			// Try solving DMax with the current configuration

#if defined LAL_DEBUG_DMax_Unc_BnB
			push_tab();
#endif

#if defined LAL_DEBUG_DMax_Unc_BnB
			const bool branch_reached_maximum =
#endif
				exe(D_p_next, D_ps_m_next, pos + 1);

#if defined LAL_DEBUG_DMax_Unc_BnB
			reached_max = reached_max or branch_reached_maximum;
			pop_tab();

			std::cout << tab() << "Branch reached maximum? " << std::boolalpha
					  << branch_reached_maximum << '\n';
#endif
		}

		roll_back_constraints(u);

#if defined LAL_DEBUG_DMax_Unc_BnB
		display_all_info(D_p, D_ps_m, pos);
#endif

		recover_state(pos);
	}

#if defined LAL_DEBUG_DMax_Unc_BnB
	return reached_max;
#endif
}

void AEF_BnB::exe(const node first_node) noexcept
{
	m_first_node = first_node;

	m_rt.init_rooted(m_t, m_first_node);

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << "**************************************************\n";
	std::cout << "*** Started execution of Branch & Bound at '" << m_first_node
			  << "' ***\n";
	std::cout << "**************************************************\n";
	std::cout << "Input (free) tree:\n";
	std::cout << m_t << '\n';
	std::cout << "Input (rooted) tree:\n";
	std::cout << m_rt << '\n';
	output_edge_list();
	display_all_info(0, 0, 0);
	push_tab();
#endif

	uint64_t D_p_next = 0;
	uint64_t D_ps_next = 0;
	update_state(m_first_node, 0ull, D_p_next, D_ps_next);

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Propagating...\n";
#endif

	const propagation_result res = propagate_constraints(m_first_node);

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Result of propagation: '"
			  << propagation_result_to_string(res) << "'\n";
#endif

	if (res == propagation_result::success) {
		exe(D_p_next, D_ps_next, 1);
	}

	roll_back_constraints(m_first_node);

	recover_state(0ull);

#if defined LAL_DEBUG_DMax_Unc_BnB
	pop_tab();
#endif

#if defined DEBUG
	// assert there are no valid predictions
	for (const auto& path : m_paths_in_tree) {
		for (node u : path.get_vertex_sequence()) {
			assert(not has_valid_LV_prediction(u));
		}
	}
#endif
}

} // namespace unconstrained
} // namespace DMax
} // namespace detail
} // namespace lal
