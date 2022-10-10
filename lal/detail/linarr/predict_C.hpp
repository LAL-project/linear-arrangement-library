/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2022
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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/detail/identity_arrangement.hpp>
#include <lal/detail/macros/basic_convert.hpp>

namespace lal {
namespace detail {

/**
 * @brief Amount of crossings pairs of edges of given lengths.
 *
 * Complexity: constant time.
 * @param n Number of edges in the linear arrangement.
 * @param d1 Length of the first edge.
 * @param d2 Length of the second edge.
 * @returns The amount of pairs of edges of lengths @e d1 and @e d2
 * respectively that can cross in a linear arrangement.
 */
inline constexpr
uint64_t alpha(const int64_t n, const int64_t d1, const int64_t d2) noexcept {
	int64_t f = 0;
	// positions s1 < s2
	if (1 <= n - (d1 + d2)) {
		// sum(d1 - 1, i, 1, n - d2 - d1)
		f += (d1 - 1)*(n - d2 - d1);
		// sum(n - d2 - i, i, n - (d1 + d2) + 1, n - d2 - 1)
		f += (d1*(d1 - 1))/2;
	}
	else {
		// sum(n - i - d2, i, 1, n - d2 - 1)
		f += ((d2 - n)*(d2 - n + 1))/2;
	}

	// positions s2 < s1
	if (d1 + d2 <= n) {
		f += (d1 - 1)*(n - d2 - d1);
	}
	if (1 + d2 - d1 >= 1) {
		if (1 + d2 <= n - d1) {
			f += (d1*(d1 - 1))/2;
		}
		else {
			f += ((n - d2)*(n - d2 - 1))/2;
		}
	}

#if defined DEBUG
	assert(f >= 0);
#endif
	return detail::to_uint64(f);
}

/**
 * @brief Amount of pairs of edges of given lengths.
 *
 * Complexity: constant time.
 * @param n Number of edges in the linear arrangement.
 * @param d1 Length of the first edge.
 * @param d2 Length of the second edge.
 * @returns The amount of pairs of edges of lengths @e d1 and @e d2
 * respectively that can cross in a linear arrangement.
 */
inline constexpr
uint64_t beta(const int64_t n, const int64_t d1, const int64_t d2) noexcept {
	int64_t f = 0;

	// positions s1 < s2
	if (1 <= n - (d1 + d2)) {
		// sum(n - i - d2 - 1, i, 1, n - d1 - d2)
		f += (n - d2)*(n - d2) + 3*(d1 + d2 - n) - d1*d1;
		// sum(n - i - d2, i, n - (d1 + d2) + 1, n - d2 - 1)
		f += d1*(d1 - 1);
	}
	else {
		// sum(n - i - d2, i, 1, n - d2 - 1)
		f += (d2 - n)*(d2 - n + 1);
	}

	// positions s2 < s1
	if (d1 < d2) {
		if (1 + d2 <= n - d1) {
			// sum(i - 3, i, 1 + d2, n - d1)
			f += (n - d1)*(n - d1) - 5*(n - d1 - d2) - d2*d2;
		}

		if (d2 <= n - d1) {
			// sum(i - 2, i, 1 + d2 - d1, d2)
			f += d1*(2*d2 - d1 - 3);
		}
		else {
			// sum(i - 2, i, 1 + d2 - d1, n - d1)
			f += (d2 - n)*(2*d1 - d2 - n + 3);
		}
	}
	else {
		// these sums are the same as in the positive
		// case above, but simplified assuming d1 = d2.

		if (1 + 2*d1 <= n) {
			f += n*(n - 3) + d1*(6 - 2*n);
		}

		if (2*d1 <= n) {
			f += d1*(d1 - 1);
		}
		else {
			f += (d1 - n)*(d1 - n + 1);
		}
	}

#if defined DEBUG
	assert(f >= 0);
	assert(f%2 == 0);
#endif
	return detail::to_uint64(f/2);
}

/**
 * @brief Predicted number of crossings based on the sum of edge lengths.
 *
 * See @cite Ferrer2014a for further details. This functions implements
 * \f$E_2[C]\f$.
 * @tparam result_t Type of the return value.
 * @tparam graph_t Type of graph.
 * @tparam arrangement_t Type of arrangement.
 * @param g Input graph.
 * @param arr Input arrangement.
 * @returns The value of \f$E_2[C]\f$.
 */
template <typename result_t, class graph_t, class arrangement_t>
result_t predict_C_using_edge_lengths(
	const graph_t& g,
	const arrangement_t& arr
)
noexcept
{
	result_t Ec2(0);
	const uint64_t n = g.get_num_nodes();
	const int64_t nn = detail::to_int64(n);

	iterators::Q_iterator<graph_t> q(g);
	while (not q.end()) {
		const auto [st, uv] = q.get_edge_pair_t();
		q.next();

		const auto [s,t] = st;
		const auto [u,v] = uv;

		const int64_t len_st = detail::to_int64(detail::abs_diff(arr[s], arr[t]));
		const int64_t len_uv = detail::to_int64(detail::abs_diff(arr[u], arr[v]));

		const auto [al, be] =
		(len_st <= len_uv ?
			std::make_pair(alpha(nn, len_st, len_uv), beta(nn, len_st, len_uv))
			:
			std::make_pair(alpha(nn, len_uv, len_st), beta(nn, len_uv, len_st))
		);

		if constexpr (std::is_same_v<result_t, numeric::rational>) {
			Ec2 += numeric::rational(al, be);
		}
		else {
			Ec2 += detail::to_double(al)/detail::to_double(be);
		}
	}

	return Ec2;
}

} // -- namespace detail
} // -- namespace lal
