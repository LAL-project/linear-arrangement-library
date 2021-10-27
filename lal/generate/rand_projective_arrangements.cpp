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

#include <lal/generate/rand_projective_arrangements.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <algorithm>
#include <vector>

// lal includes
#include <lal/detail/graphs/make_arrangement.hpp>

namespace lal {
namespace generate {

rand_projective_arrangements::rand_projective_arrangements
(const graphs::rooted_tree& rT, uint64_t seed)
noexcept
	: m_rT(rT),
	  m_rdata(m_rT.get_num_nodes())
{
#if defined DEBUG
	assert(m_rT.is_rooted_tree());
#endif
	if (seed == 0) {
		std::random_device rd;
		m_gen = std::mt19937(rd());
	}
	else {
		m_gen = std::mt19937(seed);
	}

	// initialise the random data of all vertices
	for (node u = 0; u < m_rT.get_num_nodes(); ++u) {
		// the children of vertex 'u'
		const neighbourhood& neighs = m_rT.get_out_neighbours(u);

		// fill interval with the root vertex and its children
		auto& interval = m_rdata[u];
		interval.resize(m_rT.get_out_degree(u) + 1);

		std::copy(neighs.begin(), neighs.end(), interval.begin());
		interval.back() = u;
	}
}

linear_arrangement rand_projective_arrangements::get_arrangement() noexcept {
	if (m_rT.get_num_nodes() == 1) {
		return linear_arrangement::identity(1);
	}

	// generate random intervals
	for (node u = 0; u < m_rT.get_num_nodes(); ++u) {
		auto& interval = m_rdata[u];
		// Choose random positions for the interval
		std::shuffle(interval.begin(), interval.end(), m_gen);
	}

	// generate arrangement from data
	return detail::make_arrangement_intervals(m_rT, m_rdata);
}

} // -- namespace generate
} // -- namespace lal
