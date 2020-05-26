/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#include <algorithm>
#include <cassert>
#include <random>
#include <vector>
using namespace std;

// lal includes
#include <lal/utils/graphs/trees/make_projective_arr.hpp>

#define degree_vertex(T, u)										\
	(T.get_rtree_type() == rtree::rtree_type::arborescence ?	\
		T.out_degree(u) : T.in_degree(u) )

#define neighbours_vertex(T, u)									\
	(T.get_rtree_type() == rtree::rtree_type::arborescence ?	\
		T.get_out_neighbours(u) : T.get_in_neighbours(u) )

namespace lal {
using namespace graphs;
using namespace utils;

namespace generate {

rand_projective_arrgmnt::rand_projective_arrgmnt(const rtree& rT, bool seed)
	: m_rT(rT)
{
	assert(rT.is_rooted_tree());
	if (seed) {
		random_device rd;
		gen = mt19937(rd());
	}
}
rand_projective_arrgmnt::~rand_projective_arrgmnt() { }

linearrgmnt rand_projective_arrgmnt::make_rand_arrgmnt() {
	if (m_rT.n_nodes() == 1) {
		return linearrgmnt(1, 0);
	}

	// the random data of all vertices
	vector<vector<node>> vdata(m_rT.n_nodes());

	for (node u = 0; u < m_rT.n_nodes(); ++u) {
		// -- generate random data for a single vertex

		// number of children of 'r' with respect to the tree's root
		const uint32_t deg = degree_vertex(m_rT, u);
		const neighbourhood& neighs = neighbours_vertex(m_rT, u);

		// Choose random positions for the intervals corresponding to the
		// vertex 'r' and to the trees rooted at 'r's children. These choices
		// have to be made with respect to 'r'. Remember: there are d_out+1
		// possibilities.

		vdata[u] = vector<node>(deg + 1);

		// fill interval with the root vertex and its children
		vector<node>& interval = vdata[u];
		interval[0] = u;
		for (size_t i = 0; i < neighs.size(); ++i) {
			interval[i+1] = neighs[i];
		}

		// shuffle the positions
		shuffle(interval.begin(), interval.end(), gen);
	}
	return put_in_arrangement(m_rT, vdata);
}

} // -- namespace generate
} // -- namespace lal
