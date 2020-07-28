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
 
#include <lal/properties/Q.hpp>

// C++ includes
#include <cmath>
using namespace std;

// lal includes
#include <lal/graphs/dgraph.hpp>
#include <lal/numeric/integer.hpp>

namespace lal {
using namespace graphs;
using namespace numeric;

namespace properties {

integer size_Q_integer(const ugraph& g) {
	// sum of squared degrees
	integer nk2(0);
	for (node u = 0; u < g.n_nodes(); ++u) {
		const uint32_t ku = g.degree(u);
		nk2 += ku*ku;
	}

	const uint32_t m = g.n_edges();
	integer q = integer_from_ui(m*(m + 1));
	q -= nk2;
	q /= 2;
	return q;
}

uint32_t size_Q(const ugraph& g) {
	return static_cast<uint32_t>(size_Q_integer(g).to_uint());
}

integer size_Q_integer(const dgraph& g) {
	// sum of squared degrees
	integer nk2(0);
	for (node u = 0; u < g.n_nodes(); ++u) {
		const uint64_t ku = g.in_degree(u) + g.out_degree(u);
		nk2 += ku*ku;
	}

	const uint64_t m = g.n_edges();
	integer q = integer_from_ui(m*(m + 1));
	q -= nk2;
	q /= 2;
	return q;
}

uint32_t size_Q(const dgraph& g) {
	return static_cast<uint32_t>(size_Q_integer(g).to_uint());
}

} // -- namespace properties
} // -- namespace lal
