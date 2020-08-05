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

#include <lal/linarr/headedness.hpp>

// C++ includes
using namespace std;

// lal includes
#include <lal/utils/macros.hpp>
#include <lal/iterators/E_iterator.hpp>

namespace lal {
using namespace graphs;
using namespace numeric;

namespace linarr {

inline uint32_t __headedness_rational(const directed_graph& g, const linearrgmnt& pi) {
	uint32_t edges_to_right = 0;
	iterators::E_iterator it(g);
	while (it.has_next()) {
		it.next();
		const edge e = it.get_edge();
		edges_to_right += pi[e.first] < pi[e.second];
	}
	return edges_to_right;
}

rational headedness_rational(const directed_graph& g, const linearrgmnt& pi) {
	const uint32_t etr = utils::call_with_empty_arrangement(__headedness_rational, g, pi);
	// avoid warning conversion
	return rational_from_ui(etr, g.n_edges());
}

double headedness(const directed_graph& g, const vector<node>& arr) {
	return headedness_rational(g, arr).to_double();
}

} // -- namespace linarr
} // -- namespace lal
