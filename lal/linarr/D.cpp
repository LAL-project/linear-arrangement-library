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
 
// C++ includes
#include <cassert>
using namespace std;

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/macros.hpp>

namespace lal {
using namespace graphs;
using namespace numeric;
using namespace iterators;

namespace linarr {

/* D */

inline uint32_t __sum_length_edges(const graph& g, const linear_arrangement& pi) {
	// sum of lengths
	uint32_t l = 0;

	E_iterator it(g);
	while (it.has_next()) {
		it.next();
		const auto [u,v] = it.get_edge();

		// accumulate length of edge
		l += (pi[u] < pi[v] ? pi[v] - pi[u] : pi[u] - pi[v]);
	}
	return l;
}

uint32_t sum_length_edges(const graph& g, const linear_arrangement& pi) {
	assert(pi.size() == 0 or g.n_nodes() == pi.size());
	return internal::call_with_empty_arrangement(__sum_length_edges, g, pi);
}

/* MDD */

inline rational __MDD_rational(const graph& g, const linear_arrangement& pi) {
	const uint32_t D = sum_length_edges(g, pi);
	return rational_from_ui(D, g.n_edges());
}

rational mean_dependency_distance_rational(const graph& g, const linear_arrangement& pi) {
	assert(pi.size() == 0 or g.n_nodes() == pi.size());
	return internal::call_with_empty_arrangement(__MDD_rational, g, pi);
}

double mean_dependency_distance(const graph& g, const linear_arrangement& pi) {
	return mean_dependency_distance_rational(g, pi).to_double();
}

} // -- namespace linarr
} // -- namespace lal
