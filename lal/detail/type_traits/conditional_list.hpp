/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
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
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// lal includes
#include <lal/detail/type_traits/ith_type.hpp>
#include <lal/detail/type_traits/bool_sequence.hpp>
#include <lal/detail/type_traits/type_sequence.hpp>

namespace lal {
namespace detail {

/// Partial template specialization of @ref ith_type for @ref type_sequence.
template <std::size_t ith_idx, typename... Ts>
struct ith_type<ith_idx, type_sequence<Ts...>> : ith_type<ith_idx, Ts...>
{ };

/**
 * @brief Generalization of std::conditional_list.
 *
 * Choose a type depending on the first true value in @e bool_seq.
 * @tparam bool_seq A sequence of Boolean values.
 * @tparam type_seq A sequence of types.
 * @pre Exactly one of the Boolean values in @e bool_seq has to be true.
 */
template <typename bool_seq, typename type_seq>
struct conditional_list {
	/// The type from @e type_seq that is to be used according to @e bool_seq.
	using type =
		std::conditional_t<
			bool_seq::num_true == 0,
			std::nullptr_t,
			ith_type_t<bool_seq::index_true, type_seq>
		>;
};

/// Shorthand for @ref conditional_list.
template <typename bool_seq, typename type_seq>
using conditional_list_t = typename conditional_list<bool_seq, type_seq>::type;


static_assert(
	std::is_same_v<
		conditional_list_t<
			bool_sequence<false, false, true,   false>,
			type_sequence<int,   float, double, char>
		>,
		double
	>
);
static_assert(
	std::is_same_v<
		conditional_list_t<
			bool_sequence<false, false, false,  false>,
			type_sequence<int,   float, double, char>
		>,
		std::nullptr_t
	>
);
static_assert(
	std::is_same_v<
		conditional_list_t<
			bool_sequence<false, false, false,  true>,
			type_sequence<int,   float, double, char>
		>,
		char
	>
);

} // -- namespace detail
} // -- namespace lal
