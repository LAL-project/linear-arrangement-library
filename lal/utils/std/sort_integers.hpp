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
 * @brief Sort integer values within the range \f$[m,M)\f$ increasingly.
 *
 * The values \f$m,M\f$ represent the minimum and maximum values within the
 * vector, respectively.
 * @param begin Iterator at the beginning of the container.
 * @param end Iterator at the end of the container.
 * @post v is sorted.
 */
template<typename It>
typename std::enable_if
<
	std::is_integral< typename std::iterator_traits<It>::value_type >::value,
	void
>
::type
sort_1_n_inc(It begin, It end)
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

	// fill "bit" vector
	std::vector<bool> seen(M - m + 1, false);
	for (auto it = begin; it != end; ++it) {
		seen[*it - m] = true;
	}

	// pointer to the bit vector
	auto seenit = seen.begin();
	// pointer to [begin,end)
	auto vit = begin;

	// sort elements, increasingly
	typename std::iterator_traits<It>::value_type i;
	for (i = 0; i < M - m + 1 and vit != end; ++i, ++seenit) {
		*vit = i + m;
		vit += *seenit;
	}
}

/*
 * @brief Sort integer values within the range \f$[m,M)\f$ increasingly.
 *
 * The values \f$m,M\f$ represent the minimum and maximum values within the
 * vector, respectively.
 * @param begin Iterator at the beginning of the container.
 * @param end Iterator at the end of the container.
 * @param mem The "bit" vector used to sort. Its size must be equal to
 * *std::max(begin, end).
 * @param min The minimum value that the values in [begin,end) @e could take
 * (not necessarily equal to *std::min(begin,end)).
 * @pre All values of @e mem must be set to false.
 * @post v is sorted. All the values of @e mem are set to false.
 */
template<typename It>
typename std::enable_if
<
	std::is_integral< typename std::iterator_traits<It>::value_type >::value,
	void
>
::type
sort_1_n_inc_mem(
	It begin, It end,
	std::vector<bool>& seen,
	const typename std::iterator_traits<It>::value_type min
)
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

	// fill "bit" vector
	for (auto it = begin; it != end; ++it) {
		seen[*it] = true;
	}

	// pointer to [begin,end)
	auto vit = begin;

	// sort elements, increasingly
	typename std::iterator_traits<It>::value_type i = min;
	for (auto seenit = seen.begin(); seenit != seen.end(); ++seenit) {
		*vit = i;
		vit += *seenit;
		*seenit = 0;
		++i;
	}
}

} // -- namspace utils
} // -- namespace lal
