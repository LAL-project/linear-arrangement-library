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
#include <lal/numeric/rational.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/internal/macros.hpp>

namespace lal {
using namespace graphs;
using namespace numeric;
using namespace iterators;

namespace linarr {

// -----------------------------------------------------------------------------
/* D */

template<class G>
inline uint32_t __sum_length_edges(const G& g, const linear_arrangement& pi)
noexcept
{
	// sum of lengths
	uint32_t l = 0;

	for (E_iterator<G> e_it(g); not e_it.end(); e_it.next()) {
		const auto [u,v] = e_it.get_edge();

		// accumulate length of edge
		l += (pi[u] < pi[v] ? pi[v] - pi[u] : pi[u] - pi[v]);
	}
	return l;
}

uint32_t sum_edge_lengths
(const directed_graph& g, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif

	return internal::call_with_empty_arrangement
			(__sum_length_edges<directed_graph>, g, pi);
}
uint32_t sum_edge_lengths
(const undirected_graph& g, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif

	return internal::call_with_empty_arrangement
			(__sum_length_edges<undirected_graph>, g, pi);
}

// -----------------------------------------------------------------------------
/* MDD */

template<class G, typename result>
inline result __MDD_rational(const G& g, const linear_arrangement& pi) noexcept
{
	if (g.get_num_nodes() <= 1) { return -1; }

	const uint32_t D = sum_edge_lengths(g, pi);
	if constexpr (std::is_same_v<result, numeric::rational>) {
		return rational_from_ui(D, g.get_num_edges());
	}
	else {
		return D/g.get_num_edges();
	}
}


rational mean_dependency_distance_rational
(const directed_graph& g, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif

	return internal::call_with_empty_arrangement
			(__MDD_rational<directed_graph,rational>, g, pi);
}

rational mean_dependency_distance_rational
(const undirected_graph& g, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif

	return internal::call_with_empty_arrangement
			(__MDD_rational<undirected_graph,rational>, g, pi);
}

// -----------------------------------------------------------------------------

double mean_dependency_distance
(const directed_graph& g, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif

	return internal::call_with_empty_arrangement
			(__MDD_rational<directed_graph,double>, g, pi);
}
double mean_dependency_distance
(const undirected_graph& g, const linear_arrangement& pi)
noexcept
{
#if defined DEBUG
	assert(pi.size() == 0 or g.get_num_nodes() == pi.size());
#endif

	return internal::call_with_empty_arrangement
			(__MDD_rational<undirected_graph,double>, g, pi);
}

} // -- namespace linarr
} // -- namespace lal
