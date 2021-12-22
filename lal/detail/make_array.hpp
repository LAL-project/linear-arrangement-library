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
 ********************************************************************/

#pragma once

// C++ includes
#include <array>

namespace lal {
namespace detail {

namespace __lal {

/// Function used by @ref lal::detail::make_array_with_value().
template<typename T, std::size_t size, T v, std::size_t... I>
std::array<T, size> make_array_with_value_impl(std::index_sequence<I...>) {
	std::array<T, size> a;
	((a[I] = v), ...);
	return a;
}

} // -- namespace __lal

/**
 * @brief Returns an array initialised at a given value.
 * @tparam T Type of the array's elements.
 * @tparam array_size Size of the array.
 * @tparam value_to_fill_with The value to fill the array with.
 */
template<typename T, std::size_t array_size, T value_to_fill_with>
std::array<T, array_size>
make_array_with_value() {
	return
	__lal::make_array_with_value_impl
	<T, array_size, value_to_fill_with>
	(std::make_integer_sequence<std::size_t, array_size>{});
}

/**
 * @brief Make an array with the values given as parameters of the template.
 * @tparam T Type of the array
 * @tparam ARGS List of values to be stored in the array.
 */
template<typename T, T... ARGS>
constexpr std::array<T, sizeof...(ARGS)> make_array() noexcept {
	return std::array<T, sizeof...(ARGS)>{ARGS...};
}

} // -- namespace detail
} // -- namespace lal
