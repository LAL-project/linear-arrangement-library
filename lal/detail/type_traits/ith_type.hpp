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
#include <type_traits>

// lal includes
#include <lal/detail/type_traits/first_true.hpp>

namespace lal {
namespace detail {

/**
 * @brief Selection of the ith type of a list of types.
 *
 * @tparam ith_idx Index pointing at the type we want.
 * @tparam Ts List of types to choose from.
 * @post If parameter @e ith_idx is equal to or larger than the number of
 * parameters in @e Ts, the resulting type is std::nullptr_t.
 */
template <std::size_t ith_idx, typename... Ts>
struct ith_type {
	/// Implementation of @ref ith_type.
	template <std::size_t cur_idx, typename t1, typename... ts>
	struct _ith_type_impl {
		/// Type at position @e ith_idx of @e Ts.
		using type =
			std::conditional_t<
				ith_idx == cur_idx,
				t1,
				typename _ith_type_impl<cur_idx + 1, ts...>::type
			>;
	};

	/// Implementation of @ref ith_type. Partial template specialization.
	template <std::size_t cur_idx, typename t1>
	struct _ith_type_impl<cur_idx, t1> {
		/// Type at position @e ith_idx of @e Ts.
		using type = std::conditional_t<ith_idx == cur_idx, t1, std::nullptr_t>;
	};

	/// The type at the @e i-th position in @e Ts.
	using type = typename _ith_type_impl<0, Ts...>::type;
};

/// Shorthand for @ref ith_type::type.
template <std::size_t ith_idx, typename... Ts>
using ith_type_t = typename ith_type<ith_idx, Ts...>::type;

static_assert(std::is_same_v<ith_type_t<0, int, float, double>, int>);
static_assert(std::is_same_v<ith_type_t<1, int, float, double>, float>);
static_assert(std::is_same_v<ith_type_t<2, int, float, double>, double>);
static_assert(std::is_same_v<ith_type_t<3, int, float, double>, std::nullptr_t>);
static_assert(std::is_same_v<ith_type_t<4, int, float, double>, std::nullptr_t>);
static_assert(std::is_same_v<ith_type_t<5, int, float, double>, std::nullptr_t>);

} // -- namespace detail
} // -- namespace lal

