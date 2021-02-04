/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#include <random>
#include <vector>
using namespace std;

// lal includes
#include <lal/internal/graphs/trees/make_arrangement.hpp>

namespace lal {
using namespace graphs;
using namespace internal;

namespace generate {

rand_projective_arrgmnt::rand_projective_arrgmnt
(const rooted_tree& rT, uint32_t seed) noexcept
	: m_rT(rT)
{
#if defined DEBUG
	assert(m_rT.is_rooted_tree());
#endif
	if (seed == 0) {
		random_device rd;
		m_gen = mt19937(rd());
	}
	else {
		m_gen = mt19937(seed);
	}

	// initialise the random data of all vertices
	rdata = vector<vector<node>>(m_rT.n_nodes());
	for (node u = 0; u < m_rT.n_nodes(); ++u) {
		const uint32_t deg = m_rT.out_degree(u);
		rdata[u] = vector<node>(deg + 1);
	}
}

linear_arrangement rand_projective_arrgmnt::make_rand_arrgmnt() {
	if (m_rT.n_nodes() == 1) {
		return linear_arrangement(1, 0);
	}

	for (node u = 0; u < m_rT.n_nodes(); ++u) {
		// -- generate random data for a single vertex

		// number of children of 'r' with respect to the tree's root
		const neighbourhood& neighs = m_rT.get_out_neighbours(u);

		// Choose random positions for the intervals corresponding to the
		// vertex 'r' and to the trees rooted at 'r's children. These choices
		// have to be made with respect to 'r'. Remember: there are d_out+1
		// possibilities.

		// fill interval with the root vertex and its children
		vector<node>& interval = rdata[u];
		interval[0] = u;
		for (size_t i = 0; i < neighs.size(); ++i) {
			interval[i+1] = neighs[i];
		}

		// shuffle the positions
		shuffle(interval.begin(), interval.end(), m_gen);
	}

	// generate arrangement from data
	return make_arrangement_intervals(m_rT, rdata);
}

} // -- namespace generate
} // -- namespace lal
