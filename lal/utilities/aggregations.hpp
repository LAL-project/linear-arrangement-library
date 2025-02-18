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
#if defined DEBUG
#include <iterator>
#include <cassert>
#endif
#include <cstddef>

namespace lal {
namespace utilities {

/**
 * @brief Computation of 1-level aggregation of \f$Q\f$ and \f$R\f$.
 *
 * Given
 * - a list of \f$n\f$ values \f$Q_i\f$ and \f$n\f$ values \f$R_i\f$ (produced
 * by the function @e values),
 * - an 'accumulator' operator \f$\oplus\f$ for \f$Q_i\f$ (parameter @e accQ),
 * - an 'accumulator' operator \f$\otimes\f$ for \f$R_i\f$ (parameter @e accR),
 * - a \f$Q\f$-average function \f$F_Q\f$ (parameter @e avgQ),
 * - a \f$R\f$-average function \f$F_R\f$ (parameter @e avgR),
 * - and a combination operator \f$\odot\f$ (parameter @e avg),
 *
 * this function computes the following average
 *
 * \f$A_1(Q,R) = \left( F_Q\left( \bigoplus_{i=1}^n Q_i \right) \right) \odot
 *          \left( F_R\left( \bigotimes_{i=1}^n R_i \right) \right)\f$
 *
 * The values \f$Q_i\f$ and \f$R_i\f$ are obtained from applying function @e values
 * passed as parameter. This function iterates over two sets of elements, where
 * the second is optional and may be empty; this must be known at compile time
 * through the template parameter @e second_set_empty. These sets are passed with
 * iterators (parameters @e bfirst, @e efirst, @e bsecond, @e esecond).
 *
 * If the second set is empty then function @e values only has one parameter of the
 * same type as the values in the first set. If the second set is not empty then
 * @e values has two parameters: the first parameter is of the same type as the elements
 * in the first set, and the second parameter is of the same type as the elements
 * in the second set. In the latter case, @e values is called with the @e i-th element
 * in the first set and the @e i-th element in the second set.
 *
 * The function that averages the values \f$Q_i\f$, function @e avgQ, (resp.
 * \f$R_i\f$, function @e avgR) has two parameters: the accumulation of the values
 * \f$Q_i\f$ (resp. \f$R_i\f$) and the total amount of values. Said accumulation
 * is done via functions @e accQ and @e accR respectively.
 *
 * See the implementation of function
 * @ref lal::linarr::mean_dependency_distance_1level_rational and its documentation
 * for an example of usage of this function.
 *
 * @tparam result_t The type of the result of this function.
 * @tparam second_set_empty Is the second set of elements empty?
 * @tparam iterator_first_t Iterator type.
 * @tparam iterator_second_t Iterator type.
 * @tparam metric Function type.
 * @tparam accumulate_Q Function type.
 * @tparam accumulate_R Function type.
 * @tparam make_average_Q Function type.
 * @tparam make_average_R Function type.
 * @tparam make_average Function type.
 *
 * @param bfirst Iterator at the beginning of the first set.
 * @param efirst Iterator at the end of the first set.
 * @param bsecond Iterator at the beginning of the second set.
 * @param esecond Iterator at the end of the second set.
 * @param values Function to calculate the values \f$Q_i,R_i\f$ called with the
 * \f$i\f$-th element in the first set only (if @e second_set_empty is true) or
 * called with the \f$i\f$-th elements in the first and second sets (if
 * @e second_set_empty is false). Implements \f$\odot\f$.
 * @param accQ Operator \f$\oplus\f$. A function that accumulates the values
 * \f$Q_i\f$. It has two parameters: the total accumulated value (passed by
 * reference), and the value to be accumulated.
 * @param accR Operator \f$\otimes\f$. A function that accumulates the values
 * \f$R_i\f$. It has two parameters: the total accumulated value (passed by
 * reference), and the value to be accumulated.
 * @param avgQ Function \f$F_Q\f$ that averages the values \f$Q_i\f$. It has two
 * parameters: the accumulation of the values \f$Q_i\f$ and the total amount of
 * values.
 * @param avgR Function \f$F_R\f$ that averages the values \f$R_i\f$. It has two
 * parameters: the accumulation of the values \f$R_i\f$ and the total amount of
 * values.
 * @param avg Operator \f$\odot\f$. A function that is passed the result of
 * \f$F_Q\f$ and \f$F_R\f$.
 *
 * @pre If the second set of elements is empty then @e second_set_empty must be
 * true. It must be false otherwise.
 * @returns \f$A_1(Q,R)\f$.
 */
template <
	typename result_t,
	bool second_set_empty,
	typename iterator_first_t,
	typename iterator_second_t,
	class metric,
	class accumulate_Q,
	class accumulate_R,
	class make_average_Q,
	class make_average_R,
	class make_average>
[[nodiscard]] result_t one_level_aggregation(
	iterator_first_t bfirst,
	const iterator_first_t efirst,
	iterator_second_t bsecond,
	[[maybe_unused]] const iterator_second_t esecond,
	metric values,
	accumulate_Q accQ,
	accumulate_R accR,
	make_average_Q avgQ,
	make_average_R avgR,
	make_average avg
) noexcept
{
	if constexpr (second_set_empty) {
		// no elements from the second set
		auto [total_Q, total_R] = values(*bfirst);
		std::size_t amount = 1;

		++bfirst;
		for (; bfirst != efirst; ++bfirst, ++amount) {
			const auto [Qi, Ri] = values(*bfirst);
			accQ(total_Q, Qi);
			accR(total_R, Ri);
		}

		return avg(avgQ(total_Q, amount), avgR(total_R, amount));
	}
	else {
#if defined DEBUG
		// ensure same number of elements in both sets
		assert(
			std::distance(bfirst, efirst) == std::distance(bsecond, esecond)
		);
#endif

		auto [total_Q, total_R] = values(*bfirst, *bsecond);
		std::size_t amount = 1;

		++bfirst;
		++bsecond;
		for (; bfirst != efirst; ++bfirst, ++bsecond, ++amount) {
			const auto [Qi, Ri] = values(*bfirst, *bsecond);
			accQ(total_Q, Qi);
			accR(total_R, Ri);
		}

		return avg(avgQ(total_Q, amount), avgR(total_R, amount));
	}
}

/**
 * @brief Computation of 2-level aggregation of \f$Q\f$ and \f$R\f$.
 *
 * Given
 * - a list of \f$n\f$ values \f$Q_i\f$  and \f$n\f$ values \f$R_i\f$ (produced
 * by the function @e values),
 * - and a combination operator \f$\oplus\f$ to operator two values \f$Q_i\f$
 * and \f$R_i\f$ (parameter @e comb_values),
 * - an 'accumulator' operator \f$\otimes\f$ for \f$Q_i\oplus R_i\f$ (parameter
 * @e acc_values),
 * - a function \f$F\f$ to average the result of \f$\otimes\f$ (parameter
 * @e avg),
 *
 * this function computes the following average
 *
 * \f$A_2(Q,R) = F\left( \bigotimes_{i=1}^n (Q_i\oplus R_i) \right)\f$
 *
 * The values \f$Q_i\f$ and \f$R_i\f$ are obtained from applying function @e values
 * passed as parameter. This function iterates over two sets of elements, where
 * the second is optional and may be empty; this must be known at compile time
 * through the template parameter @e second_set_empty. These sets are passed with
 * iterators (parameters @e bfirst, @e efirst, @e bsecond, @e esecond).
 *
 * If the second set is empty then function @e values only has one parameter of
 * the same type as the values in the first set. If the second set is not empty
 * then @e values has two parameters: the first parameter is of the same type as
 * the elements in the first set, and the second parameter is of the same type
 * as the elements in the second set. In the latter case, @e values is called
 * with the @e i-th element in the first set and the @e i-th element in the second
 * set.
 *
 * See the implementation of function
 * @ref lal::linarr::mean_dependency_distance_2level_rational and its documentation
 * for an example of usage of this function.
 *
 * Note: this function can be extended to more values than just \f$Q\f$ and \f$R\f$
 * by implementing the functions accordingly: @e values can return a structure with
 * several values, function @e comb_values should accept said structure and combine
 * the values accordingly, ...
 *
 * @tparam result_t The type of the result of this function.
 * @tparam second_set_empty Is the second set of elements empty?
 * @tparam iterator_first_t Iterator type.
 * @tparam iterator_second_t Iterator type.
 * @tparam metric Function type.
 * @tparam combine Function type.
 * @tparam accumulate Function type.
 * @tparam make_average Function type.
 *
 * @param bfirst Iterator at the beginning of the first set.
 * @param efirst Iterator at the end of the first set.
 * @param bsecond Iterator at the beginning of the second set.
 * @param esecond Iterator at the end of the second set.
 * @param values Function to calculate the values \f$Q_i,R_i\f$ called with the
 * \f$i\f$-th element in the first set only (if @e second_set_empty is true) or
 * called with the \f$i\f$-th elements in the first and second sets (if
 * @e second_set_empty is false).
 * @param comb_values Operator \f$\oplus\f$. Function that combines two values \f$Q_i\f$
 * and \f$R_i\f$.
 * @param acc_values Operator \f$\otimes\f$. Function that accumulates the combination
 * of two values \f$Q_i\f$ and \f$R_i\f$.
 * @param avg Function \f$F\f$ that averages the result of \f$\otimes\f$. It is
 * passed the result of \f$\otimes\f$ and the total amount of elements.
 *
 * @pre If the second set of elements is empty then @e second_set_empty must be
 * true. It must be false otherwise.
 * @returns \f$A_2(Q,R)\f$.
 */
template <
	typename result_t,
	bool second_set_empty,
	typename iterator_first_t,
	typename iterator_second_t,
	class metric,
	class combine,
	class accumulate,
	class make_average>
[[nodiscard]] result_t two_level_aggregation(
	iterator_first_t bfirst,
	const iterator_first_t efirst,
	iterator_second_t bsecond,
	[[maybe_unused]] const iterator_second_t esecond,
	metric values,
	combine comb_values,
	accumulate acc_values,
	make_average avg
) noexcept
{
	if constexpr (second_set_empty) {
		// no elements from the second set
		auto total = comb_values(values(*bfirst));
		std::size_t amount = 1;

		++bfirst;
		for (; bfirst != efirst; ++bfirst, ++amount) {
			acc_values(total, comb_values(values(*bfirst)));
		}
		return avg(total, amount);
	}
	else {
#if defined DEBUG
		// ensure same of elements in both sets
		assert(
			std::distance(bfirst, efirst) == std::distance(bsecond, esecond)
		);
#endif
		auto total = comb_values(values(*bfirst, *bsecond));
		std::size_t amount = 1;

		++bfirst;
		++bsecond;
		for (; bfirst != efirst; ++bfirst, ++bsecond, ++amount) {
			acc_values(total, comb_values(values(*bfirst, *bsecond)));
		}
		return avg(total, amount);
	}
}

} // namespace utilities
} // namespace lal
