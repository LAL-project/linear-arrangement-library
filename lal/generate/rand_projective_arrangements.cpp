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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
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
#include <lal/utils/graphs/trees/make_projecitve_arr.hpp>

namespace lal {
using namespace graphs;
using namespace utils;

namespace generate {

template<class GEN>
void random_data(
	const rtree& T, node r,
	// Its size must be equal to the number of vertices of the tree.
	vector<vector<node>>& data,
	// random number generator
	GEN& gen
)
{
	// number of children of 'r' with respect to the tree's root
	const uint32_t d_out = T.out_degree(r);
	const neighbourhood& neighs = T.get_out_neighbours(r);

	// Choose random positions for the intervals corresponding to the
	// vertex 'r' and to the trees rooted at 'r's children. These choices
	// have to be made with respect to 'r'. Remember: there are d_out+1
	// possibilities.

	data[r] = vector<node>(d_out + 1);

	// fill interval with the root vertex and its children
	vector<node>& interval = data[r];
	interval[0] = r;
	for (size_t i = 0; i < neighs.size(); ++i) {
		interval[i+1] = neighs[i];
	}

	// shuffle the positions
	shuffle(interval.begin(), interval.end(), gen);
}

linearrgmnt rand_projective_arrgmnt(const rtree& t, bool seed) {
	if (t.n_nodes() == 1) {
		return linearrgmnt(1, 0);
	}

	mt19937 gen;
	if (seed) {
		random_device rd;
		gen = mt19937(rd());
	}

	// generate random data
	vector<vector<node>> vdata(t.n_nodes());
	for (node u = 0; u < t.n_nodes(); ++u) {
		random_data(t, u, vdata, gen);
	}

	return put_in_arrangement(t, vdata);
}

} // -- namespace generate
} // -- namespace lal
