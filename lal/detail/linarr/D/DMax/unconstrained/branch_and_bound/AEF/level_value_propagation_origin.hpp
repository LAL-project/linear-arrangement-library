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
 * @brief Origin of the propagation of level value.
 *
 * This is used in @ref lal::detail::DMax::unconstrained::AEF_BnB to keep track
 * the origin of each propagation of level values.
 */
enum class LV_propagation_origin : int8_t {
	/// Propagation started at the leaf of an antenna
	antenna_leaf,
	/// Propagation started at an internal vertex of an antenna
	antenna_internal,
	/// Propagation started at the hub of an antenna
	antenna_hub,
	/// Propagation started at the first hub of a bridge
	bridge_hub_1,
	/// Propagation started at the second hub of a bridge
	bridge_hub_2,
	/**
	 * Propagation started at the lowest lexicographic vertex of a bridge.
	 * The level value of this vertex is \f$\pm2\f$.
	 */
	bridge_lowest_pm2,
	/**
	 * Propagation started at the lowest lexicographic vertex of a bridge.
	 * The level value of this vertex is \f$0\f$.
	 */
	bridge_lowest_0,
	/**
	 * Propagation started at an internal vertex of a bridge.
	 * This vertex is to the left of the lowest lexicographic vertex in the
	 * bridge.
	 */
	bridge_internal_left,
	/**
	 * Propagation started at an internal vertex of a bridge.
	 * This vertex is to the right of the lowest lexicographic vertex in the
	 * bridge.
	 */
	bridge_internal_right,
	/// This indicates the origin of the propagation.
	self,
	/// Null value
	none
};

#if defined __LAL_DEBUG_DMax_Unc_BnB
/// Converts a value of @ref lal::detail::DMax::unconstrained::LV_propagation_origin
/// to a long string.
[[nodiscard]] inline constexpr std::string_view LV_propagation_origin_to_string
(const LV_propagation_origin at) noexcept
{
	switch (at) {
	case LV_propagation_origin::antenna_leaf: return "Antenna (leaf)";
	case LV_propagation_origin::antenna_internal: return "Antenna (internal)";
	case LV_propagation_origin::antenna_hub: return "Antenna (hub)";
	case LV_propagation_origin::bridge_internal_left: return "Bridge (internal left)";
	case LV_propagation_origin::bridge_internal_right: return "Bridge (internal right)";
	case LV_propagation_origin::bridge_lowest_pm2: return "Bridge (lowest -- level +-2)";
	case LV_propagation_origin::bridge_lowest_0: return "Bridge (lowest -- level 0)";
	case LV_propagation_origin::bridge_hub_1: return "Bridge (hub 1)";
	case LV_propagation_origin::bridge_hub_2: return "Bridge (hub 2)";
	case LV_propagation_origin::self: return "Self";
	case LV_propagation_origin::none: return "None";
	}
	return "";
}

/// Converts a value of @ref lal::detail::DMax::unconstrained::LV_propagation_origin
/// to a short string.
[[nodiscard]] inline constexpr std::string_view LV_propagation_origin_to_short_string
(const LV_propagation_origin at) noexcept
{
	switch (at) {
	case LV_propagation_origin::antenna_leaf: return "al";
	case LV_propagation_origin::antenna_internal: return "ai";
	case LV_propagation_origin::antenna_hub: return "ah";
	case LV_propagation_origin::bridge_internal_left: return "bil";
	case LV_propagation_origin::bridge_internal_right: return "bir";
	case LV_propagation_origin::bridge_lowest_pm2: return "bl2";
	case LV_propagation_origin::bridge_lowest_0: return "bl0";
	case LV_propagation_origin::bridge_hub_1: return "bh1";
	case LV_propagation_origin::bridge_hub_2: return "bh2";
	case LV_propagation_origin::self: return "s";
	case LV_propagation_origin::none: return " ";
	}
	return "?";
}
#endif

} // -- namespace unconstrained
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
