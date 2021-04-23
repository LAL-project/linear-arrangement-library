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
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// lal includes
#include <lal/properties/Q.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/properties/D_rla.hpp>

namespace lal {
using namespace graphs;
using namespace numeric;

namespace properties {

/* ----------------------- */
/* VARIANCE OF D: V_rla[D] */

rational var_sum_edge_lengths_rational(const undirected_graph& g) noexcept {
	// E_rla[D]
	const rational Ed = exp_sum_edge_lengths_rational(g);
	// E_rla[D^2]
	const integer n = g.get_num_nodes();
	const integer m = g.get_num_edges();

	// calculate expectations of types
	const rational E0((n + 1)*(n*5 + 4), 45);
	const rational E1((n + 1)*(n*7 + 4), 60);
	const rational E2(n*(n + 1), 6);

	// calculate frequencies
	const integer f0 = num_pairs_independent_edges_integer(g)*2;
	const integer f2(m);
	const integer f1(m*(m - 1) - f0);

	// calculate second moment
	rational Ed2 = 0;
	Ed2 += E0*f0;
	Ed2 += E1*f1;
	Ed2 += E2*f2;

	// return variance
	return Ed2 - Ed*Ed;
}

} // -- namespace properties
} // -- namespace lal
