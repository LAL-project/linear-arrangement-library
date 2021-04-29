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
#include <array>

namespace lal {
namespace internal {

namespace __lal {

template<typename T, size_t size, T v, std::size_t... I>
std::array<T, size> make_array_with_value_impl(std::index_sequence<I...>) {
	std::array<T, size> a;
	((a[I] = v), ...);
	return a;
}

} // -- namespace __lal

/*
 * @brief Returns an array initialised at a given value.
 *
 * @param T Type of the array's elements.
 * @param array_size Size of the array.
 * @param value_to_fill_with The value to fill the array with.
 */
template<typename T, size_t array_size, T value_to_fill_with>
std::array<T, array_size>
make_array_with_value() {
	return
	__lal::make_array_with_value_impl
	<T, array_size, value_to_fill_with>
	(std::make_integer_sequence<size_t, array_size>{});
}

namespace __lal {

template<typename T, T A, T... ARGS>
constexpr std::size_t size_arguments = 1 + size_arguments<T, ARGS...>;
template<typename T, T A>
constexpr std::size_t size_arguments<T,A> = 1;

} // -- namespace __lal

template<typename T, T... ARGS>
constexpr std::array<T, __lal::size_arguments<T, ARGS...>>
make_array()
noexcept
{
	return std::array<T, __lal::size_arguments<T, ARGS...>>
			{ARGS...};
}

} // -- namespace internal
} // -- namespace lal
