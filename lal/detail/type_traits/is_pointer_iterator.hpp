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

// C*+ includes
#include <type_traits>
#include <iterator>

namespace lal {
namespace detail {

/**
 * @brief A concept for pointers
 * @tparam pointer_t The iterator type.
 * @tparam value_t The iterated type.
 */
template <typename pointer_t, typename value_t>
concept Pointer = std::is_pointer_v<pointer_t> and
				  std::is_same_v<std::remove_pointer_t<pointer_t>, value_t>;

/**
 * @brief A concept for iterators
 * @tparam iterator_t The iterator type.
 * @tparam value_t The iterated type.
 */
template <typename iterator_t, typename value_t>
concept Iterator = std::
	is_same_v<typename std::iterator_traits<iterator_t>::value_type, value_t>;

/**
 * @brief A concept for pointers or iterators
 * @tparam iterator_t The iterator type.
 * @tparam value_t The iterated type.
 */
template <typename iterator_t, typename value_t>
concept PointerOrIterator =
	Pointer<iterator_t, value_t> or Iterator<iterator_t, value_t>;

} // namespace detail
} // namespace lal
