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

#define to_double(x) static_cast<double>(x)

namespace lal {
using namespace graphs;
using namespace numeric;

namespace properties {

// g: input graph
// p: moment of the *-degree
// D: function that returns the *-degree
template<
	class G, class restype, class Callable,

	class numtype = std::conditional_t<
		std::is_same_v<restype, rational>,
		integer,
		uint32_t
	>
>
inline
restype __mmt_x_degree_rational
(const G& g, uint32_t p, const Callable& D)
{
	static_assert(
		std::is_same_v<restype, double> ||
		std::is_same_v<restype, numeric::rational>
	);

	const numtype M = [&]() {
		if constexpr (std::is_same_v<numtype, numeric::integer>) {
			 return integer_from_ui(g.get_num_nodes());
		}
		else {
			 return g.get_num_nodes();
		}
	}();

	numtype S(0);
	numtype du(0);
	for (node u = 0; u < g.get_num_nodes(); ++u) {
		const uint32_t deg = D(g, u);
		if constexpr (std::is_same_v<numtype, numeric::integer>) {
			du.set_ui(deg);
			du ^= p;
		}
		else {
			du = 1;
			for (uint32_t i = 0; i < p; ++i) {
				du *= deg;
			}
		}

		S += du;
	}
	if constexpr (std::is_same_v<restype, numeric::rational>) {
		return rational(S, M);
	}
	else {
		return to_double(S)/to_double(M);
	}
}

// -----------------------------------------------------------------------------

// moment of degree

rational moment_degree_rational(const undirected_graph& g, uint32_t p) noexcept
{
	return
	__mmt_x_degree_rational<undirected_graph, rational>
	(
		g, p,
		[](const undirected_graph& _g, node _u) -> uint32_t
		{ return _g.get_degree(_u); }
	);
}

double moment_degree(const undirected_graph& g, uint32_t p) noexcept
{
	return
	__mmt_x_degree_rational<undirected_graph, double>
	(
		g, p,
		[](const undirected_graph& _g, node _u) -> uint32_t
		{ return _g.get_degree(_u); }
	);
}

rational moment_degree_rational(const directed_graph& g, uint32_t p) noexcept
{
	return
	__mmt_x_degree_rational<directed_graph, rational>
	(
		g, p,
		[](const directed_graph& _g, node _u) -> uint32_t
		{ return _g.get_degree(_u); }
	);
}

double moment_degree(const directed_graph& g, uint32_t p) noexcept
{
	return
	__mmt_x_degree_rational<directed_graph, double>
	(
		g, p,
		[](const directed_graph& _g, node _u) -> uint32_t
		{ return _g.get_degree(_u); }
	);
}

// moment of in-degree

rational moment_degree_in_rational(const directed_graph& g, uint32_t p) noexcept
{
	return
	__mmt_x_degree_rational<directed_graph, rational>
	(
		g, p,
		[](const directed_graph& _g, node _u) -> uint32_t
		{ return _g.get_in_degree(_u); }
	);
}

double moment_degree_in(const directed_graph& g, uint32_t p) {
	return
	__mmt_x_degree_rational<directed_graph, double>
	(
		g, p,
		[](const directed_graph& _g, node _u) -> uint32_t
		{ return _g.get_in_degree(_u); }
	);
}

// moment of out-degree

rational moment_degree_out_rational(const directed_graph& g, uint32_t p) noexcept
{
	return
	__mmt_x_degree_rational<directed_graph, rational>
	(
		g, p,
		[](const directed_graph& _g, node _u) -> uint32_t
		{ return _g.get_out_degree(_u); }
	);
}

double moment_degree_out(const directed_graph& g, uint32_t p) noexcept
{
	return
	__mmt_x_degree_rational<directed_graph, double>
	(
		g, p,
		[](const directed_graph& _g, node _u) -> uint32_t
		{ return _g.get_out_degree(_u); }
	);
}

// hubiness

rational hubiness_rational(const free_tree& g) noexcept
{
	const uint32_t n = g.get_num_nodes();

	// for n <= 3, <k^2>_star = <k^2>_linear
	// which means that hubiness is not defined:
	// division by 0.
#if defined DEBUG
	assert(n > 3);
#endif

	const rational k2_linear = rational_from_ui(4*n - 6, n);
	const rational k2_star = rational_from_ui(n - 1);
	const rational k2_graph = moment_degree_rational(g, 2);
	return (k2_graph - k2_linear)/(k2_star - k2_linear);
}

double hubiness(const free_tree& g) noexcept
{
	const uint32_t n = g.get_num_nodes();

	// for n <= 3, <k^2>_star = <k^2>_linear
	// which means that hubiness is not defined:
	// division by 0.
#if defined DEBUG
	assert(n > 3);
#endif

	const double k2_linear = to_double(4*n - 6)/to_double(n);
	const double k2_star = to_double(n - 1);
	const double k2_graph = moment_degree(g, 2);
	return (k2_graph - k2_linear)/(k2_star - k2_linear);
}

} // -- namespace properties
} // -- namespace lal
