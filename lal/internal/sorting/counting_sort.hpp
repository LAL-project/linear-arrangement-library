/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#include <algorithm>
#include <type_traits>

// lal includes
#include <lal/internal/data_array.hpp>

namespace lal {
namespace internal {

template<typename T>
struct memory_counting_sort {
	data_array<std::size_t> count;
	data_array<T> output;

	memory_counting_sort(
		const std::size_t largest_key_plus_1,
		const std::size_t max_size_container
	) :
	count(largest_key_plus_1, 0), output(max_size_container)
	{ }

	~memory_counting_sort() { }

	void reset_count() { count.fill(0); }
};

/*
 * @brief Counting sort algorithm with reusable memory.
 *
 * This algorithm is interesting for sorting containers with non-unique values.
 * For details on the algorithm, see https://en.wikipedia.org/wiki/Counting_sort
 *
 * Template parameters:
 * @param T Iterated type
 * @param It Iterator type
 * @param increasing: when true, the result is sorted increasingly. When false,
 * the result is sorted decreasingly.
 *
 * Function paremeters:
 * @param begin Iterator at the beginning of the range.
 * @param end Iterator at the end of the range.
 * @param largest_key_plus_1 Integer value equal to 1 + the largest key that can
 * be obtained with function @e key.
 * @param key Function that returns a single integer value used to compare the
 * elements.
 * @post The elements in the range [begin,end) are sorted increasingly.
 */
template<
	typename T, typename It,
	bool increasing
>
void counting_sort(
	It begin, It end,
	const std::size_t largest_key_plus_1,
	const std::function<std::size_t (const T&)>& key,
	memory_counting_sort<T>& mem
)
{
	// nothing to do if there are no elements to sort
	if (begin == end) { return; }

	// calculate frequency of each element
	for (auto it = begin; it != end; ) {
		const std::size_t elem_key = key(*(it++));
		mem.count[elem_key] += 1;
	}

	std::size_t total = 0;
	for (std::size_t k = 0; k < largest_key_plus_1; ++k) {
		std::tie(mem.count[k], total)
			= std::make_pair(total, mem.count[k] + total);
	}

	std::size_t size_container = 0;
	for (auto it = begin; it != end; ) {
		++size_container;

		const std::size_t elem_key = key(*it);
		mem.output[mem.count[elem_key]] = std::move(*(it++));
		mem.count[elem_key] += 1;
	}

	// calculate output
	if constexpr (increasing) {
		auto it = begin;
		for (std::size_t k = 0; k < size_container;) {
			*(it++) = std::move(mem.output[k++]);
		}
	}
	else {
		auto it = begin;
		for (std::size_t k = size_container - 1; k > 0;) {
			*(it++) = std::move(mem.output[k--]);
		}
		*it = std::move(mem.output[0]);
	}
}

/*
 * @brief Counting sort algorithm.
 *
 * This algorithm is interesting for sorting containers with non-unique values.
 * For details on the algorithm, see https://en.wikipedia.org/wiki/Counting_sort
 *
 * Template parameters:
 * @param T Iterated type
 * @param It Iterator type
 * @param increasing: when true, the result is sorted increasingly. When false,
 * the result is sorted decreasingly.
 *
 * Function paremeters:
 * @param begin Iterator at the beginning of the range.
 * @param end Iterator at the end of the range.
 * @param M Integer value equal to the largest key that can be obtained with
 * function @e key.
 * @param upper_bound_size An upper bound of he size of the container to be
 * sorted. The lowest value is std::distance(begin,end), the actual size of the
 * container.
 * @param key Function that returns a single integer value used to compare the
 * elements.
 * @post The elements in the range [begin,end) are sorted increasingly.
 */
template<
	typename T, typename It,
	bool increasing
>
void counting_sort(
	It begin, It end,
	const std::size_t largest_key,
	const std::size_t upper_bound_size,
	const std::function<std::size_t (const T&)>& key
)
{
	// nothing to do if there are no elements to sort
	if (begin == end) { return; }

	memory_counting_sort<T> mem(largest_key+1, upper_bound_size);

	counting_sort<T, It, increasing>
	(begin, end, largest_key+1, key, mem);

	// memory is freed automatically (see destructor)
}

} // -- namespace internal
} // -- namespace lal
