/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Linear Arrangement Library.
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

// C includes
#include <string.h>

// C++ includes
#include <algorithm>
#include <iterator>

// lal includes
#include <lal/utils/sorting/insertion_sort.hpp>

namespace lal {
namespace utils {

namespace __lal {

/*
 * @brief Sorts the elements within range [begin, end)
 *
 * @param begin Iterator at the beginning of the container.
 * @param end Iterator at the end of the container.
 * @param seen The bit array used to sort. The pointer points at the first
 * element of the array.
 * @pre All values of @e mem must be set to false.
 * @pre All values within [begin, end) must be unique.
 * @post All the values of @e seen are set to false.
 * @post The elements in the range [begin,end) are sorted increasingly.
 */
template<
	typename It,
	typename T = typename std::iterator_traits<It>::value_type,
	typename std::enable_if <std::is_integral<T>::value, int>::type = 0
>
void __bit_sort(It begin, It end, const T& m, char *seen) {
	// fill bit array
	for (auto it = begin; it != end; ++it) {
		seen[*it - m] = 1;
	}

	// pointer to container
	It range = begin;
	// pointer to see
	size_t seenit = 0;
	// element to assign to container
	T i = m;

	while (range != end) {
		// assign value to container
		*range = i;
		range += seen[seenit];

		// move pointer in bit array
		seen[seenit] = 0;
		++i;
		++seenit;
	}
}

} // -- namespace __lal

/*
 * @brief Sort integer values increasingly.
 *
 * @param begin Iterator at the beginning of the container.
 * @param end Iterator at the end of the container.
 * @param seen The bit array used to sort. The pointer points at the first
 * element of the array.
 * @pre All values of @e mem must be set to false.
 * @pre All values within [begin, end) must be unique.
 * @post All the values of @e seen are set to false.
 * @post The elements in the range [begin,end) are sorted increasingly.
 */
template<
	typename It,
	typename T = typename std::iterator_traits<It>::value_type,
	typename std::enable_if <std::is_integral<T>::value, int>::type = 0
>
void bit_sort_mem(It begin, It end, char *seen)
{
	size_t size = std::distance(begin, end);
	if (size <= 1) { return; }
	if (size <= 14) {
		insertion_sort(begin, end);
		return;
	}
	if (size <= 30) {
		std::sort(begin, end);
		return;
	}

	// sort
	__lal::__bit_sort(begin,end, static_cast<T>(0), seen);
}

/*
 * @brief Sort integer values increasingly.
 *
 * @param begin Iterator at the beginning of the container.
 * @param end Iterator at the end of the container.
 * @pre All values within [begin, end) must be unique.
 * @post The elements in the range [begin,end) are sorted increasingly.
 */
template<
	typename It,
	typename T = typename std::iterator_traits<It>::value_type,
	typename std::enable_if<std::is_integral<T>::value, int>::type = 0
>
void bit_sort(It begin, It end)
{
	size_t size = std::distance(begin, end);
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
	char *seen = static_cast<char *>(malloc( (M - m + 1)*sizeof(char) ));
	memset(seen, 0, (M - m + 1)*sizeof(char) );

	// sort
	__lal::__bit_sort(begin,end, m, seen);

	free(seen);
}

} // -- namspace utils
} // -- namespace lal
