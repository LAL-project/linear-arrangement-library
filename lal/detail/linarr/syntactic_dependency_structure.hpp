/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 *  Linear Arrangement Library is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU Affero General Public License
 *  as published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  Linear Arrangement Library is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *
 *      Lluís Alemany Puig (lalemany@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 *******************************************************************/

#pragma once

// C++ includes
#include <string_view>
#include <array>

// lal includes
#include <lal/linarr/syntactic_dependency_structure.hpp>
#include <lal/detail/make_array.hpp>

namespace lal {
namespace detail {

// Converts to a string a value of the enumeration @ref syntactic_dependency_structure.
constexpr std::string_view
syntactic_dependency_structure_to_string
(const linarr::syntactic_dependency_structure& tt)
noexcept
{
	switch (tt) {
		case linarr::syntactic_dependency_structure::EC1: return "EC1";
		case linarr::syntactic_dependency_structure::planar: return "planar";
		case linarr::syntactic_dependency_structure::projective: return "projective";
		case linarr::syntactic_dependency_structure::WG1: return "WG1";
		default: return "unknown_syntactic_dependency_structure";
	}
}

static constexpr
std::array<
	linarr::syntactic_dependency_structure,
	linarr::__syntactic_dependency_structure_size
>
array_of_syntactic_dependency_structures =
make_array<
	linarr::syntactic_dependency_structure,
	linarr::syntactic_dependency_structure::EC1,
	linarr::syntactic_dependency_structure::planar,
	linarr::syntactic_dependency_structure::projective,
	linarr::syntactic_dependency_structure::WG1,
	linarr::syntactic_dependency_structure::unknown
>();

} // -- namespace detail
} // -- namespace lal
