/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2023
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
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// lal includes
#include <lal/detail/type_traits/first_true.hpp>

namespace lal {
namespace detail {

/// A sequence of Boolean values.
template <bool... values>
struct bool_sequence {
	/// Amount of true values in the sequence.
	static constexpr std::size_t num_true = (values + ...);
	/// Index of the first value set to true.
	static constexpr std::size_t index_true = first_true_v<values...>;
};

static_assert(bool_sequence<true>::index_true == 0);
static_assert(bool_sequence<false>::index_true == 1);
static_assert(bool_sequence<true>::num_true == 1);
static_assert(bool_sequence<false>::num_true == 0);

static_assert(bool_sequence<false,false>::index_true == 2);
static_assert(bool_sequence<false,true>::index_true == 1);
static_assert(bool_sequence<true,false>::index_true == 0);
static_assert(bool_sequence<true,true>::index_true == 0);
static_assert(bool_sequence<false,false>::num_true == 0);
static_assert(bool_sequence<false,true>::num_true == 1);
static_assert(bool_sequence<true,false>::num_true == 1);
static_assert(bool_sequence<true,true>::num_true == 2);

} // -- namespace detail
} // -- namespace lal
