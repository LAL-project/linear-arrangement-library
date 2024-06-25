/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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

#include <algorithm>

namespace lal {
namespace detail {

/**
 * @brief Finds an element within the sorted range [@e begin, @e end).
 * @tparam iterator_t Iterator type.
 * @tparam value_t Value type.
 * @param begin Pointer at the first element of the range.
 * @param end Pointer at the last element of the range.
 * @param size The number of elements within the range.
 * @param v The value to search for.
 * @param min_size The minimum number of elements for the function to choose
 * std::lower_bound over std::find.
 * @returns An iterator to the element @e v within the input range.
 * @pre The range [@e begin, @e end) is entirely sorted.
 */
template <typename iterator_t, typename value_t>
[[nodiscard]] inline iterator_t find_sorted
(
	const iterator_t begin, const iterator_t end, const std::size_t size,
	const value_t& v, const std::size_t min_size = 64
)
noexcept
{
	return size < min_size ?
		std::find(begin, end, v) :
		std::lower_bound(begin, end, v);
}

/**
 * @brief Finds an element within the sorted range [@e begin, @e end).
 * @tparam iterator_t Iterator type.
 * @tparam value_t Value type.
 * @param begin Pointer at the first element of the range.
 * @param end Pointer at the last element of the range.
 * @param size The number of elements within the range.
 * @param v The value to search for.
 * @param min_size The minimum number of elements for the function to choose
 * std::lower_bound over std::find.
 * @returns Whether @e v exists in the range or not.
 * @pre The range [@e begin, @e end) is entirely sorted.
 */
template <typename iterator_t, typename value_t>
[[nodiscard]] inline bool exists_sorted
(
	const iterator_t begin, const iterator_t end, const std::size_t size,
	const value_t& v, const std::size_t min_size = 64
)
noexcept
{
	return size < min_size ?
	   std::find(begin, end, v) != end :
	   std::binary_search(begin, end, v);
}

} // -- namespace lal
} // -- namespace detail
