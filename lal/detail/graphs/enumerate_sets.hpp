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

#pragma once

// C++ includes
#include <vector>

// lal includes
#include <lal/iterators/E_iterator.hpp>
#include <lal/iterators/Q_iterator.hpp>

namespace lal {
namespace detail {

template<typename G>
inline std::vector<edge> E(const G& g) noexcept {
	std::vector<edge> e(g.get_num_edges());

	auto vec_it = e.begin();
	for (iterators::E_iterator<G> e_it(g); not e_it.end(); e_it.next()) {
		*vec_it++ = e_it.get_edge();
	}
	return e;
}

template<typename G>
inline std::vector<edge_pair> Q(const G& g, uint64_t qs) noexcept {
	std::vector<edge_pair> q(qs);

	auto vec_it = q.begin();
	for (iterators::Q_iterator<G> q_it(g); not q_it.end(); q_it.next()) {
		*vec_it++ = q_it.get_edge_pair();
	}
	return q;
}

} // -- namespace detail
} // -- namespace lal
