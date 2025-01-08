/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <vector>

// lal includes
#include <lal/iterators/E_iterator.hpp>
#include <lal/iterators/Q_iterator.hpp>

namespace lal {
namespace detail {

/**
 * @brief Enumerate the set of edges of the input graph @e g.
 * @param g Input graph.
 * @returns A vector with all @e g's edges.
 */
template <class graph_t>
[[nodiscard]] std::vector<edge> set_edges(const graph_t& g) noexcept {
	std::vector<edge> e(g.get_num_edges());
	auto vec_it = e.begin();
	for (iterators::E_iterator<graph_t> e_it(g); not e_it.end(); e_it.next()) {
		*vec_it++ = e_it.get_edge();
	}
	return e;
}

/**
 * @brief Enumerate the set of pairs of independent edges of the input graph @e g.
 * @param g Input graph.
 * @param qs Total amount of pairs of independent edges.
 * @returns A vector with all @e g's pairs of independent edges.
 */
template <class graph_t>
[[nodiscard]] std::vector<edge_pair> set_pairs_independent_edges
(const graph_t& g, const uint64_t qs)
noexcept
{
	std::vector<edge_pair> q(qs);
	auto vec_it = q.begin();
	for (iterators::Q_iterator<graph_t> q_it(g); not q_it.end(); q_it.next()) {
		*vec_it++ = q_it.get_edge_pair();
	}
	return q;
}

} // -- namespace detail
} // -- namespace lal
