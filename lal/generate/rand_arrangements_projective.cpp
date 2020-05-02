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
#include <random>
#include <vector>
using namespace std;


// --------------------

struct _data {
	_data() { }
	~_data() { }

	// From position to node:
	// interval[i] = u <-> at position 'i' there is node 'u'
	vector<lal::node> interval;
};

// --------------------

namespace lal {
using namespace graphs;

namespace generate {

template<class Tree>
void put_in_arrangement(
	const Tree& T, node r,
	const vector<_data>& vdata,
	uint32_t& pos, linearrgmnt& arr
)
{
	// number of children of 'r' with respect to the tree's root
	const uint32_t n_children =
		(T.is_directed() ?
			T.degree(r) :
			T.degree(r) - (r == T.get_root() ? 0 : 1)
		);

	// vertex 'r' is a leaf
	if (n_children == 0) {
		arr[r] = pos;
		pos += 1;
		return;
	}
	const vector<node>& interval = vdata[r].interval;
	for (size_t i = 0; i < interval.size(); ++i) {
		const node vi = interval[i];
		if (vi == r) {
			arr[vi] = pos;
			pos += 1;
		}
		else {
			put_in_arrangement(T, vi, vdata, pos, arr);
		}
	}
}

/* -------------------------------------------------------------------------- */

template<class GEN>
void random_data(
	const drtree& T, node r,
	// Its size must be equal to the number of vertices of the tree.
	vector<_data>& vdata,
	// random number generator
	GEN& gen
)
{
	// number of children of 'r' with respect to the tree's root
	const uint32_t n_children = T.out_degree(r);
	const neighbourhood& neighs = T.get_out_neighbours(r);

	// Choose random positions for the intervals corresponding to the
	// vertex 'r' and to the trees rooted at 'r's children. These choices
	// have to be made with respect to 'r'. Remember: there are n_children+1
	// possibilities.

	vdata[r].interval = vector<node>(n_children + 1);

	// fill interval with the root vertex and its children
	vector<node>& interval = vdata[r].interval;
	interval[n_children] = r;
	for (size_t i = 0; i < neighs.size(); ++i) {
		interval[i] = neighs[i];
	}

	// shuffle the positions
	shuffle(interval.begin(), interval.end(), gen);

	// Choose random positions for the intervals corresponding to the
	// other vertices. Compute them inductively.

	for (const node& v : neighs) {
		random_data(T, v, vdata, gen);
	}
}

linearrgmnt rand_projective_arrgmnt(const drtree& t, bool seed) {
	if (t.n_nodes() == 1) {
		return linearrgmnt(1, 0);
	}

	mt19937 gen;
	if (seed) {
		random_device rd;
		gen = mt19937(rd());
	}

	// phase 1 -- generate random data
	vector<_data> vdata(t.n_nodes());
	random_data(t, t.get_root(), vdata, gen);

	// phase 2 -- construct arrangement
	linearrgmnt arr(t.n_nodes());
	uint32_t pos = 0;
	put_in_arrangement(t, t.get_root(), vdata, pos, arr);

	return arr;
}

/* -------------------------------------------------------------------------- */

template<class GEN>
void random_data(
	const urtree& T, node parent, node r,
	// Its size must be equal to the number of vertices of the tree.
	vector<_data>& vdata,
	// random number generator
	GEN& gen
)
{
	// number of children of 'r' with respect to the tree's root
	const uint32_t n_children = T.degree(r) - (r == T.get_root() ? 0 : 1);
	const neighbourhood& neighs = T.get_neighbours(r);

	// Choose random positions for the intervals corresponding to the
	// vertex 'r' and to the trees rooted at 'r's children. These choices
	// have to be made with respect to 'r'. Remember: there are n_children+1
	// possibilities.

	vdata[r].interval = vector<node>(n_children + 1);

	// fill interval with the root vertex and its children
	vector<node>& interval = vdata[r].interval;
	interval[n_children] = r;
	for (
		size_t interval_it = 0, neighs_it = 0;
		neighs_it < neighs.size();
		++neighs_it
	)
	{
		const node vi = neighs[neighs_it];
		interval[interval_it] = vi;

		//if (r != T.get_root() and vi == parent) { i += 0; }
		//else { i += 1; }

		//if (r == T.get_root() or vi != parent) { i += 1; }
		//else { i += 0; }
		interval_it += (r == T.get_root() or vi != parent ? 1 : 0);
	}

	// shuffle the positions
	shuffle(interval.begin(), interval.end(), gen);

	// Choose random positions for the intervals corresponding to the
	// other vertices. Compute them inductively.

	for (const node& v : neighs) {
		if (r == T.get_root() or v != parent) {
			random_data(T, r, v, vdata, gen);
		}
	}
}

linearrgmnt rand_projective_arrgmnt(const urtree& t, bool seed) {
	if (t.n_nodes() == 1) {
		return linearrgmnt(1, 0);
	}

	mt19937 gen;
	if (seed) {
		random_device rd;
		gen = mt19937(rd());
	}

	// phase 1 -- generate random data
	vector<_data> vdata(t.n_nodes());
	random_data(t, t.n_nodes()+1, t.get_root(), vdata, gen);

	// phase 2 -- construct arrangement
	linearrgmnt arr(t.n_nodes());
	uint32_t pos = 0;
	put_in_arrangement(t, t.get_root(), vdata, pos, arr);

	return arr;
}

} // -- namespace generate
} // -- namespace lal
