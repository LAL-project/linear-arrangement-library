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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>

namespace lal {
using namespace graphs;
using namespace numeric;

namespace properties {

// g: input graph
// p: moment of the *-degree
// D: function that returns the *-degree
template<class G, class Callable>
inline rational __mmt_x_degree_rational
(const G& g, uint32_t p, const Callable& D)
{
	const integer M = integer_from_ui(g.num_nodes());

	integer S(0);
	integer du(0);
	for (node u = 0; u < g.num_nodes(); ++u) {
		//du.set_ui(g.degree(u));
		du.set_ui(D(g, u));
		du ^= p;
		S += du;
	}
	return rational(S, M);
}

// -----------------------------------------------------------------------------

// moment of degree

rational mmt_degree_rational(const undirected_graph& g, uint32_t p) {
	return __mmt_x_degree_rational<undirected_graph>(
		g, p,
		[](const undirected_graph& _g, node _u) -> uint32_t
		{ return _g.degree(_u); }
	);
}

double mmt_degree(const undirected_graph& g, uint32_t p) {
	return mmt_degree_rational(g,p).to_double();
}

rational mmt_degree_rational(const directed_graph& g, uint32_t p) {
	return __mmt_x_degree_rational<directed_graph>(
		g, p,
		[](const directed_graph& _g, node _u) -> uint32_t
		{ return _g.degree(_u); }
	);
}

double mmt_degree(const directed_graph& g, uint32_t p) {
	return mmt_degree_rational(g,p).to_double();
}

// moment of in-degree

rational mmt_in_degree_rational(const directed_graph& g, uint32_t p) {
	return __mmt_x_degree_rational<directed_graph>(
		g, p,
		[](const directed_graph& _g, node _u) -> uint32_t
		{ return _g.in_degree(_u); }
	);
}

double mmt_in_degree(const directed_graph& g, uint32_t p) {
	return mmt_in_degree_rational(g,p).to_double();
}

// moment of out-degree

rational mmt_out_degree_rational(const directed_graph& g, uint32_t p) {
	return __mmt_x_degree_rational<directed_graph>(
		g, p,
		[](const directed_graph& _g, node _u) -> uint32_t
		{ return _g.degree(_u); }
	);
}

double mmt_out_degree(const directed_graph& g, uint32_t p) {
	return mmt_out_degree_rational(g,p).to_double();
}

// hubiness

rational hubiness_rational(const free_tree& g) {
	const uint32_t n = g.num_nodes();

	// for n <= 3, <k^2>_star = <k^2>_linear
	// which means that hubiness is not defined:
	// division by 0.
#if defined DEBUG
	assert(n > 3);
#endif

	const rational k2_linear = rational_from_ui(4*n - 6, n);
	const rational k2_star = rational_from_ui(n - 1);
	const rational k2_graph = mmt_degree_rational(g, 2);
	return (k2_graph - k2_linear)/(k2_star - k2_linear);
}

double hubiness(const free_tree& g) {
	return hubiness_rational(g).to_double();
}

} // -- namespace properties
} // -- namespace lal
