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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/
 
#include <lal/properties/D_rla.hpp>

// lal includes
#include <lal/properties/Q.hpp>
#include <lal/properties/C_rla.hpp>

namespace lal {
using namespace graphs;
using namespace numeric;

namespace properties {

/* -------------------------- */
/* EXPECTATION OF D: E_rla[D] */

rational expectation_D_rational(const ugraph& g) {
	rational Ed;
	Ed.init_ui((g.n_nodes() + 1)*g.n_edges());
	Ed /= 3;
	return Ed;
}

double expectation_D(const ugraph& g) {
	return expectation_D_rational(g).to_double();
}

/* ----------------------- */
/* VARIANCE OF D: V_rla[D] */

rational variance_D_rational(const ugraph& g) {
	rational Ed(0), Ed2(0);

	// E_rla[D]
	Ed = expectation_D_rational(g);
	// E_rla[D^2]
	integer n, m;
	n.init_ui(g.n_nodes());
	m.init_ui(g.n_edges());

	// calculate expectations of types
	rational E0((n + 1)*(n*5 + 4));
	E0 /= 45;
	rational E1((n + 1)*(n*7 + 4));
	E1 /= 60;
	rational E2(n*(n + 1));
	E2 /= 6;

	// calculate frequencies
	integer f0 = size_Q_integer(g)*2;
	integer f2(m);
	integer f1(m*(m - 1) - f0);

	// calculate second moment
	Ed2 = E0*f0;
	Ed2 += E1*f1;
	Ed2 += E2*f2;

	// return variance
	return Ed2 - Ed*Ed;
}

double variance_D(const ugraph& g) {
	return variance_D_rational(g).to_double();
}

} // -- namespace properties
} // -- namespace lal
