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
#include <functional>
#include <tuple>

// lal includes
#include <lal/detail/data_array.hpp>
#include <lal/detail/macros/is_pointer_iterator.hpp>
#include <lal/detail/sorting/sorting_types.hpp>

namespace lal {
namespace detail {
namespace sorting {

/**
 * @brief Types used only by the counting sort algorithm.
 *
 * For details on the algorithm, see https://en.wikipedia.org/wiki/Counting_sort
 * and \cite Cormen2001a.
 */
namespace countingsort {

/**
 * @brief Memory used for the counting sort algorithm.
 *
 * This allows users of the algorithm to initialize it outside the algorithm,
 * thus being able to save some time.
 */
template <typename T>
struct memory {
	/// Amount of times the key of an element occurs.
	data_array<std::size_t> count;
	/// The output array.
	data_array<T> output;

	/// Constructor.
	memory() noexcept = default;
	/// Copy constructor.
	memory(const memory&) noexcept = default;
	/// Move constructor.
	memory(memory&&) noexcept = default;
	/// Copy assignment operator.
	memory& operator= (const memory&) noexcept = default;
	/// Move assignment operator.
	memory& operator= (memory&&) noexcept = default;

	/**
	 * @brief Constructor with largest key (+1) and maximum container size.
	 * @param largest_key_plus_1 The value of the largest key over all elements
	 * to be sorted (+1).
	 * @param max_size_container The size of the container, equal to the largest
	 * key over all elements to be sorted. This is typically equal to @e largest_key_plus_1
	 * but not necessarily.
	 */
	memory(const std::size_t largest_key_plus_1, const std::size_t max_size_container)
	noexcept
		: count(largest_key_plus_1, 0),
		  output(max_size_container)
	{ }

	/// Destructor.
	~memory() noexcept = default;

	/// Set the @ref count member to 0.
	void reset_count() { count.fill(0); }
};

} // -- namespace countingsort

/**
 * @brief Counting sort algorithm with reusable memory.
 *
 * This algorithm is interesting for sorting containers with non-unique values
 * that can be easily mapped to integers within a reasonable range, e.g., in the
 * range [1,n]. For details on the algorithm, see https://en.wikipedia.org/wiki/Counting_sort
 * and \cite Cormen2001a.
 *
 * @tparam value_t Iterated type
 * @tparam sort_type One of @ref lal::detail::sorting::non_increasing_t or
 * @ref lal::detail::sorting::non_decreasing_t.
 * @tparam memory_has_frequencies The memory passed as parameter already conatins
 * the frequencies for the counting sort algorithm. See code for more details.
 * @tparam value_iterator_t Iterator type
 *
 * Function paremeters:
 * @param begin Iterator at the beginning of the range.
 * @param end Iterator at the end of the range.
 * @param largest_key_plus_1 Integer value equal to 1 + the largest key that can
 * be obtained with function @e key.
 * @param key Function that returns a single integer value used to compare the
 * elements.
 * @param mem Reusable memory for the counting sort algorithm.
 * @post The elements in the range [begin,end) are sorted according to the sorting
 * criterion.
 * @post The function @e key is called exactly twice for each element in the
 * range to be sorted.
 */
template <
	typename value_t,
	typename sort_type,
	bool memory_has_frequencies,
	typename value_iterator_t,
	std::enable_if_t<
		is_pointer_iterator_v<value_t, value_iterator_t> &&
		(
		std::is_same_v<sort_type, non_decreasing_t> ||
		std::is_same_v<sort_type, non_increasing_t>
		),
		bool
	> = true
>
void counting_sort(
	const value_iterator_t begin, const value_iterator_t end,
	const std::size_t largest_key_plus_1,
	const std::function<std::size_t (const value_t&)>& key,
	countingsort::memory<value_t>& mem
)
noexcept
{
	constexpr bool is_increasing =
		std::is_same_v<sort_type, non_decreasing_t>;

	// nothing to do if there are no elements to sort
	if (begin == end) { return; }

	if constexpr (not memory_has_frequencies) {
		// calculate frequency of each element
		for (auto it = begin; it != end; ) {
			// get the key of the element into a variable so that
			// the function is NOT called more than once per iteration
			const std::size_t elem_key = key(*(it++));

			++mem.count[elem_key];
		}
	}

	std::size_t total = 0;
	for (std::size_t k = 0; k < largest_key_plus_1; ++k) {
		std::tie(mem.count[k], total)
			= std::make_pair(total, mem.count[k] + total);
	}

	std::size_t actual_container_size = 0;
	for (auto it = begin; it != end; ) {
		++actual_container_size;

		// get the key of the element into a variable so that
		// the function is NOT called more than once per iteration
		const std::size_t elem_key = key(*it);

		mem.output[mem.count[elem_key]] = std::move(*(it++));
		++mem.count[elem_key];
	}

	// calculate output
	if constexpr (is_increasing) {
		auto it = begin;
		for (std::size_t k = 0; k < actual_container_size;) {
			*(it++) = std::move(mem.output[k++]);
		}
	}
	else {
		auto it = begin;
		for (std::size_t k = actual_container_size - 1; k > 0;) {
			*(it++) = std::move(mem.output[k--]);
		}
		*it = std::move(mem.output[0]);
	}
}

/**
 * @brief Counting sort algorithm.
 *
 * This algorithm is interesting for sorting containers with non-unique values
 * that can be easily mapped to integers within a reasonable range, e.g., in the
 * range [1,n]. For details on the algorithm, see https://en.wikipedia.org/wiki/Counting_sort
 *
 * Template parameters:
 * @tparam value_t Type of the values sorted.
 * @tparam sort_type One of @ref lal::detail::sorting::non_increasing_t or
 * @ref lal::detail::sorting::non_decreasing_t.
 * @tparam value_iterator_t Iterator over type 'value_t' type.
 *
 * Function paremeters:
 * @param begin Iterator at the beginning of the range.
 * @param end Iterator at the end of the range.
 * @param largest_key Integer value equal to the largest key that can be
 * obtained with function @e key.
 * @param upper_bound_size An upper bound of he size of the container to be
 * sorted. The lowest value is std::distance(begin,end), the actual size of the
 * container.
 * @param key Function that returns a single integer value used to compare the
 * elements.
 * @post The elements in the range [begin,end) are sorted according to the sorting
 * criterion.
 * @post The function @e key is called exactly twice for each element in the
 * range to be sorted.
 */
template <
	typename value_t,
	typename sort_type,
	typename value_iterator_t,
	std::enable_if_t<
		is_pointer_iterator_v<value_t, value_iterator_t> &&
		(
		std::is_same_v<sort_type, non_decreasing_t> ||
		std::is_same_v<sort_type, non_increasing_t>
		),
		bool
	> = true
>
void counting_sort(
	const value_iterator_t begin, const value_iterator_t end,
	const std::size_t largest_key,
	const std::size_t upper_bound_size,
	const std::function<std::size_t (const value_t&)>& key
)
noexcept
{
	// nothing to do if there are no elements to sort
	if (begin == end) { return; }

	countingsort::memory<value_t> mem(largest_key+1, upper_bound_size);

	counting_sort<value_t, sort_type, false, value_iterator_t>
		(begin, end, largest_key+1, key, mem);
}

} // -- namespace sorting
} // -- namespace detail
} // -- namespace lal
