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
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <utility>

namespace lal {
namespace detail {
namespace sorting {

/**
 * @brief Insertion sort.
 * @param begin Iterator at the beginning of the container.
 * @param end Iterator at the end of the container.
 * @post The elements in the range [begin,end) are sorted increasingly.
 */
template <typename iterator_t>
void insertion_sort(const iterator_t begin, const iterator_t end) noexcept
{
	// TODO: do not std::swap elements in order to avoid copies
	// and improve efficiency
	if (begin == end) {
		return;
	}
	for (iterator_t i = begin + 1; i != end; ++i) {
		iterator_t nj = i;
		iterator_t j = i - 1;
		while (*j > *nj and j != begin) {
			std::swap(*j, *nj);
			--j;
			--nj;
		}
		if (*j > *nj) {
			std::swap(*j, *nj);
		}
	}
}

} // namespace sorting
} // namespace detail
} // namespace lal
