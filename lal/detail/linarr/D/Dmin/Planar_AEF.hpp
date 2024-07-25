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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/detail/linarr/D/Dmin/utils.hpp>
#include <lal/detail/linarr/D/Dopt_utils.hpp>
#include <lal/detail/pairs_utils.hpp>

namespace lal {
namespace detail {
namespace Dmin {
namespace planar {

/**
 * @brief Minimum planar arrangement of a free tree.
 *
 * This function first constructs the sorted adjacency matrix rooted
 * at one of the tree's centroidal vertices. Then, it arranges the tree so that
 * there are no edge crossings and the centroidal vertex is not covered. Such
 * arrangement is done using an interval-based algorithm.
 *
 * This function implements the algorithm in \cite Alemany2022a.
 * @tparam make_arrangement Construct a maximum arrangement.
 * @param t Input tree.
 * @returns A pair of cost and minimum linear arrangement.
 */
template <bool make_arrangement>
[[nodiscard]] std::conditional_t<
	make_arrangement,
	std::pair<uint64_t, linear_arrangement>,
	uint64_t
>
AEF(const graphs::free_tree& t) noexcept
{
#if defined DEBUG
	assert(t.is_tree());
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

	// Make the sorted adjacency list rooted at the centroid of the tree.
	// This adjacency list is sorted non-increasingly by size of the subtrees.
	// LARGEST to SMALLEST

	std::vector<std::vector<node_size>> L;
	const node c = centroidal_vertex_plus_adjacency_list(t, 0, L).first;

	// construct the optimal planar arrangement by calculating the optimal
	// projective arrangement

	linear_arrangement arr(n);
	const uint64_t D = Dmin_utils::arrange_projective(n, L, c, arr);

	return {D, std::move(arr)};
}

} // -- namespace planar
} // -- namespace Dmin
} // -- namespace detail
} // -- namespace lal
