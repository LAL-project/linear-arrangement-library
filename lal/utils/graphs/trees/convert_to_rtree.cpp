/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Linear Arrangement Library.
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

#include <lal/utils/graphs/trees/convert_to_rtree.hpp>

// C++ includes
#include <cassert>
using namespace std;

// lal includes
#include <lal/utils/macros.hpp>

namespace lal {
using namespace graphs;

namespace utils {

rtree linear_sequence_to_tree(const vector<uint32_t>& L, uint32_t n) {
	assert(L.size() == n + 1);

	// edges of the tree
	vector<edge> edges(n - 1);
	auto eit = edges.begin();

#if defined DEBUG
	// variable to make sure that the root has been set
	bool root_set = false;
#endif
	// root node of the tree (initiliased
	// so compiler does not cry)
	node r = 0;

	for (uint32_t i = 1; i <= n; ++i) {
		if (L[i] == 0) {
			// root, do nothing
			r = i - 1;
#if defined DEBUG
			root_set = true;
#endif
		}
		else {
			// add the edge...
			*eit++ = edge(i - 1, L[i] - 1);
		}
	}

	// root must have been set.
#if defined DEBUG
	assert(root_set);
#endif

	ftree t(n);
	t.add_edges(edges);
	return rtree(t, r);
}

} // -- namespace convert
} // -- namespace lal
