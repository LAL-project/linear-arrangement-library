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
#include <lal/graphs/directed_graph.hpp>
#include <lal/numeric/integer.hpp>

namespace lal {
using namespace graphs;
using namespace numeric;

namespace properties {

integer size_Q_integer(const undirected_graph& g) {
	const uint64_t m = g.n_edges();
	integer q2 = integer_from_ui(m*(m + 1));

	// substract sum of squared degrees
	for (node u = 0; u < g.n_nodes(); ++u) {
		const uint64_t ku = g.degree(u);
		q2 -= ku*ku;
	}

	return q2/2;
}

uint64_t size_Q(const undirected_graph& g) {
	return size_Q_integer(g).to_uint();
}

integer size_Q_integer(const directed_graph& g) {
	const uint32_t m = g.n_edges();
	integer q2 = integer_from_ui(m*(m - 1));

	for (node u = 0; u < g.n_nodes(); ++u) {
		const uint64_t ku_in = g.in_degree(u);
		const uint64_t ku_out = g.out_degree(u);
		q2 -= (ku_in + ku_out)*(ku_in + ku_out - 1);
	}

	return q2/2;
}

uint64_t size_Q(const directed_graph& g) {
	return size_Q_integer(g).to_uint();
}

} // -- namespace properties
} // -- namespace lal
