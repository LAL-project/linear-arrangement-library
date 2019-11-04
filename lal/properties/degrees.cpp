/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
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
 
#include <lal/properties/degrees.hpp>

// C++ includes
using namespace std;

// lal includes
#include <lal/numeric/integer.hpp>

namespace lal {
using namespace numeric;

namespace properties {

rational second_mmt_degree_rational(const ugraph& g) {
	integer S(0);
	integer du(0);
	for (node u = 0; u < g.n_nodes(); ++u) {
		du.set_ui(g.degree(u));
		du ^= 2;
		S += du;
	}
	rational k2;
	k2 = S;
	k2 /= static_cast<int64_t>(g.n_nodes());
	return k2;
}

double second_mmt_degree(const ugraph& g) {
	return second_mmt_degree_rational(g).to_double();
}

rational third_mmt_degree_rational(const ugraph& g) {
	integer S(0);
	integer du(0);
	for (node u = 0; u < g.n_nodes(); ++u) {
		du.set_ui(g.degree(u));
		du ^= 3;
		S += du;
	}
	rational k3;
	k3 = S;
	k3 /= static_cast<int64_t>(g.n_nodes());
	return k3;
}

double third_mmt_degree(const ugraph& g) {
	return third_mmt_degree_rational(g).to_double();
}

} // -- namespace properties
} // -- namespace lal
