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

#include <lal/generate/rand_arrangements.hpp>

// C++ includes
#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include <iostream>
#include <lal/utils/std_utils.hpp>
using namespace std;

typedef vector<lal::node> interval;

// --------------------

namespace lal {
using namespace graphs;

namespace generate {

void put_in_arrangement(
	const rtree& T, node r,
	const vector<interval>& data,
	uint32_t& pos, linearrgmnt& arr
)
{
	// number of children of 'r' with respect to the tree's root
	const uint32_t d_out = T.degree(r);

	// vertex 'r' is a leaf
	if (d_out == 0) {
		arr[r] = pos;
		pos += 1;
		return;
	}
	const vector<node>& interval = data[r];
	for (size_t i = 0; i < interval.size(); ++i) {
		const node vi = interval[i];
		if (vi == r) {
			arr[vi] = pos;
			pos += 1;
		}
		else {
			put_in_arrangement(T, vi, data, pos, arr);
		}
	}
}

template<class GEN>
void random_data(
	const rtree& T, node r,
	// Its size must be equal to the number of vertices of the tree.
	vector<interval>& data,
	// random number generator
	GEN& gen
)
{
	// number of children of 'r' with respect to the tree's root
	const uint32_t d_out = T.out_degree(r);
	const neighbourhood& neighs = T.get_out_neighbours(r);

	// Choose random positions for the intervals corresponding to the
	// vertex 'r' and to the trees rooted at 'r's children. These choices
	// have to be made with respect to 'r'. Remember: there are n_children+1
	// possibilities.

	cout << "    d_out+1= " << d_out+1 << endl;
	data[r] = vector<node>(d_out + 1);

	// fill interval with the root vertex and its children
	interval& inter = data[r];
	inter[0] = r;
	for (size_t i = 0; i < neighs.size(); ++i) {
		inter[i+1] = neighs[i];
	}

	cout << inter << endl;

	// shuffle the positions
	shuffle(inter.begin(), inter.end(), gen);

	// Choose random positions for the intervals corresponding to the
	// other vertices. Compute them inductively.

	for (const node& v : neighs) {
		random_data(T, v, data, gen);
	}
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

	cout << "phase 1" << endl;

	// phase 1 -- generate random data
	vector<interval> vdata(t.n_nodes());
	random_data(t, t.get_root(), vdata, gen);

	cout << "phase 2" << endl;

	// phase 2 -- construct arrangement
	linearrgmnt arr(t.n_nodes());
	uint32_t pos = 0;
	put_in_arrangement(t, t.get_root(), vdata, pos, arr);

	return arr;
}

} // -- namespace generate
} // -- namespace lal
