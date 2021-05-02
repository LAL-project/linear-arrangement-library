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
#include <lal/graphs/directed_graph.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/internal/macros.hpp>

namespace lal {
using namespace graphs;
using namespace numeric;

namespace linarr {

inline uint32_t __headedness_rational
(const directed_graph& g, const linear_arrangement& pi) noexcept
{
	uint32_t edges_to_right = 0;
	for (iterators::E_iterator e_it(g); not e_it.end(); e_it.next()) {
		const auto [u,v] = e_it.get_edge();
		edges_to_right += pi[u] < pi[v];
	}
	return edges_to_right;
}

rational head_initial_rational
(const directed_graph& g, const linear_arrangement& pi) noexcept
{
	const uint32_t etr =
		internal::call_with_empty_arrangement(__headedness_rational, g, pi);

	// avoid warning conversion
	return rational_from_ui(etr, g.get_num_edges());
}

} // -- namespace linarr
} // -- namespace lal
