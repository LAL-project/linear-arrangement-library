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

// lal includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/graphs/traversal.hpp>
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace detail {

/**
 * @brief Fast tree non-isomorphism test.
 *
 * @tparam tree_t Tree type.
 * @param t1 One tree.
 * @param t2 Another tree.
 * @returns Whether the input trees are, might be, or are not isomorphic.
 * @returns 0 if the trees ARE isomorphic
 * @returns 1 if the trees ARE NOT isomorphic:
 * - number of vertices do not coincide,
 * - number of leaves do not coincide,
 * - second moment of degree do not coincide,
 * - maximum vertex degrees do not coincide,
 * @returns 2 if the trees MIGHT BE isomorphic
 */
template <graphs::Tree tree_t>
[[nodiscard]] char fast_non_iso(const tree_t& t1, const tree_t& t2) noexcept
{
	// check number of nodes
	if (t1.get_num_nodes() != t2.get_num_nodes()) {
		return 1;
	}

	const uint64_t n = t1.get_num_nodes();

	// trees ARE isomorphic
	if (n <= 2) {
		return 0;
	}

	uint64_t nL_t1 = 0;		// number of leaves of t1
	uint64_t nL_t2 = 0;		// number of leaves of t2
	uint64_t k2_t1 = 0;		// sum of squared degrees of t1
	uint64_t k2_t2 = 0;		// sum of squared degrees of t2
	uint64_t maxdeg_t1 = 0; // max degree of t1
	uint64_t maxdeg_t2 = 0; // max degree of t2
	for (node u = 0; u < n; ++u) {
		const uint64_t ku1 = to_uint64(t1.get_degree(u));
		const uint64_t ku2 = to_uint64(t2.get_degree(u));

		nL_t1 += t1.get_degree(u) == 1;
		nL_t2 += t2.get_degree(u) == 1;
		k2_t1 += ku1 * ku1;
		k2_t2 += ku2 * ku2;
		maxdeg_t1 = (maxdeg_t1 < ku1 ? ku1 : maxdeg_t1);
		maxdeg_t2 = (maxdeg_t2 < ku2 ? ku2 : maxdeg_t2);
	}

	// check number of leaves
	if (nL_t1 != nL_t2) {
		return 1;
	}
	// check maximum degree
	if (maxdeg_t1 != maxdeg_t2) {
		return 1;
	}
	// check sum of squared degrees
	if (k2_t1 != k2_t2) {
		return 1;
	}

	// trees MIGHT BE isomorphic
	return 2;
}

} // namespace detail
} // namespace lal
