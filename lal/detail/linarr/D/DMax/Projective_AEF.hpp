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
 *     Juan Luis Esteban (esteban@cs.upc.edu)
 *         LOGPROG: Logics and Programming Research Group
 *         Office 110, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://www.cs.upc.edu/~esteban/
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
#include <cassert>
#endif
#include <type_traits>
#include <vector>

// lal includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/linarr/D/DMax/utils.hpp>
#include <lal/detail/linarr/D/Dopt_utils.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace projective {

/**
 * @brief Maximum projective arrangement of a rooted tree.
 *
 * This algorithm frst constructs the sorted adjacency matrix rooted
 * at the tree's root. Then, it arranges the tree so that there are no edge
 * crossings and the root vertex is not covered. Such arrangement is done using
 * a interval-based algorithm.
 *
 * This function implements the algorithm in \cite Alemany2024a.
 * @tparam make_arrangement Construct a maximum arrangement.
 * @param t Input tree.
 * @returns A pair of cost and maximum linear arrangement.
 */
template <bool make_arrangement>
[[nodiscard]] std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t>
AEF(const graphs::rooted_tree& t) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Alemany2024a);
#endif

#if defined DEBUG
	assert(t.is_rooted_tree());
#endif

	const uint64_t n = t.get_num_nodes();
	if (n == 1) {
		if constexpr (make_arrangement) {
			return {0, linear_arrangement::identity(1)};
		}
		else {
			return 0;
		}
	}

	std::vector<std::vector<node_size>> L(n);
	Dopt_utils::make_sorted_adjacency_list_rooted<
		sorting::sort_type::non_increasing>(t, L);

	// construct the optimal projective arrangement

	linear_arrangement arr(make_arrangement ? n : 0);
	const uint64_t D = DMax_utils::arrange_projective<make_arrangement>(
		n, L, t.get_root(), arr
	);

	if constexpr (make_arrangement) {
		return {D, std::move(arr)};
	}
	else {
		return D;
	}
}

} // namespace projective
} // namespace DMax
} // namespace detail
} // namespace lal
