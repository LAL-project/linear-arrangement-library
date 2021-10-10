/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. To see the full code
 *  visit the webpage:
 *      https://github.com/lluisalemanypuig/linear-arrangement-library.git
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
 *      Juan Luis Esteban (esteban@cs.upc.edu)
 *          Office 110, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~esteban/
 *          Research Gate: https://www.researchgate.net/profile/Juan_Esteban13
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/internal/linarr/Dmin_utils.hpp>

namespace lal {
namespace internal {

/* Minimum projective arrangement of a free tree following the description in
 * \cite Alemany2021a.
 *
 * This algorithm frst constructs the sorted adjacency matrix rooted
 * at the tree's root. Then, it arranges the tree so that there are no edge
 * crossings and the root vertex is not covered. Such arrangement is done using
 * a interval-based algorithm.
 */
inline
std::pair<uint64_t, linear_arrangement> Dmin_Projective_AEF(const graphs::rooted_tree& t)
noexcept
{
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif

	const uint64_t n = t.get_num_nodes();
	if (n == 1) {
		return std::make_pair(0, linear_arrangement(0,0));
	}

	// M[u] : adjacency list of vertex u sorted decreasingly according
	// to the sizes of the subtrees.
	// This is used to find the optimal projective arrangement of the tree.
	std::vector<std::vector<node_size>> L(n);
	rooted::make_sorted_rooted_adjacency_list(t, L);

	// construct the optimal planar arrangement
	linear_arrangement arr(n);
	const uint64_t D = intervals::arrange(n, L, t.get_root(), arr);

	return make_pair(D, std::move(arr));
}

} // -- namespace internal
} // -- namespace lal
