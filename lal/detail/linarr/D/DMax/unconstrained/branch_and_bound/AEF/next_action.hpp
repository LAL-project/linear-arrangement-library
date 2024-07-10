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

#pragma once

// C++ includes
#if defined __LAL_DEBUG_DMax_Unc_BnB
#include <string_view>
#endif
#include <cstdint>

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

/**
 * @brief What should be done after checking the upper bound?
 *
 * This is used in @ref lal::detail::DMax::unconstrained::AEF_BnB to decide what
 * to do after the upper bound was calculated.
 */
enum class next_action : int8_t {
	/// Bound the exploration (Bound).
	bound,
	/// Continue the exploration normally (Branch)
	continue_normally,
	/**
	 * Finish the exploration.
	 * In this case, the remaining vertices to assign make up an independent
	 * set in which @e not all vertices have degree 1.
	 */
	continue_independent_set,
	/**
	 * Finish the exploration.
	 * In this case, the remaining vertices to assign make up an independent
	 * set in which @e all vertices have degree 1.
	 */
	continue_independent_set_leaves
};

#if defined __LAL_DEBUG_DMax_Unc_BnB
/// Converts a value of @ref lal::detail::DMax::unconstrained::next_action
/// to a string.
[[nodiscard]] constexpr std::string_view next_action_type_to_string
(const next_action at) noexcept
{
	switch (at) {
	case next_action::bound:
		return "Bound computation";
	case next_action::continue_normally:
		return "Continue computation";
	case next_action::continue_independent_set:
		return "Continue computation for an independent set";
	case next_action::continue_independent_set_leaves:
		return "Continue computation for an independent set of leaves";
	}
	return "";
}
#endif

} // -- namespace unconstrained
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
