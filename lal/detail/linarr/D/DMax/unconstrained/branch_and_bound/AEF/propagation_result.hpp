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
 * @brief The possible results of the propagation of level values.
 *
 * This is used in @ref lal::detail::DMax::unconstrained::AEF_BnB when propagating
 * level values. See also @ref lal::detail::DMax::unconstrained::LV_propagation_origin.
 */
enum class propagation_result : int8_t {
	/// The propagation did not fail.
	success,
	/// There was a conflict while propagating the level value.
	conflict_LV_propagation,
	/// There was a conflict when emulating the propagation of level value.
	conflict_LV_emulated_propagation,

	/**
	 * Null value.
	 * This is used only for debugging.
	 */
	__last_item
};

#if defined __LAL_DEBUG_DMax_Unc_BnB
/// Converts a value of @ref lal::detail::DMax::unconstrained::propagation_result
/// to a long string.
[[nodiscard]] static inline constexpr std::string_view propagation_result_to_string
(const propagation_result at) noexcept
{
	switch (at) {
	case propagation_result::success:
		return "Success";
	case propagation_result::conflict_LV_propagation:
		return "Propagation of level values conflicted";
	case propagation_result::conflict_LV_emulated_propagation:
		return "Emulated propagation of level values conflicted";
	case propagation_result::__last_item:
		return "?";
	}
	return "";
}

/// Converts a value of @ref lal::detail::DMax::unconstrained::propagation_result
/// to an index value.
[[nodiscard]] static inline constexpr std::size_t propagation_result_to_index
(propagation_result rd) noexcept
{ return static_cast<std::size_t>(rd); }

/// Converts an index value to a value of
/// @ref lal::detail::DMax::unconstrained::propagation_result.
[[nodiscard]] static inline constexpr propagation_result index_to_propagation_result
(std::size_t i) noexcept
{ return static_cast<propagation_result>(i); }
#endif

} // -- namespace unconstrained
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
