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
 *******************************************************************/

#pragma once

// C++ includes
#include <string_view>
#include <array>

// lal includes
#include <lal/linarr/syntactic_dependency_tree/type.hpp>
#include <lal/detail/make_array.hpp>

namespace lal {
namespace detail {

/// Converts a value of the enumeration
/// @ref lal::linarr::syntactic_dependency_tree into a string.
constexpr std::string_view
syntactic_dependency_tree_to_string
	(const linarr::syntactic_dependency_tree& tt)
noexcept
{
	switch (tt) {
	case linarr::syntactic_dependency_tree::EC1: return "EC1";
	case linarr::syntactic_dependency_tree::planar: return "planar";
	case linarr::syntactic_dependency_tree::projective: return "projective";
	case linarr::syntactic_dependency_tree::WG1: return "WG1";
		default: return "unknown_syntactic_dependency_tree";
	}
}

/// The array of all types of syntact dependency structures.
inline constexpr
std::array<
		linarr::syntactic_dependency_tree,
	linarr::__syntactic_dependency_tree_size
>
array_of_syntactic_dependency_trees =
make_array<
				linarr::syntactic_dependency_tree,
				linarr::syntactic_dependency_tree::EC1,
				linarr::syntactic_dependency_tree::planar,
				linarr::syntactic_dependency_tree::projective,
				linarr::syntactic_dependency_tree::WG1,
				linarr::syntactic_dependency_tree::unknown
>();

} // -- namespace detail
} // -- namespace lal
