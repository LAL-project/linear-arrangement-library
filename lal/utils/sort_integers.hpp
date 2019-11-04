/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
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
#include <vector>

namespace lal {
namespace macros {

/*
 * @brief Insertion sort.
 */
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
 * @brief Sort integer values within the range [0,n)
 *
 * The range was named [1,n] so as to avoid naming it as "sort_0_n",
 * since it is quite misleading.
 * @param v Vector to be sorted.
 * @param n Upper bound of the interval.
 * @post v is sorted.
 */
template<typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
sort_1_n(std::vector<T>& v, size_t n)
{
	if (v.size() <= 1) { return; }
	if (v.size() <= 14) {
		insertion_sort(v.begin(), v.end());
		return;
	}
	if (v.size() <= 30) {
		std::sort(v.begin(), v.end());
		return;
	}

	std::vector<bool> seen(n, false);
	for (auto it = v.begin(); it != v.end(); ++it) {
		seen[*it] = true;
	}

	auto seenit = seen.begin();
	auto vit = v.begin();
	for (T i = 0; i < n and vit != v.end(); ++i, ++seenit) {
		*vit = i;
		vit += *seenit;
	}
}

} // -- namspace macros
} // -- namespace lal
