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

// C++ includes
#include <cstdint>

namespace lal {
namespace detail {

/**
 * @brief From a list of Boolean values, find the first that is set to true.
 *
 * If there is no value set to true, then the index will be equal to the
 * amount of values in the list.
 * @tparam values List of Boolean values known at compile time.
 */
template <bool... values>
struct first_true {

	/// Implementation of @ref first_true.
	template <std::size_t idx, bool v1, bool... vs>
	struct _ft_impl {
		/// The index of the first value set to true.
		static constexpr std::size_t value =
			(v1 ? idx : _ft_impl<idx + 1, vs...>::value);
	};

	/// Implementation of @ref first_true.
	template <std::size_t idx, bool v>
	struct _ft_impl<idx, v> {
		/// The index of the first value set to true.
		static constexpr std::size_t value = (v ? idx : idx + 1);
	};

	/// The index of the value in @e values set to true.
	static constexpr std::size_t value = _ft_impl<0, values...>::value;
};

/// Shorthand for @ref first_true.
template <bool... conds>
static constexpr std::size_t first_true_v = first_true<conds...>::value;

// Some tests
static_assert(first_true_v<true, false, false> == 0);
static_assert(first_true_v<false, true, false> == 1);
static_assert(first_true_v<false, false, true> == 2);
static_assert(first_true_v<false, false, false> == 3);
static_assert(first_true_v<false, false, true, false> == 2);
static_assert(first_true_v<false, false, false, true> == 3);

} // -- namespace detail
} // -- namespace lal

