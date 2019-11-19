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

// C includes
#include <assert.h>

// C++ includes
#include <functional>
using namespace std;

namespace lal {
using namespace graphs;
using namespace numeric;

namespace properties {

// g: input graph
// p: moment of the *-degree
// D: function that returns the *-degree
template<class G>
inline rational __mmt_x_degree_rational
(
	const G& g, uint64_t p,
	const function<uint64_t(const G& _g, node _u)>& D
)
{
	const integer M = integer_from_ui(g.n_nodes());

	integer S(0);
	integer du(0);
	for (node u = 0; u < g.n_nodes(); ++u) {
		//du.set_ui(g.degree(u));
		du.set_ui(D(g, u));
		du ^= p;
		S += du;
	}
	return rational(S, M);
}

// moment of degree

#define _in_degree(g,u) dynamic_cast<const dgraph&>(g).degree(u)
rational mmt_degree_rational(const graph& g, uint64_t p) {
	return __mmt_x_degree_rational<graph>(
		g, p,
		[](const graph& _g, node _u) -> uint64_t
		{ return _g.degree(_u) + (_g.is_directed() ? _in_degree(_g,_u) : 0); }
	);
}

double mmt_degree(const graph& g, uint64_t p) {
	return mmt_degree_rational(g,p).to_double();
}

// moment of in-degree

rational mmt_in_degree_rational(const dgraph& g, uint64_t p) {
	return __mmt_x_degree_rational<dgraph>(
		g, p,
		[](const dgraph& _g, node _u) -> uint64_t
		{ return _g.in_degree(_u); }
	);
}

double mmt_in_degree(const dgraph& g, uint64_t p) {
	return mmt_in_degree_rational(g,p).to_double();
}

// moment of out-degree

rational mmt_out_degree_rational(const dgraph& g, uint64_t p) {
	return __mmt_x_degree_rational<dgraph>(
		g, p,
		[](const dgraph& _g, node _u) -> uint64_t
		{ return _g.degree(_u); }
	);
}

double mmt_out_degree(const dgraph& g, uint64_t p) {
	return mmt_out_degree_rational(g,p).to_double();
}

// hubiness

rational hubiness_rational(const utree& g) {
	const uint64_t n = g.n_nodes();

	// for n <= 3, <k^2>_star = <k^2>_linear
	// which means that hubiness is not defined:
	// division by 0.
	assert(n > 3);

	const rational k2_linear = rational_from_ui(4*n - 6, n);
	const rational k2_star = rational_from_ui(n - 1);
	const rational k2_graph = mmt_degree_rational(g, 2);
	return (k2_graph - k2_linear)/(k2_star - k2_linear);
}

double hubiness(const utree& g) {
	return hubiness_rational(g).to_double();
}

} // -- namespace properties
} // -- namespace lal
