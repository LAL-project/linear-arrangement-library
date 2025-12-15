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
 ********************************************************************/

#pragma once

#if defined LAL_INTERFACE_PYTHON || defined LAL_COMPILE_PYTHON
#include <string>
#else
#include <string_view>
#endif

namespace lal {
namespace utilities {

#if defined LAL_INTERFACE_PYTHON || defined LAL_COMPILE_PYTHON
typedef std::string string_t;
#else
typedef std::string_view string_t;
#endif

/**
 * @brief A decorator for formatted output.
 *
 * This is a helper class to more easily write formatted multiline output.
 * @tparam T The thing that we want to output.
 */
template <typename T>
struct decorator {
	/// Prefix string.
	string_t prefix;
	/// Value to be written into output.
	const T *value;
	/// Suffix string.
	string_t suffix;
};

namespace decorator_operators {

template <typename T>
[[nodiscard]] static inline decorator<T>
operator+ (string_t w, const T& t) noexcept
{
	return decorator<T>{.prefix = w, .value = &t, .suffix = ""};
}

template <typename T>
[[nodiscard]] static inline decorator<T>
operator+ (const T& t, string_t w) noexcept
{
	return decorator<T>{.prefix = "", .value = &t, .suffix = w};
}

template <typename T>
[[nodiscard]] static inline decorator<T>
operator+ (decorator<T> t, string_t w) noexcept
{
	t.suffix = w;
	return t;
}

template <typename T>
[[nodiscard]] static inline decorator<T>
operator+ (string_t w, decorator<T> t) noexcept
{
	t.prefix = w;
	return t;
}

} // namespace decorator_operators

} // namespace utilities
} // namespace lal
