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

namespace lal {
namespace detail {
namespace sorting {

/// Namespace that contains specific methods and types used only by the radix sort algorithm.
namespace radixsort {

/**
 * @brief Move the elements in the buckets to the actual array.
 * @tparam value_t A type for an array (or vector) of values.
 * @param buckets The series of buckets with the sorted elements.
 * @param queue The array with the elements to be sorted.
 */
template <typename value_t>
void from_buckets_to_queue(
	array<std::vector<value_t>>& buckets, array<value_t>& queue
)
{
	std::size_t j = 0;
	for (std::size_t i = 0; i < buckets.size(); ++i) {
		for (value_t& k : buckets[i]) {
			queue[j++] = std::move(k);
		}
		buckets[i].clear();
	}
}

} // namespace radixsort

/**
 * @brief Radix sort algorithm adapted to arbitrary lists of elements.
 * @tparam type The type of ordering.
 * @tparam value_t A type for an array (or vector) of values.
 * @param queue The array with the elements to be sorted.
 * @param max_value An upper bound of the maximum value over all elements in
 * every list of @e queue.
 * @param max_length An upper bound of the length of the longest list in @e queue.
 */
template <sort_type type, typename value_t>
void radix_sort(
	array<value_t>& queue,
	const std::size_t max_value,
	const std::size_t max_length
)
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Aho1974a);
#endif

	array<std::vector<value_t>> buckets(max_value + 1);

	for (std::size_t j = max_length; j >= 1; --j) {

		for (std::size_t i = 0; i < queue.size(); ++i) {
			const std::size_t elem =
				(j - 1 < queue[i].size() ? queue[i][j - 1] : 0);

			buckets[elem].emplace_back(std::move(queue[i]));
		}

		radixsort::from_buckets_to_queue<value_t>(buckets, queue);
	}

	if constexpr (type == sort_type::non_increasing) {
		std::reverse(queue.begin(), queue.end());
	}
}

/**
 * @brief Radix sort algorithm adapted to arbitrary lists of elements.
 * @tparam type The type of ordering.
 * @tparam value_t A type for an array (or vector) of values.
 * @param queue The array with the elements to be sorted.
 */
template <sort_type type, typename value_t>
void radix_sort(array<value_t>& queue)
{
	std::size_t max_value = 0;
	std::size_t max_length = 0;
	for (const value_t& q : queue) {
		max_length = std::max(q.size(), max_length);
		for (std::size_t i = 0; i < q.size(); ++i) {
			max_value = std::max(max_value, q[i]);
		}
	}
	radix_sort<type, value_t>(queue, max_value, max_length);
}

} // namespace sorting
} // namespace detail
} // namespace lal
