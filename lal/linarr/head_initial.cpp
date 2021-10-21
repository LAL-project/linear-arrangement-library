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

#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/detail/macros.hpp>

#define to_double(x) static_cast<double>(x)

namespace lal {
namespace linarr {

inline
uint64_t __left_branching_edges(const graphs::directed_graph& g, const linear_arrangement& pi)
noexcept
{
	uint64_t edges_to_right = 0;
	for (iterators::E_iterator e_it(g); not e_it.end(); e_it.next()) {
		const auto [u,v] = e_it.get_edge();
		edges_to_right += pi[u] < pi[v];
	}
	return edges_to_right;
}

numeric::rational head_initial_rational
(const graphs::directed_graph& g, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(g.get_num_edges() > 0);
#endif

	const uint64_t etr =
		detail::call_with_empty_arrangement(__left_branching_edges, g, pi);
	return numeric::rational(etr, g.get_num_edges());
}

double head_initial
(const graphs::directed_graph& g, const linear_arrangement& pi) noexcept
{
#if defined DEBUG
	assert(g.get_num_edges() > 0);
#endif

	const uint64_t etr =
		detail::call_with_empty_arrangement(__left_branching_edges, g, pi);
	return to_double(etr)/to_double(g.get_num_edges());
}

} // -- namespace linarr
} // -- namespace lal
