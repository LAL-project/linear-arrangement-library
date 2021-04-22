/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. To see the full code
 *  visit the webpage:
 *      https://github.com/lluisalemanypuig/linear-arrangement-library.git
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
 ********************************************************************/

#pragma once

// C++ includes
#include <string_view>
#include <array>

// lal includes
#include <lal/graphs/tree_type.hpp>
#include <lal/internal/make_array.hpp>

namespace lal {
namespace internal {

// Converts to a string a value of the enumeration @ref lal::graphs::tree_type.
inline constexpr std::string_view
tree_type_string(const graphs::tree_type& tt)
noexcept
{
	using tree_type = graphs::tree_type;

	switch (tt) {
		case tree_type::bistar: return "bistar";
		case tree_type::caterpillar: return "caterpillar";
		case tree_type::linear: return "linear";
		case tree_type::quasistar: return "quasistar";
		case tree_type::spider: return "spider";
		case tree_type::star: return "star";
		default: return "unknown_tree_type";
	}
}

static constexpr std::array<graphs::tree_type, graphs::__tree_type_size>
array_of_tree_types =
make_array<
	graphs::tree_type,
	graphs::tree_type::bistar,
	graphs::tree_type::caterpillar,
	graphs::tree_type::linear,
	graphs::tree_type::quasistar,
	graphs::tree_type::spider,
	graphs::tree_type::star,
	graphs::tree_type::unknown
>();

} // -- namespace internal
} // -- namespace lal
