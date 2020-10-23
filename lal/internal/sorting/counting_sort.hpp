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
#include <functional>

namespace lal {
namespace internal {

/*
 * @brief Counting sort algorithm.
 *
 * This algorithm is interesting for sorting containers with non-unique values.
 * For details on the algorithm, see https://en.wikipedia.org/wiki/Counting_sort
 *
 * Template parameters:
 * @param It Iterator type
 * @param T Iterated type
 * @param increasing: when true, the result is sorted increasingly. When false,
 * the result is sorted decreasingly.
 *
 * Function paremeters:
 * @param begin Iterator at the beginning of the range.
 * @param end Iterator at the end of the range.
 * @param _M Integer value equal to the largest key that can be obtained with
 * function @e key.
 * @param S The size of the container to be sorted. This is equal to
 * std::distance(begin,end).
 * @param key Function that returns a single integer value used to compare the
 * elements.
 * @post The elements in the range [begin,end) are sorted increasingly.
 */
template<
	typename It,
	typename T = typename std::iterator_traits<It>::value_type,
	bool increasing
>
void counting_sort(
	It begin, It end, const size_t _M, const size_t S,
	const std::function<size_t (const T&)>& key
)
{
	// nothing to do if there are no elements to sort
	if (begin == end) { return; }

	// increase
	const size_t M = _M + 1;

	// allocate memory
	T *output = new T[S];
	size_t *count = new size_t[M]{0};

	// calculate frequency of each element
	for (auto it = begin; it != end; ++it) {
		const size_t elem_key = key(*it);
		count[elem_key] += 1;
	}

	size_t total = 0;
	for (size_t k = 0; k < M; ++k) {
		std::tie(count[k], total)
			= std::make_pair(total, count[k] + total);
	}
	auto it = begin;
	for (; it != end; ++it) {
		const size_t elem_key = key(*it);
		output[count[elem_key]] = *it;
		count[elem_key] += 1;
	}

	// calculate output
	if constexpr (increasing) {
		it = begin;
		for (size_t k = 0; k < S; ++k, ++it) { *it = output[k]; }
	}
	else {
		it = begin;
		for (size_t k = S - 1; k > 0; --k, ++it) {
			*it = output[k];
		}
		*it = output[0];
	}

	// free memory
	delete[] output;
	delete[] count;
}

} // -- namespace internal
} // -- namespace lal
