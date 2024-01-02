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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
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
#include <algorithm>

// lal includes
#include <lal/detail/data_array.hpp>
#include <lal/detail/sorting/insertion_sort.hpp>
#include <lal/detail/type_traits/is_pointer_iterator.hpp>

namespace lal {
namespace detail {
namespace sorting {

/**
 * @brief Sorts the elements within range [@e begin, @e end)
 * @tparam T The type of the values ot be sorted.
 * @tparam iterator_t The type of iterator over the range [@e being, @e end)
 * @param begin Iterator at the beginning of the container.
 * @param end Iterator at the end of the container.
 * @param m Smallest value in the range to sort.
 * @param seen The bit array used to sort.
 * @pre All values of @e mem are set to false.
 * @pre All values within [begin, end) are unique.
 * @post All the values of @e seen are set to false.
 * @post The elements in the range [begin,end) are sorted increasingly.
 */
template <
	typename T, typename iterator_t,
	std::enable_if_t<
		std::is_integral_v<T> && is_pointer_iterator_v<T, iterator_t>,
		bool
	> = true
>
void bit_sort(iterator_t begin, iterator_t end, const T& m, char * const seen)
noexcept
{
	// fill bit array
	for (auto it = begin; it != end; ++it) {
		seen[*it - m] = 1;
	}

	// pointer to see
	std::size_t seenit = 0;
	// element to assign to container
	T i = m;

	// pointer to container
	iterator_t it = begin;
	while (it != end) {
		// assign value to container
		*it = i;
		it += seen[seenit];

		// move pointer in bit array
		seen[seenit] = 0;
		++i;
		++seenit;
	}
}

/**
 * @brief Sort integer values increasingly.
 * @tparam T The type of the values ot be sorted.
 * @tparam It The type of iterator over the range [@e being, @e end)
 * @param begin Iterator at the beginning of the container.
 * @param end Iterator at the end of the container.
 * @param size The value of std::distance(begin, end), i.e., the number of
 * elements from begin to sort, i.e., the size of the container.
 * @param seen The bit array used to sort.
 * @pre All values of @e mem must be set to 0.
 * @pre All values within [begin, end) must be unique.
 * @post All the values of @e seen are set to 0.
 * @post The elements in the range [begin,end) are sorted increasingly.
 */
template <
	typename T, typename It,
	std::enable_if_t<
		std::is_integral_v<T> && is_pointer_iterator_v<T, It>,
		bool
	> = true
>
void bit_sort_mem(It begin, It end, const std::size_t size, char * const seen)
noexcept
{
	if (size <= 1) { return; }
	if (size <= 14) {
		insertion_sort(begin, end);
		return;
	}
	if (size <= 30) {
		std::sort(begin, end);
		return;
	}

	bit_sort(begin,end, T{0}, seen);
}

/**
 * @brief Sort integer values increasingly.
 * @tparam T The type of the values ot be sorted.
 * @tparam It The type of iterator over the range [@e being, @e end)
 * @param begin Iterator at the beginning of the container.
 * @param end Iterator at the end of the container.
 * @param size The value of std::distance(begin, end), i.e., the number of
 * elements from begin to sort, i.e., the size of the container.
 * @pre All values within [begin, end) must be unique.
 * @post The elements in the range [begin,end) are sorted increasingly.
 */
template <
	typename T, typename It,
	std::enable_if_t<
		std::is_integral_v<T> && is_pointer_iterator_v<T, It>,
		bool
	> = true
>
void bit_sort(It begin, It end, const std::size_t size) noexcept
{
	if (size <= 1) { return; }
	if (size <= 14) {
		insertion_sort(begin, end);
		return;
	}
	if (size <= 30) {
		std::sort(begin, end);
		return;
	}

	// maximum & minimum elements within array
	const auto [m_it,M_it] = std::minmax_element(begin, end);
	const auto m = *m_it;
	const auto M = *M_it;

	// bit array
	data_array<char> seen(M - m + 1, 0);

	bit_sort(begin,end, m, seen.begin());
}

} // -- namespace sorting
} // -- namspace utils
} // -- namespace lal
