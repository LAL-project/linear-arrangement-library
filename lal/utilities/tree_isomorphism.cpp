/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#include <lal/utilities/tree_isomorphism.hpp>

// C++ includes
#include <vector>
using namespace std;

// lal includes
#include <lal/internal/graphs/trees/tree_centre.hpp>

#define to_uint64(x) (static_cast<uint64_t>(x))

namespace lal {
using namespace graphs;

namespace utilities {

/* Returns whether the input trees are NOT isomorphic. Returns TRUE if:
 * - number of vertices do not coincide
 * - number of leaves do not coincide
 * - second moment of degree do not coincide
 */
template<class T>
bool fast_non_iso(const T& t1, const T& t2) {
	// check number of nodes
	if (t1.n_nodes() != t2.n_nodes()) { return true; }

	const uint32_t n = t1.n_nodes();

	uint32_t nL_t1 = 0; // number of leaves of t1
	uint32_t nL_t2 = 0; // number of leaves of t2
	uint64_t k2_t1 = 0; // sum of squared degrees of t1
	uint64_t k2_t2 = 0; // sum of squared degrees of t2
	uint64_t maxdeg_t1 = 0; // max degree of t1
	uint64_t maxdeg_t2 = 0; // max degree of t2
	for (node u = 0; u < n; ++u) {
		const uint64_t ku1 = to_uint64(t1.degree(u));
		const uint64_t ku2 = to_uint64(t2.degree(u));

		nL_t1 += t1.degree(u) == 1;
		nL_t2 += t2.degree(u) == 1;
		k2_t1 += ku1*ku1;
		k2_t2 += ku2*ku2;
		maxdeg_t1 = (maxdeg_t1 < ku1 ? ku1 : maxdeg_t1);
		maxdeg_t2 = (maxdeg_t2 < ku2 ? ku2 : maxdeg_t2);
	}

	// check number of leaves
	if (nL_t1 != nL_t2) { return true; }
	// check maximum degree
	if (maxdeg_t1 != maxdeg_t2) { return true; }
	// check sum of squared degrees
	if (k2_t1 != k2_t2) { return true; }

	return false;
}

bool are_trees_isomorphic(const rooted_tree& t1, const rooted_tree& t2) {
	if (fast_non_iso(t1,t2)) { return false; }
	return false;
}

bool are_trees_isomorphic(const free_tree& t1, const free_tree& t2) {
	if (fast_non_iso(t1,t2)) { return false; }
	const uint32_t n = t1.n_nodes();

	// find centres of the trees
	const auto c1 = internal::retrieve_centre(t1, 0);
	const auto c2 = internal::retrieve_centre(t2, 0);

	// check centre sizes
	const uint32_t size1 = (c1.second == n ? 2 : 1);
	const uint32_t size2 = (c2.second == n ? 2 : 1);
	if (size1 != size2) { return false; }

	const rooted_tree rt1 = rooted_tree(t1, c1.first);

	// the centres have only one vertex
	if (size1 == 1) {
		const rooted_tree rt2 = rooted_tree(t2, c2.first);
		return are_trees_isomorphic(rt1, rt2);
	}

	// the centres have two vertices
	const rooted_tree rt2 = rooted_tree(t2, c2.second);
	return are_trees_isomorphic(rt1, rt2);
}

} // -- namespace utilities
} // -- namespace lal
