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
#include <cstdint>

namespace lal {
namespace detail {

/// Conversion to uint64_t.
template <typename T>
[[nodiscard]] constexpr uint64_t to_uint64(const T& t) noexcept
{
	return static_cast<uint64_t>(t);
}

/// Conversion to int64_t.
template <typename T>
[[nodiscard]] constexpr int64_t to_int64(const T& t) noexcept
{
	return static_cast<int64_t>(t);
}

/// Conversion to uint32_t.
template <typename T>
[[nodiscard]] constexpr uint32_t to_uint32(const T& t) noexcept
{
	return static_cast<uint32_t>(t);
}

/// Conversion to int32_t.
template <typename T>
[[nodiscard]] constexpr int32_t to_int32(const T& t) noexcept
{
	return static_cast<int32_t>(t);
}

/// Conversion to double.
template <typename T>
[[nodiscard]] constexpr double to_double(const T& t) noexcept
{
	return static_cast<double>(t);
}

/// Absolute difference of two values.
template <typename T>
[[nodiscard]] constexpr T abs_diff(const T& t1, const T& t2) noexcept
{
	return (t1 < t2 ? t2 - t1 : t1 - t2);
}

} // namespace detail
} // namespace lal
