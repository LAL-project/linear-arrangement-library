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

// C includes
#include <string.h>

// C++ includes
#include <functional>
#include <vector>

// lal includes
#include <lal/utils/sorting/insertion_sort.hpp>

namespace lal {
namespace utils {

/*
 * @brief Counting sort algorithm.
 *
 * This algorithm is interesting for sorting containers with non-unique values.
 * For details on the algorithm, see https://en.wikipedia.org/wiki/Counting_sort
 *
 * @param begin Iterator at the beginning of the range.
 * @param end Iterator at the end of the range.
 * @param M Integer value that has to be, at least, one unit larger than the
 * largest key obtained with function @e key.
 * @param key Function that returns an integer value used to compare the elements.
 * @post The elements in the range [begin,end) are sorted increasingly.
 */
template<
	typename It,
	typename T = typename std::iterator_traits<It>::value_type
>
void counting_sort(
	It begin, It end, const size_t M, const std::function<size_t (const T&)>& key
)
{
	const size_t S = std::distance(begin, end);

	// allocate memory
	T *output = static_cast<T *>(malloc(S*sizeof(T)));
	size_t *count = static_cast<size_t *>(malloc(M*sizeof(size_t)));

	// initialise memory (when needed)
	memset(count, 0, M*sizeof(size_t));

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
	it = begin;
	for (size_t k = 0; k < S; ++k, ++it) { *it = output[k]; }

	// free memory
	free(output);
	free(count);
}

} // -- namespace utils
} // -- namespace lal
