/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
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
 ********************************************************************/

#pragma once

// C++ includes
#include <algorithm>
#include <vector>

// lal includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/detail/array.hpp>
#include <lal/detail/sorting/sorting_types.hpp>
#include <lal/detail/sorting/counting_sort.hpp>

namespace lal {
namespace detail {
namespace sorting {

/**
 * @brief Radix sort algorithm adapted to arbitrary lists of elements.
 *
 * This implementation may not be appropriate when @e value_t is a number type.
 * @tparam type The type of ordering.
 * @tparam value_t The values to be sorted.
 * @param queue The array with the elements to be sorted.
 * @param max_value An upper bound of the maximum value over all elements in
 * every list of @e queue.
 * @param max_length An upper bound of the length of the longest list in @e queue.
 */
template <sort_type type, typename value_t, typename Callable>
void radix_sort(
	array<value_t>& queue,
	const std::size_t max_value,
	const std::size_t max_length,
	const Callable& digit
)
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Aho1974a);
#endif

	countingsort::memory<value_t> mem(max_value + 1, queue.size());

	for (std::size_t j = max_length; j >= 1; --j) {
		counting_sort<sort_type::non_decreasing, false>(
			queue.begin(),
			queue.end(),
			[&](const value_t& v)
			{
				return digit(v, j - 1);
			},
			mem
		);
		mem.reset_count();
	}

	if constexpr (type == sort_type::non_increasing) {
		std::reverse(queue.begin(), queue.end());
	}
}

/**
 * @brief Radix sort algorithm adapted to arbitrary lists of elements.
 *
 * This implementation may not be appropriate when @e value_t is a number type.
 * @tparam type The type of ordering.
 * @tparam value_t A type for an array (or vector) of values.
 * @param queue The array with the elements to be sorted.
 */
template <sort_type type, typename value_t, typename Callable>
void radix_sort(array<value_t>& queue, const Callable& digit)
{
	std::size_t max_value = 0;
	std::size_t max_length = 0;
	for (const value_t& q : queue) {
		max_length = std::max(q.size(), max_length);
		for (std::size_t i = 0; i < q.size(); ++i) {
			max_value = std::max(max_value, q[i]);
		}
	}
	radix_sort<type, value_t>(queue, max_value, max_length, digit);
}

} // namespace sorting
} // namespace detail
} // namespace lal
