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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <algorithm>
#include <iterator>

namespace lal {
namespace utils {

namespace __lal {

template<
	typename It,
	typename T = typename std::iterator_traits<It>::value_type,
	typename std::enable_if <std::is_integral<T>::value, int>::type = 0
>
void __bit_sort(It begin, It end, const T& m, std::vector<bool>& seen) {
	// fill bit vector
	for (auto it = begin; it != end; ++it) {
		seen[*it - m] = true;
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

		// move pointer in bit vector
		seen[seenit] = false;
		++i;
		++seenit;
	}
}

} // -- namespace __lal

// Insertion sort
template<typename It>
void insertion_sort(It begin, It end) {
	for (It i = begin + 1; i != end; ++i) {
		It nj = i;
		It j = i - 1;
		while (*j > *nj and j != begin) {
			std::swap(*j, *nj);
			--j;
			--nj;
		}
		if (*j > *nj) { std::swap(*j, *nj); }
	}
}

/*
 * @brief Sort integer values increasingly.
 *
 * @param begin Iterator at the beginning of the container.
 * @param end Iterator at the end of the container.
 * @param seen The bit vector used to sort.
 * @pre All values of @e mem must be set to false.
 * @post The elements in the range [begin,end) is sorted. All the values of
 * @e seen are set to false.
 */
template<
	typename It,
	typename T = typename std::iterator_traits<It>::value_type,
	typename std::enable_if <std::is_integral<T>::value, int>::type = 0
>
void bit_sort_mem(It begin, It end, std::vector<bool>& seen)
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
 * @post The elements in the range [begin,end) is sorted.
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

	// maximum & minimum elements within vector
	const auto [m_it,M_it] = std::minmax_element(begin, end);
	const auto m = *m_it;
	const auto M = *M_it;

	// bit vector
	std::vector<bool> seen(M - m + 1, false);

	// sort
	__lal::__bit_sort(begin,end, m, seen);
}

} // -- namspace utils
} // -- namespace lal
