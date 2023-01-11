/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2023
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <numeric>

// lal includes
#include <lal/linear_arrangement.hpp>

namespace lal {
namespace detail {

/**
 * @brief Type of arrangement.
 *
 * Used to call functions that have arrangements as input parameters.
 */
enum class linarr_type {
	/// Identity arrangement. \f$\pi(i)=i\f$.
	identity,
	/// Non-identity arrangement. An arrangement that is not the identity.
	nonident
};

/**
 * @brief A wrapper to easily use identity arrangements.
 *
 * This reduces execution time: it prevents the explicit construction of
 * the identity arrangement (which involves allocation of memory, ...)
 */
template <linarr_type type>
struct linarr_wrapper {
	/// Constructor with arrangement.
	linarr_wrapper(const lal::linear_arrangement& arr) noexcept
		: m_arr(arr)
	{
#if defined DEBUG
		if constexpr (type == linarr_type::identity) {
			assert(arr.size() == 0);
		}
		else {
			assert(arr.size() != 0);
		}
#endif
	}

	/**
	 * @brief Access operator.
	 *
	 * Only when the arrangement is not the identity arrangement, the
	 * method accessess @ref m_arr.
	 * @param p Either a @ref lal::node_t or a @ref lal::position_t.
	 */
	template <typename param_t>
	uint64_t operator[] (const param_t& p) const noexcept {
		static_assert(
			std::is_same_v<param_t,lal::node_t> or
			std::is_same_v<param_t,lal::position_t>
		);

		if constexpr (type == linarr_type::identity) {
			return *p;
		}
		else {
			return m_arr[p];
		}
	}

	/// Constant reference to actual arrangement.
	const lal::linear_arrangement& m_arr;
};

/// Shorthand for an identity arrangement.
inline linarr_wrapper<linarr_type::identity> identity_arr(const linear_arrangement& arr)
noexcept
{
	return linarr_wrapper<linarr_type::identity>(arr);
}

/// Shorthand for a nonidentity arrangement.
inline linarr_wrapper<linarr_type::nonident> nonident_arr(const linear_arrangement& arr)
noexcept
{
	return linarr_wrapper<linarr_type::nonident>(arr);
}

} // -- namespace detail
} // -- namespace lal
